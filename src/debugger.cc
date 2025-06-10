#include "debugger.h"
#include <sys/types.h>
#include <sys/wait.h>
#include "lib/cli.h"
#include "lib/utils.h"
#include <iostream>
#include <sys/ptrace.h>


Debugger::Debugger(std::string process, pid_t pid)
        : process_ {std::move(process)}, pid_ {pid}
{}

void Debugger::run() {

    int wstatus;
    int options {};

    waitpid(pid_, &wstatus, options);

    std::string line {};

    while ((line = cliInput("mdbg> ")) != "q") {
        if (line != "ignore") {
            handlecmd_(line);
            cliAddHistory(line);
            displayHistory();
        }
    }

}


void Debugger::handlecmd_(const std::string& line) {
    auto args = split(line,' ');
    auto command = args[0];


    if (is_prefix(command, "c") || is_prefix(command, "cont") || is_prefix(command, "continue"))
        cexec_();
    else if (is_prefix(command, "b") || is_prefix(command, "br") || is_prefix(command, "break")) {
        std::string addr {args[1], 2};
        setbp_(std::stol(addr, 0, 16));
    }
    else 
        std::cerr << "Unknown command\n";
}



void Debugger::cexec_() const {
    ptrace(PTRACE_CONT, pid_, nullptr, nullptr);

    int wstatus;
    int options {};

    waitpid(pid_, &wstatus, options);
}



void Debugger::setbp_(std::intptr_t addr) {
    std::cout << "Set breakpoint at 0x" << std::hex << addr << std::endl;
    Breakpoint bp {pid_, addr};
    bp.enable();
    breakpoints_[addr] = bp;
}