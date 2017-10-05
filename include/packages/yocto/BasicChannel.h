/* BasicChannel.h
 * Copyright 2017 Mac Radigan
 * All Rights Reserved
 */

#include "packages/yocto/Channel.h"

#include <stdlib.h>
#include <inttypes.h>
#include <pthread.h>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <array>
#include <atomic>
#include <sstream>
#include <cstring>
#include <iostream>

#ifndef yo_BasicChannel_h
#define yo_BasicChannel_h

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

namespace rad::yocto {

  template<typename T, std::size_t N>
  class BasicChannel : public rad::yocto::Channel<T,N> 
  {
    public:
      BasicChannel() : Channel<T,N>(), n_in_(0), n_out_(0) 
      { 
        this->buffer_ = reinterpret_cast<unsigned char *>(&storage_[0]); 
      };
      virtual ~BasicChannel() {};
      inline std::size_t read(T *buf, std::size_t size);
      inline std::size_t write(const T* const buf, std::size_t size);
      void summarize(std::ostream &os);
    private:
      std::atomic_uint64_t n_in_;  // write in
      std::atomic_uint64_t n_out_; // read out
      std::mutex lck_in_;
      std::mutex lck_out_;
      std::condition_variable cv_in_;
      std::condition_variable cv_out_;
      std::array<T, N> storage_;
      unsigned char *buffer_;
      std::size_t buffer_size_ = N * sizeof(T);
      template<typename U, std::size_t V>
      friend std::ostream& operator<<(std::ostream &os, const BasicChannel<U,V>& o);
  };

} // namespace

  template<typename T, std::size_t N>
  std::ostream& operator<<(std::ostream &os, const rad::yocto::BasicChannel<T,N>& o)
  {
    o.summarize();
    return os;
  }

  template<typename T, std::size_t N>
  void rad::yocto::BasicChannel<T,N>::summarize(std::ostream &os)
  {
    std::unique_lock<std::mutex> lck_in(lck_in_);
    std::unique_lock<std::mutex> lck_out(lck_out_);
    const std::size_t n_in  = n_in_.load();
    const std::size_t n_out = n_out_.load();
    const std::size_t n_delta = n_in-n_out;
    os <<      "IN:" << n_in 
       <<    " OUT:" << n_out 
       << " BUFFER:" << n_delta 
       << std::endl << std::flush;
  }

  template<typename T, std::size_t N>
  size_t rad::yocto::BasicChannel<T,N>::read(T *data, std::size_t data_size)
  {
    std::unique_lock<std::mutex> lck(lck_out_);
    auto test_fn = [&](void) -> bool { return n_in_.load() <= n_out_.load(); };
    std::size_t xfer_size = 0;
    while(data_size > 0)
    {
      if(test_fn()) cv_out_.wait(lck, [&]{return !test_fn();});
      const std::size_t n_in  = n_in_.load();
      const std::size_t n_out = n_out_.load();
      const std::size_t n_delta = std::min(data_size, n_in-n_out);
      const std::size_t k = (n_out + n_delta) % buffer_size_;
      const std::size_t k_size = std::min(buffer_size_-k, data_size);
      std::memcpy(&data[xfer_size], &buffer_[k], k_size);
      n_out_ += k_size;
      data_size -= k_size;
      xfer_size += k_size;
      cv_in_.notify_one();
    }
    return xfer_size;
  }

  template<typename T, std::size_t N>
  size_t rad::yocto::BasicChannel<T,N>::write(const T* const data, std::size_t data_size)
  {
    std::unique_lock<std::mutex> lck(lck_in_);
    auto test_fn = [&](void) -> bool { n_in_.load() - n_out_.load() >= buffer_size_; };
    std::size_t xfer_size = 0;
    while(data_size > 0)
    {
      if(test_fn()) cv_in_.wait(lck, [&]{return !test_fn();});
      const std::size_t n_in  = n_in_.load();
      const std::size_t n_out = n_out_.load();
      const std::size_t n_delta = std::min(data_size, n_in-n_out);
      const std::size_t k = (n_in + n_delta) % buffer_size_;
      const std::size_t k_size = std::min(buffer_size_-k, data_size);
      std::memcpy(&buffer_[k], &data[xfer_size], k_size);
      n_in_ += k_size;
      data_size -= k_size;
      xfer_size += k_size;
      cv_out_.notify_one();
    }
    return xfer_size;
  }

#endif

/* *EOF* */
