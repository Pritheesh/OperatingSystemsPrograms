/*
  Author    : Pritheesh Panchmahalkar
  SMU ID#   : 47524741
  Course    : 7343
  Program   : 3

  Server - genuse26
  C - compiler -> gcc
  Commands used to run the program
  $ gcc -pthread shell.c -o shell
  $ ./shell

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#define MAX_SIZE 512
#define ARR_SIZE 10

extern char **environ;

typedef struct my_struct {
    char * filename;
    int size;
} thread_struct;

// for qsort
int comparator(const void *a, const void *b) {
    return ( *(int *)a - *(int *)b);
}

// thread function for frand
void *thread_frand(void *temp) {
    thread_struct *ts = (thread_struct *)temp;
    FILE *fp;
    if ((fp = fopen(ts->filename, "w"))) {
        for(int i = 0; i < ts->size; i++){
            fprintf(fp, "%u\n", rand());
         }
         sleep(5);
        fclose(fp);
    }
    else
        perror("Error");
    return NULL;
}

void frand(char **arr) {
    pthread_t tid;
    thread_struct *ts = (thread_struct *)malloc(sizeof(thread_struct));
    ts->filename = (char *)malloc(MAX_SIZE * sizeof(char));
    ts->filename = arr[1];
    ts->size = atoi(arr[2]);
    pthread_create(&tid, NULL, thread_frand, ts);
    // pthread_join(tid, NULL);
}

// Writes the size sized sorted array into the file with filename
void thread_write(char *filename, unsigned int *arr, int size) {
    FILE *fp;
    if ((fp = fopen(filename, "w"))) {
        for(int i = 0; i < size; i++)
            fprintf(fp, "%u\n", arr[i]);
        fclose(fp);
    }
    else
        perror("Error");
}

// thread function to read, sort and write back to
// the file with filename
void *thread_fsort(void *file) {
    char *filename = (char *)file;
    FILE *fp;
    unsigned int *arr = (unsigned int *)malloc(sizeof(unsigned int) * MAX_SIZE);
    int i = 0;

    if((fp = fopen(filename, "r"))) {
        fseek(fp, 0, SEEK_SET);
        while(fscanf(fp, "%u\n", &arr[i]) > 0)
            i++;
        qsort(arr, i, sizeof(unsigned int), comparator);
        fclose(fp);
        sleep(5);
        thread_write(filename, arr, i);
    }
    else
        perror("Error\n");

    return NULL;
}

void fsort(char *filename) {
    pthread_t tid;
    pthread_create(&tid, NULL, thread_fsort, filename);
    // pthread_join(tid, NULL);
}

// Reference: https://s2.smu.edu/~devans/7343/Exercises/html/environ.c.html
void exec_environ() {
    char ** env = environ;
    while (*env)
        printf("%s\n",*env++);
}

// clears strings
void clear_str(char * str) {
    while(*str)
        *str++ = 0;
}

// splits the cmd string into an array of string
// with space delimiter
void split_cmd(char **arr, char *cmd) {
    int i = 0, j = 0;
    char ch;
    char * str = (char *)malloc(sizeof(char) * MAX_SIZE);
    while((ch = *cmd++) != '\0') {
        if(ch == ' ' || ch == '\n') {
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


void exec_cmd(char *cmd) {
    char **arr = (char **)malloc(ARR_SIZE * sizeof(char *));
    split_cmd(arr, cmd);
    if (!strcmp(arr[0],  "clr"))
        system("clear");
    else if(!strcmp(arr[0], "dir"))
        system("ls -al");
    else if(!strcmp(arr[0], "environ"))
        exec_environ();
    else if(!strcmp(arr[0], "frand"))
        frand(arr);
    else if(!strcmp(arr[0], "fsort"))
        fsort(arr[1]);
    else if(!strcmp(arr[0], "quit"))
        exit(0);
    else if(!strcmp(arr[0], "help"))
        printf("Please type any of the following commands.\n1. clr\n2. dir \
               \n3. environ\n4. quit\n5. frand filename size\n6. fsort filename\n");
    else
        system(cmd);
}

void shell() {
    char *cmd;
    size_t len;
    while (1) {
        printf("prish$ "); //Pritheesh shell
        getline(&cmd, &len, stdin);
        exec_cmd(cmd);
    }
}

int main() {

    srand((unsigned int)time(NULL));
    system("clear");
    shell();

    return 0;
}
