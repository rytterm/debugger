#include <iostream>
#include <unistd.h>
#include "debugger.h"
#include <sys/ptrace.h>
#include <iomanip>


int main(int argc, char** argv) {
    if (argc < 2)  {
        std::cerr << "Program not specified." << std::endl;
        return -1;
    }

    char* process {argv[1]};

    pid_t pid {fork()};

    if (pid == 0) {

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

}