#ifndef _LIB_DEBUG_H
#define _LIB_DEBUG_H


#ifdef __cplusplus
extern "C" {
#endif



#define NO_RETURN __attribute__ ((noreturn))
#define PRINTF_FORMAT(FMT, FIRST) __attribute__ ((format (printf, FMT, FIRST)))


#define PANIC(...) debug_panic(__FILE__, __LINE__, __func__, __VA_ARGS__)



void debug_panic (const char* file, int line, const char* func,
                  const char* msg, ...) PRINTF_FORMAT(4, 5) NO_RETURN;








#ifdef __cplusplus
}
#endif



#endif