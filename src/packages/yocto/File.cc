/* File.cc
 * Copyright 2017 Mac Radigan
 * All Rights Reserved
 */

#include "packages/yocto/File.h"

  using namespace rad::yocto;

  File::File(const std::string &name, std::size_t size)
   : header_shm_(name+"_hdr", sizeof(grey_file_t), size>0)
  {
    owner_ = size > 0;
    channel_ = new YoChannel(name+"_dat", size, owner_);
    header_ = reinterpret_cast<grey_file_t *>(header_shm_.get_data());
    if(owner_)
    {
      memset(header_, 0, sizeof(grey_file_t));
      grey_header_t &hdr = header_->header;
      strncpy(hdr.version,  GREY__VERSION,           4);
      strncpy(hdr.head_rep, GREY__REP_LITTLE_ENDIAN, 4);
      strncpy(hdr.data_rep, GREY__REP_LITTLE_ENDIAN, 4);
      hdr.locked = GREY__FALSE;
      hdr.piped  = GREY__TRUE;
    }
  }

  File::~File()
  {
    delete channel_;
  }

/* *EOF* */
