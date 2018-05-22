/*
  Author    : Pritheesh Panchmahalkar
  SMU ID#   : 47524741
  Course    : 7343
  Program   : 5

  Server - genuse26
  C - compiler -> gcc
  Commands used to run the program
  $ gcc clock_repl.c -o clock
  $ ./clock

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define CLOCK_SIZE 4

typedef struct {
   int page;
   bool use, modify;
} frame_t;

frame_t frames[CLOCK_SIZE];
int next_frame;

void write_clock_to_file(FILE *fp) {
      fprintf(fp, "FRAME        PAGE       USE        MODIFY\n");
      for (int i = 0  ; i < CLOCK_SIZE; i++) {
         fprintf(fp, "%2d%14d%10d%12d",\
            i, frames[i].page, frames[i].use, frames[i].modify);
         if(i == next_frame)
            fprintf(fp, " <- next frame");
         fprintf(fp, "\n");
      }
}

void init_frames() {
   for(int i = 0; i < CLOCK_SIZE; i++) {
      frames[i].modify = frames[i].use = 0;
      frames[i].page   = -1;
   }
}

void update_frame(int page, char operation) {
   frames[next_frame].page = page;
   frames[next_frame].use  = 1;
   if(operation == 'w')
      frames[next_frame].modify = 1;
   else
      frames[next_frame].modify = 0;
}

bool replace_page(int page, char operation, int modify, int use) {
   for(int i = 0; i < CLOCK_SIZE; i++) {
      if(frames[next_frame].modify == modify &&
         frames[next_frame].use    == use) {
            update_frame(page, operation);
            next_frame = (next_frame+1) % CLOCK_SIZE;
            return true;
         }
      if(modify == 1)
         frames[next_frame].use = 0;
      next_frame = (next_frame+1) % CLOCK_SIZE;
   }
   return false;
}

void clock_replacement(int page, char operation) {
   for(int i = 0; i < CLOCK_SIZE; i++) {
      if(frames[i].page == page) {
         frames[i].use  = 1;
         if( operation  == 'w' )
            frames[i].modify = 1;
         return;
      }
   }

   while (1) {
      if(replace_page(page, operation, 0, 0)) break;
      if(replace_page(page, operation, 1, 0)) break;
   }
}

int main() {
   char in_file_name[]  = "testdata.txt";
   char out_file_name[] = "results.txt";
   FILE *fp_in, *fp_out;
   int  page;
   char operation;

   if(!(fp_in = fopen(in_file_name, "r"))) {
      printf("File %s could not be opened.\n", in_file_name);
      exit(1);
   }

   if(!(fp_out = fopen(out_file_name, "w"))) {
      printf("File %s could not be opened.\n", out_file_name);
      exit(1);
   }

   init_frames();

   while(fscanf(fp_in, "%d%c", &page, &operation) != EOF) {
      fprintf(fp_out, "\nPage referenced: %d %c\n", page, operation);
      clock_replacement(page, operation);
      write_clock_to_file(fp_out);
   }

   return 0;
}

/*
Output: results.txt

Page referenced: 4 r
FRAME        PAGE       USE        MODIFY
 0             4         1           0
 1            -1         0           0 <- next frame
 2            -1         0           0
 3            -1         0           0

Page referenced: 3 w
FRAME        PAGE       USE        MODIFY
 0             4         1           0
 1             3         1           1
 2            -1         0           0 <- next frame
 3            -1         0           0

Page referenced: 6 w
FRAME        PAGE       USE        MODIFY
 0             4         1           0
 1             3         1           1
 2             6         1           1
 3            -1         0           0 <- next frame

Page referenced: 1 r
FRAME        PAGE       USE        MODIFY
 0             4         1           0 <- next frame
 1             3         1           1
 2             6         1           1
 3             1         1           0

Page referenced: 5 r
FRAME        PAGE       USE        MODIFY
 0             5         1           0
 1             3         0           1 <- next frame
 2             6         0           1
 3             1         0           0

Page referenced: 5 w
FRAME        PAGE       USE        MODIFY
 0             5         1           1
 1             3         0           1 <- next frame
 2             6         0           1
 3             1         0           0

Page referenced: 2 w
FRAME        PAGE       USE        MODIFY
 0             5         1           1 <- next frame
 1             3         0           1
 2             6         0           1
 3             2         1           1

Page referenced: 5 r
FRAME        PAGE       USE        MODIFY
 0             5         1           1 <- next frame
 1             3         0           1
 2             6         0           1
 3             2         1           1

Page referenced: 8 w
FRAME        PAGE       USE        MODIFY
 0             5         0           1
 1             8         1           1
 2             6         0           1 <- next frame
 3             2         1           1

Page referenced: 9 w
FRAME        PAGE       USE        MODIFY
 0             5         0           1
 1             8         1           1
 2             9         1           1
 3             2         1           1 <- next frame

Page referenced: 0 r
FRAME        PAGE       USE        MODIFY
 0             0         1           0
 1             8         1           1 <- next frame
 2             9         1           1
 3             2         0           1

Page referenced: 5 r
FRAME        PAGE       USE        MODIFY
 0             0         1           0 <- next frame
 1             8         0           1
 2             9         0           1
 3             5         1           0

Page referenced: 2 r
FRAME        PAGE       USE        MODIFY
 0             0         0           0
 1             2         1           0
 2             9         0           1 <- next frame
 3             5         1           0

Page referenced: 7 w
FRAME        PAGE       USE        MODIFY
 0             7         1           1
 1             2         1           0 <- next frame
 2             9         0           1
 3             5         1           0

Page referenced: 4 w
FRAME        PAGE       USE        MODIFY
 0             7         1           1
 1             2         0           0
 2             4         1           1
 3             5         1           0 <- next frame

*/
