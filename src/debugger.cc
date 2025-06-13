#include "debugger.h"
#include <sys/types.h>
#include <sys/wait.h>
#include "../include/cli.h"
#include "../include/utils.h"
#include <iostream>
#include <sys/ptrace.h>
#include "../include/reg.h"
#include <iomanip>
#include <sys/personality.h>
#include <fcntl.h>

Debugger::Debugger(char *process, pid_t pid)
    : process_{std::move(process)}, pid_{pid}
{}

void Debugger::run()
{

  waitsig_();

  std::string line{};

  while ((line = cliInput("mdbg> ")) != "q")
  {
    if (line != "ignore")
    {
      handlecmd_(line);
      cliAddHistory(line);
      // displayHistory();
    }
  }
  // kill(pid_, SIGKILL);
}

void Debugger::handlecmd_(const std::string &line)
{
  std::vector<std::string> args = split(line, ' ');
  std::string command = args[0];

  if (is_prefix(command, "r") || is_prefix(command, "run"))
    launch_target_();
  else if (is_prefix(command, "c") || is_prefix(command, "cont") || is_prefix(command, "continue"))
    cexec_();
  else if (is_prefix(command, "b") || is_prefix(command, "br") || is_prefix(command, "break"))
  {
    std::string addr{args[1], 2};
    setbp_(std::stol(addr, 0, 16));
  }
  else if (is_prefix(command, "reg") || is_prefix(command, "register"))
  {
    if (is_prefix(args[1], "dump"))
      regdump_();
    else if (is_prefix(args[1], "read"))
      std::cout << get_reg_value(pid_, name_get_reg(args[2])) << std::endl;
    else if (is_prefix(args[1], "write"))
    {
      std::string val{args[3], 2};
      set_reg_value(pid_, name_get_reg(args[2]), std::stol(val, 0, 16));
    }
  }
  else if (is_prefix(command, "mem") || is_prefix(command, "memory"))
  {
    std::string addr{args[2], 2};

    if (is_prefix(args[1], "read"))
    {
      std::cout << std::hex << readmem_(std::stol(addr, 0, 16)) << std::endl;
    }
    if (is_prefix(args[1], "write"))
    {
      std::string val{args[3], 2};
      writemem_(std::stol(addr, 0, 16), std::stol(val, 0, 16));
    }
  }
  else
    std::cerr << "Unknown command\n";
}

void Debugger::cexec_()
{
  ptrace(PTRACE_CONT, pid_, nullptr, nullptr);
  waitsig_();
}

void Debugger::setbp_(std::intptr_t addr)
{
  std::cout << "Set breakpoint at 0x" << std::hex << addr << std::endl;
  Breakpoint bp{pid_, addr};
  if (pid_ > 0)
    bp.enable();
  breakpoints_[addr] = bp;
}

void Debugger::regdump_() const
{
  for (const reg_desc &rd : registers)
  {
    std::cout << rd.name << "0x" << std::setfill('0') << std::setw(16) << std::hex << get_reg_value(pid_, rd.r) << std::endl;
  }
}

uint64_t Debugger::readmem_(uint64_t addr) const
{
  return ptrace(PTRACE_PEEKDATA, pid_, addr, nullptr);
}

void Debugger::writemem_(uint64_t addr, uint64_t value) const
{
  ptrace(PTRACE_POKEDATA, pid_, addr, value);
}

uint64_t Debugger::getpc_() const
{
  return get_reg_value(pid_, Reg::rip);
}

void Debugger::setpc_(uint64_t pc) const
{
  set_reg_value(pid_, Reg::rip, pc);
}

void Debugger::skipbp_()
{
  uint64_t possible_brp{getpc_() - 1};

  if (breakpoints_.count(possible_brp))
  {
    auto &bp{breakpoints_.at(possible_brp)};

    if (bp.is_enabled())
    {
      uint64_t prev_instr{possible_brp};
      setpc_(prev_instr);
      bp.disable();
      ptrace(PTRACE_SINGLESTEP, pid_, nullptr, nullptr);
      waitsig_();
      bp.enable();
    }
  }
}

void Debugger::waitsig_()
{
  int wstatus;
  int options{};
  waitpid(pid_, &wstatus, options);
  skipbp_();
}

void Debugger::launch_target_()
{
  pid_ = fork();

  if (pid_ == 0)
  {
    personality(ADDR_NO_RANDOMIZE);
    ptrace(PTRACE_TRACEME, 0, nullptr, nullptr);
    execl(process_, process_, nullptr);
  }
  else
  {
    waitsig_();
    std::cout << "Debugger entered:" << std::endl
              << "pid: " << pid_ << " | Name: " << process_ << std::endl;

    for (auto &[addr, bp] : breakpoints_)
    {
      bp.setpid(pid_); // if Breakpoint needs an updated PID
      bp.enable();     // actually writes 0xCC
    }
    cexec_();
  }
}