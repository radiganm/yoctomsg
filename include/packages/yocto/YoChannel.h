/* YoChannel.h
 * Copyright 2017 Mac Radigan
 * All Rights Reserved
 */

#include "packages/yocto/SharedMemory.h"

#include <stdlib.h>
#include <inttypes.h>
#include <pthread.h>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <array>
#include <atomic>
#include <cstring>

#ifndef yo_YoChannel_h
#define yo_YoChannel_h

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

namespace rad::yocto {

  class YoChannel
  {
    public:
      YoChannel(const std::string &name, std::size_t size, bool owner=true)
        : n_in_(0), n_out_(0), buffer_size_(size), shmem_(name, size, owner)
      { 
        buffer_ = reinterpret_cast<unsigned char *>(shmem_.get_data());
      };
      virtual ~YoChannel() {};
      inline size_t read(void *buf, size_t size);
      inline size_t write(const void* const buf, size_t size);
      void summarize(std::ostream &os);
      virtual void release();
    private:
      std::atomic_uint64_t n_in_;  // write in
      std::atomic_uint64_t n_out_; // read out
      SharedMemory shmem_;
      std::mutex lck_in_;
      std::mutex lck_out_;
      std::condition_variable cv_in_;
      std::condition_variable cv_out_;
      unsigned char *buffer_;
      std::size_t buffer_size_;
  };

} // namespace

  size_t rad::yocto::YoChannel::read(void *data, std::size_t data_size)
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
      unsigned char *cdata = reinterpret_cast<unsigned char *>(data);
      std::memcpy(&cdata[xfer_size], &buffer_[k], k_size);
      n_out_ += k_size;
      data_size -= k_size;
      xfer_size += k_size;
      cv_in_.notify_one();
    }
    return xfer_size;
  }

  size_t rad::yocto::YoChannel::write(const void* const data, std::size_t data_size)
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
      const unsigned char * const cdata = reinterpret_cast<const unsigned char * const>(data);
      std::memcpy(&buffer_[k], &cdata[xfer_size], k_size);
      n_in_ += k_size;
      data_size -= k_size;
      xfer_size += k_size;
      cv_out_.notify_one();
    }
    return xfer_size;
  }

#endif

/* *EOF* */
