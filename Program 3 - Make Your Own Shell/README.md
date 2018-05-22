

CSE 5/7343 Program 3 - Make Your Own Shell
Due Friday, March 9 by 11:59pm
Write a shell program that loops reading a line from standard input and checks the first word of the input line. If the first word is one of the following internal commands (or aliases) perform the designated task. Otherwise use the standard ANSI C system function to execute the line through the default system shell.

Internal Commands/Aliases:
clr

clear the screen using the system function clear:    system("clear").



dir

list the current directory contents by substituting the command (ls -al).



environ

list all the environment strings - the environment strings can be accessed from within a program by specifying the POSIX compliant environment list:
```
    extern char **environ;
```
as a global variable. environ is an array of pointers to the environment strings terminated with a NULL pointer. (see environ.c for examples of use)



quit

quit from the program with a zero return value. Use the standard exit function.



frand filename size

Since this could be a time consuming operation depending on the file size, your shell will spawn a thread that will create a text file named filename and populate it with size random unsigned integers.  The thread will run to completion in the background.



fsort filename

Since this could be a time consuming operation, your shell spawn a thread that will sort, in ascending order, an existing text file named filename that is populated with unsigned integers.  The thread will run to completion in the background.  Use a quicksort so the thread stack will get a good workout.

        For all other command line inputs, relay the command line as is to the parent shell for execution using the system function.



Code should be in 'straight' C using the gcc compiler:

>gcc –pthread shell.c

Try to eliminate compiler warning messages.

Devise a prompt that will uniquely identify to the user that your shell is being used.

Submit your .c file through Canvas.









©
