/* test_channel.cc
 * Copyright 2016 Mac Radigan
 * All Rights Reserved
 */

#include "packages/yocto/Channel.h"

#include <getopt.h>
#include <iostream>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include <errno.h>
#include <time.h>
#include <math.h>

  int usage(int argc, char *argv[])
  {
    std::cout << argv[0] << " - " << "test channel" << std::endl;
    std::cout << "    -d,--debug           debug" << std::endl;
    std::cout << "    -v,--verbose         verbose" << std::endl;
    std::cout << "    -h,--help            print this help message" << std::endl;
    exit(1);
  }

  int main(int argc, char *argv[])
  {
    int status = EXIT_SUCCESS;
    bool debug;
    bool verbose;
    while(true)
    {
      int this_option_optind = optind ? optind : 1;
      int option_index = 0;
      static struct option long_options[] = {
        {"debug", 0, 0, 0},
        {"verbose", 0, 0, 0},
        {"help", 0, 0, 0},
        {0, 0, 0, 0}
      };
      int c = getopt_long(argc, argv, "dhv", long_options, &option_index);
      if(-1 == c) break;
      switch(c)
      {
        case 'd':
          debug = true;
          break;
        case 'v':
          verbose = true;
          break;
        case 'h':
          usage(argc, argv);
          break;
      }
    }

/*
  Channel<slot_t, N> channel;
  auto write_fn = [&](const slot_t * const data, size_t size) -> bool {
    channel.write(data, size);
  };

  auto n_read = channel.read(hout, chunk_size);

  method(std::function<bool (const void* const buf, size_t size)> fn) { fn_ = fn; };
*/


    return status;
  }


/* *EOF* */
