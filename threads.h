#ifndef THREADS_THREADS_H
#define THREADS_THREADS_H

#include <pthread.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef char byte;
typedef unsigned int uint;
typedef enum { false, true } bool;
enum thread_state { ready, running, exited };
enum { MAX_THREADS = 128, STACK_SIZE = 32767, THREAD_TIMER = 50000 };
enum { JB_BX, JB_SI, JB_DI, JB_BP, JB_SP, JB_PC };

typedef struct {
  pthread_t tid;     /* thread id */
  jmp_buf registers; /* for saving registers */
  byte *stack;       /* private stack */
  enum thread_state state;
} TCB;

TCB threads[MAX_THREADS];

int pthread_create(pthread_t *restrict_thread,
                   const pthread_attr_t *restrict_attr,
                   void *(*start_routine)(void *), void *restrict_arg);
void pthread_exit(void *value_ptr);
pthread_t pthread_self(void);

#endif
