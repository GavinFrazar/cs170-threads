#include "threads.h"
#include <signal.h>
#include <stdio.h>

uint num_threads = 1;
uint alive_threads = 1;
uint running_thread = 0;
bool sigaction_set = false;

static int ptr_mangle(int p) {
  unsigned int ret;
  asm(" movl %1, %%eax;\n"
      " xorl %%gs:0x18, %%eax;"
      " roll $0x9, %%eax;"
      " movl %%eax, %0;"
      : "=r"(ret)
      : "r"(p)
      : "%eax");
  return ret;
}

static void AlarmHandler(int signum) {
  threads[running_thread].state = ready;
  int longjumped_here = setjmp(threads[running_thread].registers);
  if (!longjumped_here) {
    /* just saved state */
    do {
      running_thread = (running_thread + 1) % num_threads;
    } while (threads[running_thread].state != ready);
    ualarm(50000, 0);
    longjmp(threads[running_thread].registers, 1);
  } else {
    threads[running_thread].state = running;
    // set ONE alarm
    ualarm(50000, 0);
    return;
  }
}

int pthread_create(pthread_t *restrict_thread,
                   const pthread_attr_t *restrict_attr, /* attr always NULL */
                   void *(*start_routine)(void *), void *restrict_arg) {
  sigset_t sigs;
  sigemptyset(&sigs);
  sigaddset(&sigs, SIGALRM);

  // disable alarm
  sigprocmask(SIG_BLOCK, &sigs, NULL);
  if (!sigaction_set) {
    memset(&threads, 0, MAX_THREADS * sizeof(TCB));
    sigaction_set = true;
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    memset(&act, 0, sizeof(struct sigaction));
    act.sa_flags = SA_NODEFER;
    act.sa_handler = AlarmHandler;
    sigaction(SIGALRM, &act, NULL);
  }

  TCB thread_info;
  uint sp_idx, bp_idx;

  thread_info.state = ready;
  sp_idx = bp_idx = STACK_SIZE;

  thread_info.tid = num_threads;
  *restrict_thread = num_threads;
  thread_info.stack = (byte *)calloc(STACK_SIZE, sizeof(byte));
  // push arg
  sp_idx -= 4;
  *((intptr_t *)(thread_info.stack + sp_idx)) = (intptr_t)(restrict_arg);

  // push pthread_exit as ret
  sp_idx -= 4;
  *((intptr_t *)(thread_info.stack + sp_idx)) = (intptr_t)(pthread_exit);

  // save registers
  setjmp(thread_info.registers);

  // modify esp, ebp, eip
  thread_info.registers[0].__jmpbuf[JB_SP] =
      ptr_mangle((intptr_t)(thread_info.stack + sp_idx));
  thread_info.registers[0].__jmpbuf[JB_BP] =
      ptr_mangle((intptr_t)(thread_info.stack + sp_idx));
  thread_info.registers[0].__jmpbuf[JB_PC] =
      ptr_mangle((intptr_t)start_routine);

  // copy the new thread into the array of threads
  threads[thread_info.tid] = thread_info;
  ++num_threads;

  // enable alarm
  sigprocmask(SIG_UNBLOCK, &sigs, NULL);

  useconds_t remaining = ualarm(THREAD_TIMER, 0);
  if (remaining > 0) {
    ualarm(remaining, 0);
  }
  ++alive_threads;
  return thread_info.tid;
}

void pthread_exit(void *value_ptr) {
  if (threads[running_thread].state != exited) {
    threads[running_thread].state = exited;
    if (running_thread != 0) {
      free(threads[running_thread].stack);
      threads[running_thread].stack = NULL;
    }
    if (alive_threads > 1) {
      --alive_threads;
      do {
        running_thread = (running_thread + 1) % num_threads;
      } while (threads[running_thread].state != ready);
    } else {
      exit(0);
    }
    longjmp(threads[running_thread].registers, 1);
  } else {
    perror("exiting already exited thread");
    exit(-1);
  }
}

pthread_t pthread_self(void) { return threads[running_thread].tid; }
