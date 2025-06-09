#pragma once

#include <string>


class Debugger {
public:
    Debugger(std::string process, pid_t pid);

    void run() const;

private:
    std::string process_;
    pid_t       pid_;

    void handlecmd_(const std::string&) const;
    void contexec() const;
};