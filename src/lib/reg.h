#pragma once

#include <string>
#include <array>
#include <stdint.h>

enum class Reg
{
  rax,  rbx,  rcx,  rdx,
  rdi,  rsi,  rbp,  rsp,
  r8,   r9,   r10,  r11,
  r12,  r13,  r14,  r15,
  rip,  rflags,     cs,
  orig_rax,   fs_base,
  gs_base,    fs,   
  gs,   ss,   ds,   es
};

constexpr unsigned NUM_REGISTERS{27};

struct reg_desc
{
  Reg r;
  int dwarf;
  std::string name;
};

const std::array<reg_desc, NUM_REGISTERS> registers{{
    {Reg::r15, 15, "r15"},
    {Reg::r14, 14, "r14"},
    {Reg::r13, 13, "r13"},
    {Reg::r12, 12, "r12"},
    {Reg::rbp, 6, "rbp"},
    {Reg::rbx, 3, "rbx"},
    {Reg::r11, 11, "r11"},
    {Reg::r10, 10, "r10"},
    {Reg::r9, 9, "r9"},
    {Reg::r8, 8, "r8"},
    {Reg::rax, 0, "rax"},
    {Reg::rcx, 2, "rcx"},
    {Reg::rdx, 1, "rdx"},
    {Reg::rsi, 4, "rsi"},
    {Reg::rdi, 5, "rdi"},
    {Reg::orig_rax, -1, "orig_rax"},
    {Reg::rip, -1, "rip"},
    {Reg::cs, 51, "cs"},
    {Reg::rflags, 49, "eflags"},
    {Reg::rsp, 7, "rsp"},
    {Reg::ss, 52, "ss"},
    {Reg::fs_base, 58, "fs_base"},
    {Reg::gs_base, 59, "gs_base"},
    {Reg::ds, 53, "ds"},
    {Reg::es, 50, "es"},
    {Reg::fs, 54, "fs"},
    {Reg::gs, 55, "gs"},
}};

// Get register value
//
// This will only work for linux x86_64 as
// user_regs_struct has a standard layout
uint64_t get_reg_value(pid_t, Reg);

// Set register value
//
// This will only work for linux x86_64 as
// user_regs_struct has a standard layout
void set_reg_value(pid_t, Reg, uint64_t);

uint64_t dwarf_get_reg(pid_t, unsigned);

std::string get_reg_name(Reg);

Reg name_get_reg(const std::string &);