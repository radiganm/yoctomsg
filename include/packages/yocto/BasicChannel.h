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
      inline size_t read(T *buf, size_t size);
      inline size_t write(const T* const buf, size_t size);
      void summarize(std::ostream &os);
    private:
      std::atomic_int n_in_;  // write in
      std::atomic_int n_out_; // read out
      std::mutex lck_in_;
      std::mutex lck_out_;
      std::condition_variable cv_in_;
      std::condition_variable cv_out_;
      std::array<T, N> storage_;
      unsigned char *buffer_;
  };

} // namespace

  template<typename T, std::size_t N>
  void rad::yocto::BasicChannel<T,N>::summarize(std::ostream &os)
  {
    std::stringstream ss;
    os << ss.str();
  }

  template<typename T, std::size_t N>
  size_t rad::yocto::BasicChannel<T,N>::read(T *data, size_t data_size)
  {
    std::unique_lock<std::mutex> lck(lck_out_);
    const size_t buffer_size = N * sizeof(T);
    const size_t n_in  = n_in_.load();
    const size_t n_out = n_out_.load();
    if(n_in <= n_out) cv_out_.wait(lck, [&]{return n_in_.load() > n_out_.load();});
    while(data_size > 0)
    {
      const size_t n_delta = n_in - n_out;
      const size_t k = (n_out + n_delta) % buffer_size;
      const size_t m_delta = k % sizeof(T);
      const size_t M = m_delta / sizeof(T);
      data_size -= M;
      n_out_ -= m_delta;
      std::copy(&storage_[0], &storage_[M], data);
      cv_in_.notify_one();
    }
    return data_size;
  }

  template<typename T, std::size_t N>
  size_t rad::yocto::BasicChannel<T,N>::write(const T* const data, size_t data_size)
  {
    std::unique_lock<std::mutex> lck(lck_in_);
    const size_t buffer_size = N * sizeof(T);
    const size_t n_in  = n_in_.load();
    const size_t n_out = n_out_.load();
    if(n_in <= n_out) cv_in_.wait(lck, [&]{return n_in_.load() > n_out_.load();});
    while(data_size > 0)
    {
      const size_t n_delta = n_in - n_out;
      const size_t k = (n_out + n_delta) % buffer_size;
      const size_t m_delta = k % sizeof(T);
      const size_t M = m_delta / sizeof(T);
      data_size -= M;
      n_in_ -= m_delta;
      std::copy(data, data+M, &storage_[0]);
      cv_in_.notify_one();
    }
    return data_size;
  }

#endif

/* *EOF* */
