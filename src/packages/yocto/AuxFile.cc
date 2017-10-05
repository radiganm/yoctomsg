/* AuxFile.cc
 * Copyright 2017 Mac Radigan
 * All Rights Reserved
 */

#include "packages/yocto/AuxFile.h"

#include "packages/yocto/Pipe.h"
#include "packages/yocto/File.h"

  using namespace rad::yocto;

  AuxFile::AuxFile(const std::string &name, std::size_t size)
  {
    std::string prefix("_");
    is_pipe_ = !name.compare(0, prefix.size(), prefix);
    stream_ = is_pipe_
      ? static_cast<Stream *>(new Pipe(name, size))
      : static_cast<Stream *>(new File(name, size));
  }

  AuxFile::~AuxFile()
  {
    delete stream_;
  }

/* *EOF* */

