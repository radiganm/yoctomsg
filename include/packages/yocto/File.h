/* File.h
 * Copyright 2017 Mac Radigan
 * All Rights Reserved
 */

#include "packages/yocto/Stream.h"
#include "packages/yocto/YoChannel.h"
#include "packages/yocto/grey.h"

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
#include <cstring>
#include <iostream>

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

namespace rad::yocto {

  class File : public rad::yocto::Stream
  {
    public:
      File(const std::string &name, std::size_t size=0);
      virtual ~File();
      template<typename T>
      inline std::size_t read(T *data, std::size_t size);
      template<typename T>
      inline std::size_t write(const T* const data, std::size_t size);
    private:
      SharedMemory  header_shm_;
      grey_file_t  *header_;
      YoChannel    *channel_;
      bool          owner_;
  };

} // namespace

  template<typename T>
  inline std::size_t rad::yocto::File::read(T *data, std::size_t size)
  {
    channel_->write(data, size);
  }

  template<typename T>
  inline std::size_t rad::yocto::File::write(const T* const data, std::size_t size)
  {
    channel_->read(data, size);
  }

/* *EOF* */
