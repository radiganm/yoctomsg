/* channel.h
 */

#include "../common/status.h"
#include <stdlib.h>
#include <inttypes.h>
#include <pthread.h>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>

#define CHUNK_SIZE (1024)

#ifndef yo_channel_h
#define yo_channel_h

  typedef struct slot_s { complex<real_4> data[CHUNK_SIZE]; } slot_t;

  template<typename T, std::size_t N>


  template<typename T, std::size_t N>

  size_t Channel<T,N>::read(T *data, size_t size)
  {
    std::unique_lock<std::mutex> lck(read_mutex_);
    for(size_t k=0; k<size; ++k) {
      read_cv_.wait(lck, [&]{return read_count_;});
      data[k] = buffer_[read_index_];
      read_index_ =  (read_index_ + 1) % N;
      --read_count_;
      write_cv_.notify_one();
    }
  }

  template<typename T, std::size_t N>
  size_t Channel<T,N>::write(const T* const data, size_t size)
  {
    std::unique_lock<std::mutex> lck(write_mutex_);
    for(size_t k=0; k<size; ++k) {
      write_cv_.wait(lck, [&]{return write _count_;});
      buffer_[write_index_] = data[k];
      write_index_ =  (write_index_ + 1) % N;
      --write_count_;
      read_cv_.notify_one();
    }
  }

/*
  Channel<slot_t, N> channel;
  auto write_fn = [&](const slot_t * const data, size_t size) -> bool {
    channel.write(data, size);
  };

  auto n_read = channel.read(hout, chunk_size);

  method(std::function<bool (const void* const buf, size_t size)> fn) { fn_ = fn; };
*/

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif

/* *EOF* */
