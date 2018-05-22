




Program 2 – Spawning a Process

Due: Friday, February 23 @ 11:59pm


Part I

You are to modify your sleepy program (program 1) as follows:

·        Name this program twoSleepy.

·        When twoSleepy is called using twoSleepy n, it will create a child process that is an exact duplicate of itself using the fork function.    If fork returns a 0 you will need to jump to the child code.  If fork returns a non-zero value you will jump to the parent code.  You will need to do a little research to fully understand how this works.  You will also need to research and use the exit and wait functions to ensure that the child process ends before the parent process.

·        Both parent and child will use the same value of n to determine the number of seconds each process should run.  Just like in program 1, implement this timing by looping n times of sleep(1) - this will put the process to sleep for one second n times before exiting.

·        In each loop print out the process ID, the parent’s process ID, and the loop count so that that particular process can be identified. The process ID can be obtained from the getpid function.  The parent process ID can be obtained from the getppid function.

·        Each line of output produced by the original process should be in the format:

           "Original Process with PID: xxxxxx and PPID: yyyyyy tick n"

     Where xxxxxx is the process ID, yyyyyy is its parent process ID, and n is the tick count.

·        Each line of output produced by the child process should be in the format:

           "Child Process with PID: xxxxxx and PPID: yyyyyy tick n"

·        Each process should also display a termination message.

·        Run the program several times with different values for n.

·        Your program will need the following header files:
```
#include <unistd.h>

#include <sys/types.h>

#include <stdlib.h>

#include <stdio.h>

#include <sys/wait.h>
```


·        Using a simple five-state process model, what states do you think each process goes through during its lifetime?  Might this explain why the output is so evenly interleaved between parent and child?  Document your answers using in-line comments.

Part II

·        Comment out the call to the sleep and wait functions in the parent.  This will ensure that the parent terminates before the child.  All other code will remain the same. Recompile and run your program several times.

·        Describe what you see using comments in your source code.  Is there anything unusual about the PPID in the child? Do some research and explain what is happening.



Part III (optional)

·        Modify you program from Part I so that the child process calls fork to create a child of its own.   Use exit and wait to ensure that the processes end in reverse order from which they were created.
