#ifdef __linux__
#include "debugger.h"
#include <sys/ptrace.h>
#include <sys/personality.h>
#endif

#include <iostream>
#include <unistd.h>
#include <iomanip>




int main(int argc, char** argv) {

#ifndef __linux__
    std::cerr << "Must be on linux" << std::endl;
    return -1;
#endif
#ifndef __x86_64__
    std::cerr << "Must be on 64 bit architecture" << std::endl;
    return -1;
#endif
    
    if (argc < 2)  {
        std::cerr << "Program not specified." << std::endl;
        return -1;
    }

    char* process {argv[1]};


    //std::cout << process;
    //pid_t pid {fork()};

    Debugger dbg {process, -1};
    dbg.run();
/*
    if (pid == 0) {
        personality(ADDR_NO_RANDOMIZE);
        ptrace(PTRACE_TRACEME, 0, nullptr, nullptr);
        execl(process, process, nullptr);
    } else if (pid >= 1) {

        std::cout << "Debugger entered:" << std::endl 
        <<  "pid: " << pid << " | Name: " << process << std::endl;
        
        Debugger dbg {process, pid};

        dbg.run();

    } else {
        std::cerr << "Invalid child process id:" << pid << std::endl;
        return -1;
    }
*/
}