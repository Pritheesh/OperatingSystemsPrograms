/*
  Author    : Pritheesh Panchmahalkar
  SMU ID#   : 47524741
  Course    : 7343
  Program   : 6

  Server - genuse26
  C - compiler -> gcc
  Commands used to run the program
  $ gcc msgQueue.c -o msg -pthread
  $ ./msg

  How are mutual exclusion and synchronization achieved between the producer
  and consumers?

  Answer: In this program, mutual exclusion and synchronization are achieved
  between the producer and consumers by the use of message queues. The Producer
  process places an integer in the message queue and only one of the  consumer
  threads can receive the message in a First-In First-Out fashion, thus
  achieving mutual exclusion. Each consumer thread has its own total sum and
  hence, one thread can't modify other thread's total sum. Also, the producer
  process and the consumer threads sleep as soon they produce or consume, thus,
  making sure both producer and consumer run. This means that message queue is
  not full or empty for a long time. The msgsnd system call is a non-blocking
  operation, while the msgrcv system call is a blocking operation as it wasn't
  used with the flag "IPC_NOWAIT". These operations help in achieving
  synchronization.

  I feel that there "might" be starvation for a few consumer threads if there
  are many of them and there are less iterations to run, as there would be many
  threads ready to execute at the same time after the producer produces and few
  threads might not be picked by the scheduler for execution. But they are
  terminated after 'N' numbers are produced and consumed ie/e., at the end of
  the execution of the program.
*/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_THREADS 3

typedef struct {
   long mtype;
   int num;
} msg_t;

typedef struct {
   int tid, count;
} thread_t;

void check_error(int check) {
   if (check < 0) {
      perror("Error. Exiting!");
      exit(0);
   }
}

void * consumer(void *arg) {
   thread_t *thread   = (thread_t *)arg;
   int key            = ftok("msgQueue.c", 23);
   int msgid          = msgget(key, 0666 | IPC_CREAT);
   int total_consumed = 0;

   check_error(key);
   check_error(msgid);

   for(int i = 0; i < thread->count; i++) {
      msg_t msg;
      msgrcv(msgid, &msg, sizeof(msg.num), 0, 0);
      total_consumed += msg.num;
      printf("Consumer thread %d consumed a %d\n", thread->tid, msg.num);
      sleep((rand()%3)+1);
   }

   printf("Total Consumed by consumer thread %d = %d\n", \
      thread->tid, total_consumed);

   return NULL;
}

void child(int N) {
   pthread_t th_id[NUM_THREADS];
   thread_t  threads[NUM_THREADS];

   for(int i = 0; i < NUM_THREADS; i++) {
      threads[i].tid   = i;
      threads[i].count = N / NUM_THREADS;
      if(N % NUM_THREADS > i)
         threads[i].count++;
      pthread_create(&th_id[i], NULL, consumer, &threads[i]);
   }

   for(int i = 0; i < NUM_THREADS; i++)
      pthread_join(th_id[i], NULL);
}

void parent(int N) {
   int total_produced = 0;
   int key            = ftok("msgQueue.c", 23);
   int msgid          = msgget(key, 0666 | IPC_CREAT);

   srand(time(0));

   check_error(key);
   check_error(msgid);

   for(int i = 0; i < N; i++) {
      msg_t msg;
      msg.mtype = 1;
      msg.num   = rand() % 1000;
      msgsnd(msgid, &msg, sizeof(msg.num), 0);
      total_produced += msg.num;
      printf("Producer produced a %d\n", msg.num);
      sleep(rand()%2);
   }

   printf("Total produced = %d\n", total_produced);
   wait(NULL);
   msgctl(msgid, IPC_RMID, NULL);
}

int main(int argc, char const *argv[]) {
   int N;

   if (argc == 2) N = atoi(argv[1]);
   else           N = 10;

   pid_t pid = fork();

   check_error(pid);

   if(pid == 0) child(N);
   else         parent(N);

   return 0;
}
