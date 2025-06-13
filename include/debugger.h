#pragma once

#include <string>
#include "breakpoint.h"
#include <unordered_map>

class Debugger
{
public:
  Debugger(char *, pid_t);

  void run();

private:
  char *process_;
  pid_t pid_;


  std::unordered_map<std::intptr_t, Breakpoint> breakpoints_;

  void regdump_() const;

  void handlecmd_(const std::string &);

  // Continue execution
  void cexec_();

  // Set breakpoint at address addr
  void setbp_(std::intptr_t);

  uint64_t readmem_(uint64_t) const;
  void writemem_(uint64_t, uint64_t) const;

  uint64_t getpc_() const;
  void setpc_(uint64_t) const;

  void skipbp_();

  void waitsig_();

  void launch_target_();
};
