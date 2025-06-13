#if defined(__linux__) && defined(__x86_64__)
#else
#error "Must be on linux x86_64"
#endif


#include "../include/debugger.hh"
#include <sys/ptrace.h>
#include <sys/personality.h>
#include <iostream>
#include <unistd.h>
#include <iomanip>
#include "../include/dwelf/elf.h"
#include "../lib/debug.h"


int main(int argc, char **argv)
{

  read_header(argv[1]);

  if (argc < 2)
  {
    std::cerr << "Program not specified." << std::endl;
    return -1;
  }

  char *process{argv[1]};

  Debugger dbg{process, -1};
  dbg.run();

}