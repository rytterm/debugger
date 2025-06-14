#include "../include/breakpoint.hh"
#include <sys/ptrace.h>

Breakpoint::Breakpoint(pid_t pid, std::intptr_t addr)
    : pid_{pid}, addr_{addr}, enabled_{false}, data_{}
{
}

void Breakpoint::enable()
{
  long data{ptrace(PTRACE_PEEKDATA, pid_, addr_, nullptr)};
  data_ = static_cast<uint8_t>(data & 0xff);
  uint64_t i3{0xcc};
  uint64_t i3data{((data & ~0xff) | i3)};
  ptrace(PTRACE_POKEDATA, pid_, addr_, i3data);
  enabled_ = true;
}

void Breakpoint::disable()
{
  long data{ptrace(PTRACE_PEEKDATA, pid_, addr_, nullptr)};
  long resdata{((data & ~0xff) | data_)};
  ptrace(PTRACE_POKEDATA, pid_, addr_, resdata);
  enabled_ = false;
}

bool Breakpoint::is_enabled() const
{
  return enabled_;
}

std::intptr_t Breakpoint::getaddr() const
{
  return addr_;
}

void Breakpoint::setpid(pid_t pid)
{
  pid_ = pid;
}