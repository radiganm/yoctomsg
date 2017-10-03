/* SharedMemory.h
 * Copyright 2017 Mac Radigan
 * All Rights Reserved
 */

#include <string>
#include <exception>

#ifndef yo_SharedMemory_h
#define yo_SharedMemory_h

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
      SharedMemory(const std::string &name);
      SharedMemory(const std::string &name, size_t size);
      virtual ~SharedMemory();
    protected:
    private:
      int fd_;
      size_t size_;
      bool owner_;
      void *data_;
      std::string name_;
      friend std::ostream& operator<<(std::ostream &os, rad::yocto::SharedMemory& o);
  };

} // namespace

  std::ostream& operator<<(std::ostream &os, const rad::yocto::SharedMemory& o)
  {
    return os;
  }

#endif

/* *EOF* */
