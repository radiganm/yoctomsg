/* Stream.h
 * Copyright 2017 Mac Radigan
 * All Rights Reserved
 */

#include <stdlib.h>
#include <iostream>

#ifndef yo_Stream_h
#define yo_Stream_h

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

namespace rad::yocto {

  class Stream : public std::streambuf
  {
    public:
      Stream() {};
      virtual ~Stream() {};
      template<typename T>
      std::size_t read(T *buf, std::size_t size);
      template<typename T>
      std::size_t write(const T* const buf, std::size_t size);
      void release();
    protected:
    private:
  };

} // namespace

#endif

/* *EOF* */
