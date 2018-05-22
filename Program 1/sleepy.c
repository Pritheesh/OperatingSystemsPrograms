/*
  Author    : Pritheesh Panchmahalkar
  SMU ID#   : 47524741
  Course    : 7343

  The process does not have the same process id when run multiple times.
  After a call to sleep function, I think that the process transitions to a
  blocked state.

  Server - genuse26
  C - compiler -> gcc
  Commands used to run the program
  $ gcc sleepy.c
  $ ./a.out 4

*/

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

void make_sleep(int count) {
  int i;
  for (i = 1; i <= count; i++) {
    sleep(1);
    printf("%d-TICK %d \n", getpid(), i);
  }
}

int main(int argc, char const *argv[]) {
  int count;

  if (argc == 2)
    count = atoi(argv[1]);
  else
    count = 5;

  printf("%d-START\n", getpid());
  make_sleep(count);

  return 0;
}
