/*
  Author    : Pritheesh Panchmahalkar
  SMU ID#   : 47524741
  Course    : 7343

  Part 1:
  Each process transitions between blocked state and running state
  every time sleep is called. That is, when parent process calls
  sleep, it goes to a blocked state and then child goes into running
  state until it calls sleep. After one second parent calls sleep, it
  goes into ready state and when running, executes the statements before
  sleep and goes back into blocked state when it calls sleep. Same
  happens with the child process. This explains why the output is so
  evenly interleaved between parent and Child

  Part 2:
  If sleep and wait functions are commented in the parent, the ppid of
  child process becomes 1 and not same as pid of parent. The child Process
  becomes orphan process as the original parent died before it did and hence
  init process with pid 1 becomes the parent of the child process.

  Part 3:
  Implemented in this program.

  Server - genuse26
  C - compiler -> gcc
  Commands used to run the program
  $ gcc twoSleepy.c
  $ ./a.out 4

*/

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

// Helper function to execute the common stuff
void func(int count, char * str) {
  int i;
  for (i = 1; i <= count; i++) {
    sleep(1);
    printf("%s Process with PID: %d and PPID: %d; tick %d\n", str, getpid(),
      getppid(), i);
  }
}

void child(int count) {
  int status;

  // Child process creating a Grand Child process
  pid_t pid2 = fork();

  // Check if there is any error while creating a grand child process.
  if (pid2 == -1) {
    printf("Error creating grand child process.\n");
    exit(0);
  }

  // Check if pid is zero to execute the grand child process
  else if (pid2 == 0) {
    func(count, "Grand child");
    printf("Grand child Process terminating\n");
    exit(0);
  }

  // Execute the child process if pid is not zero and wait for
  // Grand child process to finish before Child process finishes execution.
  else {
    func(count, "Child");
    wait(&status);
    printf("Child Process terminating\n");
    exit(0);
  }
}

int main(int argc, char const *argv[]) {
  int count, status;

  if (argc == 2)
    count = atoi(argv[1]);
  else
    count = 5;

  // Parent process creating a child
  pid_t pid = fork();

  // Check if there is any error while creating a grand child process.
  if (pid == -1) {
    printf("Error in creating child process.\n");
    exit(0);
  }
  // Checking if pid is zero and execute the child process
  else if (pid == 0)
    child(count);
  // Execute the parent code if pid is not zero and wait for Child Process
  // to finish before parent finishes.
  else {
    func(count, "Parent");
    wait(&status);
    printf("Parent process terminating\n");
  }

  return 0;
}

/*

Sample output

Parent Process with PID: 4688 and PPID: 4570; tick 1
Grand child Process with PID: 4690 and PPID: 4689; tick 1
Child Process with PID: 4689 and PPID: 4688; tick 1
Parent Process with PID: 4688 and PPID: 4570; tick 2
Grand child Process with PID: 4690 and PPID: 4689; tick 2
Child Process with PID: 4689 and PPID: 4688; tick 2
Parent Process with PID: 4688 and PPID: 4570; tick 3
Grand child Process with PID: 4690 and PPID: 4689; tick 3
Child Process with PID: 4689 and PPID: 4688; tick 3
Parent Process with PID: 4688 and PPID: 4570; tick 4
Grand child Process with PID: 4690 and PPID: 4689; tick 4
Child Process with PID: 4689 and PPID: 4688; tick 4
Parent Process with PID: 4688 and PPID: 4570; tick 5
Grand child Process with PID: 4690 and PPID: 4689; tick 5
Grand child Process terminating
Child Process with PID: 4689 and PPID: 4688; tick 5
Child Process terminating
Parent process terminating

*/
