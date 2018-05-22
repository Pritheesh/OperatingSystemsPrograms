/*
Author    : Pritheesh Panchmahalkar
SMU ID#   : 47524741
Course    : 7343
Program   : 4

Server - genuse26
C - compiler -> gcc
Commands used to run the program
$ gcc -pthread shell_2.c -o shell2
$ ./shell2

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

#define MAX_SIZE 512
#define BUFFER_SIZE 1024
#define THREAD_POOL_SIZE 15

extern char **environ;
pthread_t pid[THREAD_POOL_SIZE];
bool flag = false;

typedef struct {
   char **arr;
   pthread_mutex_t *mutex;
   pthread_cond_t *full, *empty;
   int first, count, last;
} buffer_t;

int comparator(const void *a, const void *b) {
   return ( *(int *)a - *(int *)b);
}

void insert_cmd(buffer_t *buffer, char *cmd) {
   buffer->arr[buffer->first] = cmd;
   buffer->first = (buffer->first + 1) % MAX_SIZE;
   buffer->count++;
}

char * get_cmd(buffer_t *buffer, char *cmd) {
   cmd = strdup(buffer->arr[buffer->last]);
   buffer->count--;
   buffer->last = (buffer->last + 1) % MAX_SIZE;

   if (!strcmp(cmd, "quit\n") && flag == false) {
      for(int i = 0; i < THREAD_POOL_SIZE-1; i++)
      insert_cmd(buffer, cmd);
      flag = true;
   }
   return cmd;
}

// Reference: https://s2.smu.edu/~devans/7343/Exercises/html/environ.c.html
void exec_environ() {
   char ** env = environ;
   while (*env)
      printf("%s\n",*env++);
}

// clears strings
void clear_str(char * str) {
   while (*str)
      *str++ = 0;
}

void split_cmd(char **arr, char *cmd) {
   int i = 0, j = 0;
   char ch;
   char * str = (char *)malloc(sizeof(char) * MAX_SIZE);
   while ((ch = *cmd++) != '\0') {
      if (ch == ' ' || ch == '\n') {
         arr[i] = (char *)malloc(sizeof(char) * MAX_SIZE);
         str[j] = 0;
         arr[i++] = strdup(str);
         clear_str(str);
         j = 0;
      }
      else
         str[j++] = ch;
   }
}

void frand(char **arr) {
   FILE *fp;
   int size = atoi(arr[2]);
   if ((fp = fopen(arr[1], "w"))) {
      for (int i = 0; i < size; i++)
         fprintf(fp, "%u\n", rand());
      fclose(fp);
   }
   else
      perror("Error");
}

void write_file(char *filename, unsigned int *arr, int size) {
   FILE *fp;
   if ((fp = fopen(filename, "w"))) {
      for (int i = 0; i < size; i++)
         fprintf(fp, "%u\n", arr[i]);
      fclose(fp);
   }
   else
      perror("Error");
}

void fsort(char *filename) {
   FILE *fp;
   unsigned int *arr = (unsigned int *)malloc(sizeof(unsigned int) * MAX_SIZE);
   int i = 0;

   if ((fp = fopen(filename, "r"))) {
      fseek(fp, 0, SEEK_SET);
      while (fscanf(fp, "%u\n", &arr[i]) > 0)
         i++;
      qsort(arr, i, sizeof(unsigned int), comparator);
      fclose(fp);
      write_file(filename, arr, i);
   }
   else
      perror("Error\n");
}

void exec_cmd(buffer_t *buffer, char *cmd) {
   char **arr = (char **)malloc(MAX_SIZE * sizeof(char *));
   split_cmd(arr, cmd);

   if (!strcmp(arr[0],  "clr"))
      system("clear");
   else if (!strcmp(arr[0], "dir"))
      system("ls -al");
   else if (!strcmp(arr[0], "environ"))
      exec_environ();
   else if (!strcmp(arr[0], "frand"))
      frand(arr);
   else if (!strcmp(arr[0], "fsort"))
      fsort(arr[1]);
   else if (!strcmp(arr[0], "quit")) {
      printf("Exiting thread - %lu\n", (unsigned long)pthread_self());
      pthread_exit(NULL);
   }
   else if (!strcmp(arr[0], "help"))
      printf("Please type any of the following commands.\n1. clr\n2. dir \
         \n3. environ\n4. quit\n5. frand filename size\n6. fsort filename\n");
   else
      system(cmd);
}

void * consumer(void *temp) {
   buffer_t *buffer = (buffer_t *)temp;
   while (1) {
      char *cmd = (char *)malloc(sizeof(char) * MAX_SIZE);
      pthread_mutex_lock(buffer->mutex);
      while (buffer->count == 0)
         pthread_cond_wait(buffer->empty, buffer->mutex);
      cmd = get_cmd(buffer, cmd);
      pthread_mutex_unlock(buffer->mutex);
      pthread_cond_signal(buffer->full);
      exec_cmd(buffer, cmd);
   }
   // pthread_exit(NULL);
}

buffer_t * buffer_init() {
   buffer_t *buffer = (buffer_t *)malloc(sizeof(buffer_t));
   buffer->arr = (char **)malloc(sizeof(char*) * BUFFER_SIZE);
   buffer->mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
   buffer->full = (pthread_cond_t *)malloc(sizeof(pthread_cond_t));
   buffer->empty = (pthread_cond_t *)malloc(sizeof(pthread_cond_t));
   pthread_mutex_init(buffer->mutex, NULL);
   pthread_cond_init(buffer->full, NULL);
   pthread_cond_init(buffer->empty, NULL);
   buffer->first = buffer->count = buffer->last = 0 ;
   return buffer;
}

void free_buffer(buffer_t* buffer) {
   free(buffer->mutex);
   free(buffer->full);
   free(buffer->empty);
   free(buffer->arr);
   free(buffer);
}

void shell(buffer_t* buffer) {
   char *cmd = (char *)malloc(sizeof(char) * MAX_SIZE);
   size_t len;
   while (1) {
      printf("prish$ "); //Pritheesh shell
      fgets(cmd, MAX_SIZE, stdin);

      pthread_mutex_lock(buffer->mutex);
      while (buffer->count == MAX_SIZE)
         pthread_cond_wait(buffer->full, buffer->mutex);
      insert_cmd(buffer, cmd);
      pthread_mutex_unlock(buffer->mutex);
      pthread_cond_signal(buffer->empty);

      if (!strcmp(cmd, "quit\n")){
         pthread_cond_broadcast(buffer->empty);
         return;
      }
   }
}

int main() {
   pthread_t pid[THREAD_POOL_SIZE];
   buffer_t *buffer = buffer_init();

   srand((unsigned int)time(NULL));
   system("clear");
   for (size_t i = 0; i < THREAD_POOL_SIZE; i++)
      pthread_create(&pid[i], NULL, consumer, buffer);

   shell(buffer);

   for (size_t i = 0; i < THREAD_POOL_SIZE; i++)
      pthread_join(pid[i], NULL);

   free_buffer(buffer);

   return 0;
}
