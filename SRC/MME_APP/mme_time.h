#ifndef FILE_MME_TIME_SEEN
#define FILE_MME_TIME_SEEN
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
extern struct timeval g_timer[];
extern int g_timer_idx;

#define INIT_TIME { g_timer_idx = 0;}
#define PRINT_TIME {\
  gettimeofday(&g_timer[g_timer_idx], NULL);\
  printf("\n************************************************************************************************************\n");\
  printf("\t\tTimer[%d] is at %ld sec + %ld usec\n", g_timer_idx, g_timer[g_timer_idx].tv_sec, g_timer[g_timer_idx].tv_usec);\
  printf("\n************************************************************************************************************\n");\
  g_timer_idx++;\
}

#define SHOW_TIME {\
  printf("\n*************************************************TIMER RECORD***************************************************\n");\
  for (int i = 1; i<g_timer_idx; i++){\
    struct timeval tv1 = g_timer[0];\
    struct timeval tv2 = g_timer[i];\
    double sec = tv2.tv_sec - tv1.tv_sec;\
    double usec = tv2.tv_usec - tv1.tv_usec;\
    double ms = sec * 1000 + usec / 1000;\
    printf("%.3lf         ", ms);\
  }\
  printf("\n************************************************************************************************************\n");\
}
#endif


