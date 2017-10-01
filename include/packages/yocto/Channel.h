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
      virtual size_t read(T *buf, size_t size);
      virtual size_t write(const T* const buf, size_t size);
      int underflow() 
      {
        if (this->gptr() == this->egptr()) {
          size_t size = read(&slot_, 1);
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
      //std::streambuf* sbuf_;
      //char* buffer_;
      //friend std::ostream & operator<<(std::ostream &os, const Channel& o);
      //friend std::istream & operator>>(std::ostream &os, const Channel& o);
  };

} // namespace

#endif

//std::istream & operator>>(std::istream &is, const Channel& o)
//{
//  o.read(
//  return os;
//}
//
//std::ostream & operator<<(std::ostream &os, const Channel& o)
//{
//  o.write(
//  return os;
//}

/* *EOF* */
