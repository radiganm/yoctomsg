/* basic_channel.cc
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
#include <chrono>
#include <string.h>

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

    constexpr size_t N = 1024;
    rad::yocto::Channel<rad::yocto::slot_t,N> &channel = *new rad::yocto::BasicChannel<rad::yocto::slot_t,N>();

    auto write_fn = [&](void) -> void {
      rad::yocto::slot_t data; 
      memset(&data, 'x', sizeof(data));
      while(bool forever = true)
      {
        auto size = channel.write(&data, 1);
        std::cerr << "sent:" << size << " block(s)" << std::endl << std::flush;
        std::this_thread::sleep_for(std::chrono::seconds(1));
      } // send loop
    };
    std::thread write_th(write_fn);


    auto read_fn = [&](void) -> void {
      rad::yocto::slot_t data; 
      while(bool forever = true)
      {
        auto size = channel.read(&data, 1);
        std::cerr << "received:" << size << " block(s)" << std::endl << std::flush;
        std::this_thread::sleep_for(std::chrono::seconds(1));
      } // receive loop
    };
    std::thread read_th(read_fn);

    read_th.join();
    write_th.join();

//  std::istream_iterator<std::string> eos;
//  std::istream_iterator<std::string> is = std::istream_iterator<std::string>(std::cin);
//  std::ostream_iterator<std::string> os(std::cout," :\n: ");
//  std::copy(is, eos, os);

    return status;
  }


/* *EOF* */
