/* Channel.h
 * Copyright 2017 Mac Radigan
 * All Rights Reserved
 */

#include <stdlib.h>
#include <inttypes.h>
#include <complex>

#define CHUNK_SIZE (1024)

#ifndef yo_Channel_h
#define yo_Channel_h

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

namespace rad::yocto {
  typedef struct slot_s { std::complex<double> data[CHUNK_SIZE]; } slot_t;
} // namespace
  std::ostream& operator<<(std::ostream &os, const rad::yocto::slot_t &o);

namespace rad::yocto {

  template<typename T, std::size_t N>
  class Channel : public std::streambuf
  {
    public:
      Channel() {};
      virtual ~Channel() { buffer_ = reinterpret_cast<char *>(&slot_); };
      virtual std::size_t read(T *buf, std::size_t size);
      virtual std::size_t write(const T* const buf, std::size_t size);
      virtual void summarize(std::ostream &os);
      virtual void release();
      int underflow() 
      {
        if (this->gptr() == this->egptr()) {
          std::size_t size = read(&slot_, 1);
          this->setg(this->buffer_, this->buffer_, this->buffer_ + size);
        }
        return this->gptr() == this->egptr()
          ? std::char_traits<char>::eof()
          : std::char_traits<char>::to_int_type(*this->gptr());
      }
      // overflow, sync (writing)
    protected:
      char *buffer_;
      T slot_;
    private:
      template<typename U, std::size_t M>
      friend std::ostream& operator<<(std::ostream &os, rad::yocto::Channel<T,N>& o);
  };

} // namespace

  template<typename T, std::size_t N>
  std::ostream& operator<<(std::ostream &os, rad::yocto::Channel<T,N>& o)
  {
    o.summarize(os);
    return os;
  }

  template<typename T, std::size_t N>
  void rad::yocto::Channel<T,N>::release()
  {
  }

#endif

/* *EOF* */
