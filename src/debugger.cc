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

void Debugger::run() const {

    int wstatus;
    int options {};

    waitpid(pid_, &wstatus, options);

    std::string line {};

    while ((line = cliInput("\nmdbg> ")) != "") {
        handlecmd_(line);
        cliAddHistory(line);
    }

}


void Debugger::handlecmd_(const std::string& line) const {
    auto args = split(line,' ');
    auto command = args[0];


    if (is_prefix(command, "c") || is_prefix(command, "cont") || is_prefix(command, "continue"))
        contexec();
    else 
        std::cerr << "Unknown command\n";
}



void Debugger::contexec() const {
    ptrace(PTRACE_CONT, pid_, nullptr, nullptr);

    int wstatus;
    int options {};

    waitpid(pid_, &wstatus, options);
}