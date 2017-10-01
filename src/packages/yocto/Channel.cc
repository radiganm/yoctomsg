/* Channel.cc
 * Copyright 2017 Mac Radigan
 * All Rights Reserved
 */

#include "packages/yocto/Channel.h"

  std::ostream& operator<<(std::ostream &os, const rad::yocto::slot_t &o)
  {
    std::ios_base::fmtflags fo = os.flags();
    std::ios_base::fmtflags ff = os.flags();
    ff |= os.hex;
    os.setf(ff);
  //os << reinterpret_cast<const char *>(&o) << std::endl << std::flush; 
    os << reinterpret_cast<const char *>(&o)[0] << std::endl << std::flush; 
    os.setf(fo);
    return os;
  }

/* *EOF* */
