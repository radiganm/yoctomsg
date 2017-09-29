/* Channel.h
 */

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

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

  class Channel
  {
    public:
      Channel() : read_index_(0),
                  write_index_(0),
                  read_count_(0),
                  write_count_(0) {};
      virtual ~Channel() {};
      inline size_t read(T *buf, size_t size);
      inline size_t read(CPHEADER &hout, size_t size);
      inline size_t write(const T* const buf, size_t size);
    private:
      size_t read_index_;
      size_t write_index_;
      size_t read_count_;
      size_t write_count_;
      std::mutex read_mutex_;
      std::mutex write_mutex_;
      std::condition_variable read_cv_;
      std::condition_variable write_cv_;
      array<T, N> buffer_;
  };


#endif

/* *EOF* */
