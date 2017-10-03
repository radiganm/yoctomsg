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
      BasicChannel() : Channel<T,N>(), 
                       read_index_(0),
                       write_index_(0),
                       count_(0) 
                       {};
                     //{ this->buffer_ = reinterpret_cast<char *>(&storage_[0]); };
      virtual ~BasicChannel() {};
      inline size_t read(T *buf, size_t size);
      inline size_t write(const T* const buf, size_t size);
      void summarize(std::ostream &os);
    private:
      size_t read_index_;
      size_t write_index_;
      std::atomic_int count_;
      std::mutex read_mutex_;
      std::mutex write_mutex_;
      std::condition_variable read_cv_;
      std::condition_variable write_cv_;
      std::array<T, N> storage_;
  };

} // namespace

  template<typename T, std::size_t N>
  void rad::yocto::BasicChannel<T,N>::summarize(std::ostream &os)
  {
    std::unique_lock<std::mutex> lck_read(read_mutex_);
    std::unique_lock<std::mutex> lck_write(write_mutex_);
    std::stringstream ss;
    const std::string delim = "";
    const std::string sep = "|";
    size_t m = count_.load();
    ss << std::endl;
    ss << m << "(" << read_index_ << "," << write_index_ << ") ";
    for(size_t k=0; k<m; ++k) {
      unsigned char c = *reinterpret_cast<unsigned char *>((&storage_[k].data[0]));
      ss << c << delim;
    }
    ss << sep;
    for(size_t k=m+1; k<N; ++k) {
      unsigned char c = *reinterpret_cast<unsigned char *>((&storage_[k].data[0]));
      ss << c << delim;
    }
    ss << std::endl;
    os << ss.str();
  }

  template<typename T, std::size_t N>
  size_t rad::yocto::BasicChannel<T,N>::read(T *data, size_t size)
  {
    std::unique_lock<std::mutex> lck(read_mutex_);
    std::stringstream ss;
    for(size_t k=0; k<size; ++k) {
      read_cv_.wait(lck, [&]{return count_.load() > 0;});
      data[k] = storage_[read_index_];
      read_index_ =  (read_index_ + 1) % N;
      --count_;
      write_cv_.notify_one();
    }
    return size;
  }

  template<typename T, std::size_t N>
  size_t rad::yocto::BasicChannel<T,N>::write(const T* const data, size_t size)
  {
    std::unique_lock<std::mutex> lck(write_mutex_);
    for(size_t k=0; k<size; ++k) {
      write_cv_.wait(lck, [&]{return count_.load() <= N;});
      storage_[write_index_] = data[k];
      write_index_ =  (write_index_ + 1) % N;
      ++count_;
      read_cv_.notify_one();
    }
    return size;
  }

#endif

/* *EOF* */
