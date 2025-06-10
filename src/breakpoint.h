#pragma once

#include <cstdint>
#include <sys/types.h>

class Breakpoint {
public:
    Breakpoint() = default;
    Breakpoint(pid_t, std::intptr_t);

    void            enable();
    void            disable();

    constexpr bool  is_enabled() const;
    std::intptr_t   getaddr() const;


private:
    pid_t           pid_;
    std::intptr_t   addr_;
    bool            enabled_;
    uint8_t         data_;
};