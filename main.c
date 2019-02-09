#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "threads.h"

void* foo(void* v) {
  while (true) printf("a\n");
}

void* bar(void* v) {
  while (true) printf("bbbb\n");
}

int main(int argc, char* argv[]) {
  pthread_t tid1, tid2;
  int i = 42;
  pthread_create(&tid1, NULL, foo, &i);
  pthread_create(&tid2, NULL, bar, NULL);
  while (true) printf("main\n");
  return 0;
}
