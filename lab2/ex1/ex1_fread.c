/*************************************
* Lab 2 Exercise 1
* Name: Jennifer Long
* Student No: E0446263
* Lab Group:
*************************************
Warning: Make sure your code works on
lab machine (Linux on x86)
*************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     //for fork(), wait()
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <errno.h>  // for file opening error
extern int errno;


/*
read() is a low level, unbuffered read.
It makes a direct system call on UNIX.

fread() is part of the C library, and provides buffered reads.
It is usually implemented by calling read() to fill its buffer.
*/
void freadFromFile (char who[50], FILE* fd);
void freadOneFromFile (char who[50], FILE* fd);

int main(int argc, char **argv) {
  char* fileName = NULL;
  int nChild = 0;
  char who[50];

  if (argc < 3)  {
    printf ("Usage: %s <filename> <number_of_processes>\n", argv[0]);
    return 1;
  }

  fileName = argv[1];
  nChild = atoi(argv[2]);

   //fopen stores file to stream: incl buffer, position, state, etc.
  FILE* fd = fopen( fileName, "r" );
  if(!fd) {// check the error code
    fprintf(stderr, "Error fopening file: %s\n", strerror( errno ));
    return 1; // error fopening file
  }

  int parentPID = getpid();
  sprintf(who, "Parent [%d]", getpid()); // sets who
  freadOneFromFile(who, fd); //fread once first

  // Call fork() n times to create n child processes
  for (int i = 0; i < nChild; ++i) {
    // prevent child process from forking again
    if (getpid() == parentPID){
      fork();
    }
    if (getpid() != parentPID){
      sprintf(who, "Child %d[%d]", i+1, getpid()); // sets who
      break;
    }
  }

  freadFromFile(who, fd );
  fclose(fd);

  if (getpid() == parentPID){
    for (int i = 0; i < nChild; ++i) {
      printf("Parent: Child [%d] done.\n", wait(NULL));
    }
  }

  return 0;
}

void freadFromFile (char who[50], FILE* fd)
{
  size_t readBytes = 1;
  char charBuf = 0;
  char strBuf[128];

  // printf("%s\n", who);
  while (readBytes > 0) {
    usleep (1000);
    charBuf = 0;
    readBytes = fread( &charBuf, sizeof(char), 1, fd );
    // fread is reading buffered data

    if( readBytes != 1 )    {
      if( readBytes == 0 )      {
        // printf( "EOF\n");
        return;
      }
    }
    sprintf(strBuf, "%s: %c\n", who, charBuf);
    fwrite ( &strBuf, 1, strlen(strBuf), stdout );
  }
}

void freadOneFromFile (char who[50], FILE* fd)
{
  size_t readBytes = 1;
  char charBuf = 0;
  char strBuf[128];

  readBytes = fread( &charBuf, sizeof(char), 1, fd );

  sprintf(strBuf, "%s: %c\n", who, charBuf);
  fwrite ( &strBuf, 1, strlen(strBuf), stdout );
}
