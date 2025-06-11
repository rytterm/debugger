#include "reg.h"
#include <sys/user.h>
#include <sys/ptrace.h>
#include <algorithm>
#include <iterator>



uint64_t get_reg_value(pid_t pid, Reg r) {
    user_regs_struct regs;
    ptrace(PTRACE_GETREGS, pid, nullptr, &regs);

    auto it {std::find_if(registers.begin(), registers.end(),
                            [r](auto&& rd) { return rd.r == r; })};

    return *(reinterpret_cast<uint64_t*>(&regs) + (it - registers.begin()));
}


void set_reg_value(pid_t pid, Reg r, uint64_t value) {
    user_regs_struct regs;
    ptrace(PTRACE_GETREGS, pid, nullptr, &regs);

    auto it {std::find_if(registers.begin(), registers.end(),
                            [r](auto&& rd) { return rd.r == r; })};

    *(reinterpret_cast<uint64_t*>(&regs) + (it - registers.begin())) = value;
    ptrace(PTRACE_SETREGS, pid, nullptr, &regs);
}


uint64_t dwarf_get_reg(pid_t pid, unsigned regnum) {
    auto it = std::find_if(registers.begin(), registers.end(),
                           [regnum](auto&& rd) { return rd.dwarf_r == regnum; });
    if (it == registers.end()) {
        throw std::out_of_range{"Unknown dwarf register"};
    }

    return get_reg_value(pid, it->r);
}