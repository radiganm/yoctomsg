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

    constexpr size_t N = 5;
    rad::yocto::Channel<rad::yocto::slot_t,N> &channel = *new rad::yocto::BasicChannel<rad::yocto::slot_t,N>();

    std::thread write_th([&](void) -> void {
      char counter = 'a';
      rad::yocto::slot_t data; 
      while(bool forever = true)
      {
        memset(&data, counter, sizeof(data));
        counter = ('z' == counter) ? 'a' : ++counter;
        auto size = channel.write(&data, 1);
        std::cerr << "sent:" << size << " block(s), with " << data << std::endl << std::flush;
        std::this_thread::sleep_for(std::chrono::seconds(1));
      } // send loop
    });

    std::thread read_th([&](void) -> void {
      rad::yocto::slot_t data; 
      while(bool forever = true)
      {
        auto size = channel.read(&data, 1);
        std::cerr << "received:" << size << " block(s)" << std::endl << std::flush;
        std::this_thread::sleep_for(std::chrono::seconds(1));
      } // receive loop
    });

    std::thread summary_th([&](void) -> void {
      while(bool forever = true)
      {
        //channel.summarize(std::cout);
        std::cerr << channel;
        std::this_thread::sleep_for(std::chrono::seconds(1));
      } // summary loop
    });

    read_th.join();
    write_th.join();
    summary_th.join();

    return status;
  }


/* *EOF* */
