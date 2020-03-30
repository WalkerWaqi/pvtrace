/********************************************************************
 * File: instrument.c
 *
 * Instrumentation source -- link this with your application, and
 *  then execute to build trace data file (trace.txt).
 *
 * Author: M. Tim Jones <mtj@mtjones.com>
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define CODE_PATH         "ttt"
#define CODE_PERMISSIONS  "r-xp"
#define TRACE_FILE        "trace.txt"

/* Function prototypes with attributes */
void main_constructor(void)
    __attribute__((no_instrument_function, constructor));

void main_destructor(void)
    __attribute__((no_instrument_function, destructor));

void __cyg_profile_func_enter(void *, void *)
    __attribute__((no_instrument_function));

void __cyg_profile_func_exit(void *, void *)
    __attribute__((no_instrument_function));

static FILE *fp;
static void *segment_start = 0;

#define N 255
void __attribute__((no_instrument_function))
get_segment_start()
{
  FILE *fp;
  char str[N + 1];

  if ((fp = fopen("/proc/self/maps", "rt")) == NULL)
    exit(-1);

  while (fgets(str, N, fp) != NULL)
  {
    // printf("%s", str);

    if (strstr(str, CODE_PERMISSIONS) && strstr(str, CODE_PATH))
    {
      char *end = strstr(str, "-");
      if (end)
      {
        *end = '\0';

        sscanf(str, "%llx", &segment_start);

        // printf("%p", segment_start);

        break;
      }
    }
  }

  fclose(fp);
}

void main_constructor(void)
{
  fp = fopen(TRACE_FILE, "w");
  if (fp == NULL)
    exit(-1);

  get_segment_start();
}

void main_deconstructor(void)
{
  fclose(fp);
}

void __cyg_profile_func_enter(void *this, void *callsite)
{
  if (fp)
  {
    // fprintf(fp, "E%p\n", (int *)(this - segment_start));
    int thread_id = pthread_self();
    fprintf(fp, "%u Enter\n%u %p\n%u %p\n", thread_id, thread_id, (int *)(callsite - segment_start), thread_id, (int *)(this - segment_start));
  }
}

void __cyg_profile_func_exit(void *this, void *callsite)
{
  if (fp)
  {
    // fprintf(fp, "X%p\n", (int *)(this - segment_start));
    int thread_id = pthread_self();
    fprintf(fp, "%u Exit\n%u %p\n%u %p\n", thread_id, thread_id, (int *)(callsite - segment_start), thread_id, (int *)(this - segment_start));
  }
}
