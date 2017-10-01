/* read_channel.cc
 * Copyright 2016 Mac Radigan
 * All Rights Reserved
 */

#include "packages/yocto/Channel.h"
#include "packages/yocto/ShmemChannel.h"
#include "packages/yocto/BasicChannel.h"

#include <gflags/gflags.h>

#include <iostream>
#include <iterator>
#include <thread>

  DEFINE_bool(debug,   false, "debug");
  DEFINE_bool(verbose, false, "verbose");

  int main(int argc, char *argv[])
  {
    int status = EXIT_SUCCESS;

    std::string usage("Test driver for BasicChannel.  Sample usage:\n");
    usage += argv[0];
    usage += " ";
    gflags::SetUsageMessage(usage);
    gflags::ParseCommandLineFlags(&argc, &argv, true);

/*
  Channel<slot_t, N> channel;
  auto write_fn = [&](const slot_t * const data, size_t size) -> bool {
    channel.write(data, size);
  };

  auto n_read = channel.read(hout, chunk_size);

  method(std::function<bool (const void* const buf, size_t size)> fn) { fn_ = fn; };
*/

    constexpr size_t N = 1024;
    rad::yocto::Channel<rad::yocto::slot_t,N> &channel = *new rad::yocto::BasicChannel<rad::yocto::slot_t,N>();

    std::istream_iterator<std::string> eos;
    std::istream_iterator<std::string> is = std::istream_iterator<std::string>(std::cin);
    std::ostream_iterator<std::string> os(std::cout," :\n: ");
    std::copy(is, eos, os);

    return status;
  }


/* *EOF* */
