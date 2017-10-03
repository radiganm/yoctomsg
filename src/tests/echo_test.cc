/* echo_test.cc
 * Copyright 2016 Mac Radigan
 * All Rights Reserved
 */

#include <gflags/gflags.h>

#include <iostream>
#include <iterator>
#include <string.h>

  DEFINE_bool(debug,   false, "debug");
  DEFINE_bool(verbose, false, "verbose");

  int main(int argc, char *argv[])
  {
    int status = EXIT_SUCCESS;

    std::string usage("Echo Test.  Sample usage:\n");
    usage += argv[0];
    usage += " ";
    gflags::SetUsageMessage(usage);
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    std::istream_iterator<std::string> eos;
    std::istream_iterator<std::string> is = std::istream_iterator<std::string>(std::cin);
    std::ostream_iterator<std::string> os(std::cout,"");
    std::copy(is, eos, os);

    return status;
  }


/* *EOF* */
