/* AuxFile.h
 * Copyright 2017 Mac Radigan
 * All Rights Reserved
 */

#include "packages/yocto/Stream.h"

#include <sstream>
#include <cstring>
#include <iostream>

#ifndef yo_AuxFile_h
#define yo_AuxFile_h

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

namespace rad::yocto {

  class AuxFile
  {
    public:
      AuxFile(const std::string &name, std::size_t size=0);
      virtual ~AuxFile();
      template<typename T>
      inline std::size_t read(T *data, std::size_t size);
      template<typename T>
      inline std::size_t write(const T* const data, std::size_t size);
    private:
      Stream *stream_;
      bool    is_pipe_;
  };

} // namespace

  template<typename T>
  inline std::size_t rad::yocto::AuxFile::read(T *data, std::size_t size)
  {
    stream_->write(data, size);
  }

  template<typename T>
  inline std::size_t rad::yocto::AuxFile::write(const T* const data, std::size_t size)
  {
    stream_->read(data, size);
  }

#endif

/* *EOF* */
