#include "backtrace.h"
#include <inttypes.h>  

#define _Assert_Exit_                           \
{                                               \
  fprintf(stderr, "\nExiting execution\n");   \
  display_backtrace();                        \
  fflush(stdout);                             \
  fflush(stderr);                             \
  exit(EXIT_FAILURE);                         \
}

#define _Assert_(cOND, aCTION, fORMAT, aRGS...)             \
do {                                                        \
    if (!(cOND)) {                                          \
        fprintf(stderr, "\nAssertion ("#cOND") failed!\n"   \
                "In %s() %s:%d\n" fORMAT,                   \
                __FUNCTION__, __FILE__, __LINE__, ##aRGS);  \
        aCTION;                                             \
    }                                                       \
} while(0)

#if DEBUG_IS_ON
#define _ASSERT_FINAL_ _Assert_SegFault_
#else
#define _ASSERT_FINAL_ _Assert_Exit_
#endif

#define DevCheck(cOND, vALUE1, vALUE2, vALUE3)                                                          \
                        _Assert_(cOND, _ASSERT_FINAL_, #vALUE1 ": %" PRIdMAX "\n" #vALUE2 ": %" PRIdMAX "\n" #vALUE3 ": %" PRIdMAX "\n\n",  \
                        (intmax_t)vALUE1, (intmax_t)vALUE2, (intmax_t)vALUE3)


#define DevAssert(cOND)                     _Assert_(cOND, _ASSERT_FINAL_, "")
#define DevMessage(mESSAGE)                 _Assert_(0, _ASSERT_FINAL_, #mESSAGE)

