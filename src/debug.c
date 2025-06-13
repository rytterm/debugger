#include "debug.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void debug_panic (const char* file, int line, const char* func,
                  const char* msg, ...)
{
  va_list args;
  printf("Program exited at %s:%d in %s(): ", file, line, func);
  va_start(args, msg);
  vprintf(msg, args);
  printf("\n");
  va_end(args);

  exit(EXIT_FAILURE);
}