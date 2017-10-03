/* ShmemChannel.h
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

#ifndef yo_ShmemChannel_h
#define yo_ShmemChannel_h

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

namespace rad::yocto {

  template<typename T, std::size_t N>
  class ShmemChannel : public rad::yocto::Channel<T,N> 
  {
    public:
      ShmemChannel() : Channel<T,N>(), 
                       read_index_(0),
                       write_index_(0),
                       count_(0) {};
      virtual ~ShmemChannel() {};
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
      std::array<T, N> buffer_;
  };

} // namespace

  template<typename T, std::size_t N>
  size_t rad::yocto::ShmemChannel<T,N>::read(T *data, size_t size)
  {
    std::unique_lock<std::mutex> lck(read_mutex_);
    for(size_t k=0; k<size; ++k) {
      read_cv_.wait(lck, [&]{return count_.load() > 0;});
      data[k] = buffer_[read_index_];
      read_index_ =  (read_index_ + 1) % N;
      --count_;
      write_cv_.notify_one();
    }
  }

  template<typename T, std::size_t N>
  size_t rad::yocto::ShmemChannel<T,N>::write(const T* const data, size_t size)
  {
    std::unique_lock<std::mutex> lck(write_mutex_);
    for(size_t k=0; k<size; ++k) {
      write_cv_.wait(lck, [&]{return count_.load() < N;});
      buffer_[write_index_] = data[k];
      write_index_ =  (write_index_ + 1) % N;
      ++count_;
      read_cv_.notify_one();
    }
  }

#endif

/* *EOF* */
