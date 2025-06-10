#pragma once

#include <string>
#include "breakpoint.h"
#include <unordered_map>


class Debugger {
public:
    Debugger(std::string, pid_t);

    void run();

private:
    std::string process_;
    pid_t       pid_;
    
    std::unordered_map<std::intptr_t,Breakpoint> breakpoints_;

    void handlecmd_(const std::string&);
    void cexec_() const;
    void setbp_(std::intptr_t);

};


