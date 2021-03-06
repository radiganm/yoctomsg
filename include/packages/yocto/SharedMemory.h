/* SharedMemory.h
 * Copyright 2017 Mac Radigan
 * All Rights Reserved
 */

#include <string>
#include <exception>
#include <iostream>

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

namespace rad::yocto {

  class SharedMemory
  {
    public:
      SharedMemory(const std::string &name, std::size_t size, bool owner=true);
      virtual ~SharedMemory();
      inline void *get_data() { return data_; };
      void release();
    protected:
    private:
      int fd_;
      std::size_t size_;
      bool owner_;
      void *data_;
      std::string name_;
      //friend std::ostream& operator<<(std::ostream &os, const SharedMemory& o);
  };

} // namespace

//std::ostream& operator<<(std::ostream &os, const rad::yocto::SharedMemory& o)
//{
//  return os;
//}

// *EOF*
