#ifndef __INCLUDE_DEBUG_H
#define __INCLUDE_DEBUG_H


#ifdef __cplusplus
extern "C" {
#endif



#define NO_RETURN __attribute__ ((noreturn))
#define PRINTF_FORMAT(FMT, FIRST) __attribute__ ((format (printf, FMT, FIRST)))


#define PANIC(...) debug_panic(__FILE__, __LINE__, __func__, __VA_ARGS__)



void debug_panic (const char*, int, const char*, const char*, ...) 
                  PRINTF_FORMAT(4, 5) NO_RETURN;





#ifdef NDEBUG
#define ASSERT(COND) (void(0))
#else
#define ASSERT(COND) \
  do { \
    if (!(COND)) PANIC("Assertion %s failed", #COND); \
  } while (0)
#endif




#ifdef __cplusplus
}
#endif



#endif