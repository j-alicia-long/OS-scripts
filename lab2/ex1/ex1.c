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

void readFromFile (char who[50], int fd);
void readOneFromFile (char who[50], int fd);

int main(int argc, char **argv) {
  char* fileName = NULL;
  int nChild = 0;
  int fd = 0;
  char who[50];

  if (argc < 3)
  {
    printf ("Usage: %s <filename> <number_of_processes>\n", argv[0]);
    return 1;
  }

  fileName = argv[1];
  nChild = atoi(argv[2]);

  fd = open( fileName, O_RDONLY );
  if(fd < 0) {// check the error code
    fprintf(stderr, "Error opening file: %s\n", strerror( errno ));
    return 1; // error opening file
  }


  int parentPID = getpid();
  sprintf(who, "Parent [%d]", getpid()); // sets who
  readOneFromFile(who, fd);
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

  readFromFile(who, fd );
  close (fd);

  //TODO add your code

  if (getpid() == parentPID){
    for (int i = 0; i < nChild; ++i) {
      printf("Parent: Child [%d] done.\n", wait(NULL));
    }
  }


  return 0;
}

void readFromFile (char who[50], int fd)
{
  ssize_t readBytes = 1;
  char charBuf = 0;
  char strBuf[128];

  while (readBytes > 0) {
    usleep (1000);
    charBuf = 0;
    readBytes = read( fd, &charBuf, 1 );

    if( readBytes != 1 )
    {
      if( readBytes == 0 )
      {
        // printf( "EOF\n");
        return;
      }
    }
    sprintf(strBuf, "%s: %c\n", who, charBuf);
    write (STDOUT_FILENO, strBuf, strlen(strBuf));
    //TODO add your code
  }
}

void readOneFromFile (char who[50], int fd)
{
  size_t readBytes = 1;
  char charBuf = 0;
  char strBuf[128];

  readBytes = read( fd, &charBuf, 1 );

  sprintf(strBuf, "%s: %c\n", who, charBuf);
  fwrite ( &strBuf, 1, strlen(strBuf), stdout );
}
