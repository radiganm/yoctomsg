/* shmem_channel.cc
 * Copyright 2016 Mac Radigan
 * All Rights Reserved
 */

#include "packages/yocto/ShmemChannel.h"

#include <gflags/gflags.h>

#include <iostream>
#include <iterator>
#include <thread>
#include <chrono>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <cstring>

  static std::atomic<bool> is_running(true);

  static constexpr size_t N = 5;
  static rad::yocto::Channel<rad::yocto::slot_t,N> &channel = *new rad::yocto::ShmemChannel<rad::yocto::slot_t,N>("TEST__shmem_channel", true);

  static void interrupt(int signo)
  {
    switch(signo)
    {
      case SIGINT:
        is_running = false;
        channel.release();
        break;
      default:
        std::cerr << "unhandled signal" << std::endl << std::flush;
    }
  }
  
  DEFINE_bool(debug,   false, "debug");
  DEFINE_bool(verbose, false, "verbose");

  int main(int argc, char *argv[])
  {
    int status = EXIT_SUCCESS;

    if(SIG_ERR == signal(SIGINT, interrupt)) {
      perror("unable to set signal");
      return status;
    }

    std::string usage("Test driver for ShmemChannel.  Sample usage:\n");
    usage += argv[0];
    usage += " ";
    gflags::SetUsageMessage(usage);
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    std::thread write_th([&](void) -> void {
      char counter = 'a';
      rad::yocto::slot_t data; 
      while(is_running)
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
      while(is_running)
      {
        auto size = channel.read(&data, 1);
        std::cerr << "received:" << size << " block(s)" << std::endl << std::flush;
        std::this_thread::sleep_for(std::chrono::seconds(1));
      } // receive loop
    });

    std::thread summary_th([&](void) -> void {
      while(is_running)
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
