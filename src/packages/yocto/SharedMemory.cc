/* SharedMemory.cc
 * Copyright 2017 Mac Radigan
 * All Rights Reserved
 */

#include "packages/yocto/SharedMemory.h"

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

  using namespace rad::yocto;

  SharedMemory::SharedMemory(const std::string &name, std::size_t size, bool owner)
  {
    name_  = name;
    size_  = size;
    owner_ = owner;
    mode_t mode = S_IRUSR | S_IWUSR;
    int oflag = owner ? O_RDWR | O_CREAT : O_RDWR;
    fd_ = shm_open(name.c_str(), oflag, mode);
    if(fd_ < 1) throw new std::bad_alloc;
    if(owner) if(-1 == ftruncate(fd_, size_)) throw new std::bad_alloc;
    data_ = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_, 0);
    if(MAP_FAILED == data_) throw new std::bad_alloc;
  }

  SharedMemory::~SharedMemory()
  {
    close(fd_);
    if(owner_ && (-1 == shm_unlink(name_.c_str()))) perror("could not unlink shared memory");
  }

/* *EOF* */
