/* Channel.cc
 */

#include "packages/yocto/Channel.h"

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
      write_cv_.wait(lck, [&]{return write_count_;});
      buffer_[write_index_] = data[k];
      write_index_ =  (write_index_ + 1) % N;
      --write_count_;
      read_cv_.notify_one();
    }
  }

/* *EOF* */
