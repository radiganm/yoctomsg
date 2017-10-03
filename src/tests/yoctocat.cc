/* basic_channel.cc
 * Copyright 2016 Mac Radigan
 * All Rights Reserved
 */

#include "packages/yocto/ShmemChannel.h"
#include "packages/yocto/BasicChannel.h"

#include <gflags/gflags.h>

#include <iostream>
#include <iterator>
#include <thread>
#include <chrono>
#include <string.h>

  DEFINE_bool(debug,   false, "debug");
  DEFINE_bool(verbose, false, "verbose");

  int main(int argc, char *argv[])
  {
    int status = EXIT_SUCCESS;

    std::string usage("yoctocat - Sample usage:\n");
    usage += argv[0];
    usage += " ";
    gflags::SetUsageMessage(usage);
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    try
    {
    }
    catch(std::bad_alloc &ex)
    {
    }

    return status;
  }


/* *EOF* */
