/*************************************
  * Lab 2 Exercise 2
  * Name: Jennifer Long
  * Student No: E0446263
  * Lab Group:
 *************************************
 Warning: Make sure your code works on
 lab machine (Linux on x86)
 *************************************/

#include <stdio.h>
#include <fcntl.h>      //For stat()
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>   //for waitpid()
#include <unistd.h>     //for fork(), wait()
#include <string.h>     //for string comparison etc
#include <stdlib.h>     //for malloc()

#include <stdbool.h>    //for boolean functions

char** readTokens(int maxTokenNum, int maxTokenSize, int* readTokenNum, char* buffer);
void freeTokenArray(char** strArr, int size);
bool commandExists(char* command);
void execCommand(char* command, char** tokens);
void displayCmdMenu();

int main() {
  char line[219];        // An input line read from stdin
  int maxTokenNum = 10;
  int maxTokenSize = 19;
  int readTokenNum = 0;
  char* command = "";     // A string cmd entered by the user

  printf( "Welcome to Genie!");
  displayCmdMenu();

  while (strcmp(command, "quit") != 0)
  {
    printf ("\nEnter a command: ");
    fgets (line, sizeof(line), stdin);      // read in a line
    char** tokens = readTokens(maxTokenNum, maxTokenSize, &readTokenNum, line);
    if (readTokenNum == 0){
      printf( "You must enter a valid command! ");
      continue;
    }
    command = tokens[0];
    // printf( "Given command: %s\n", command);
    // while ( *tokens ) printf( "%s ", *tokens++ );

    if (strcmp(command, "quit") == 0) {
      printf( "Goodbye!\n");
      break;
    }

    // If not quit, check other commands
    if (commandExists(command))  // check if filepath exists
    {
      printf( "Command %s exists!\n", command);
      execCommand(command, tokens);
    }
    else {
      printf( "%s not found\n", command);
    }
    freeTokenArray(tokens, readTokenNum);
  }
  return 0;
}

void displayCmdMenu(){
  printf ("\n\nSample commands:\n");
  printf (" quit\n");
  printf (" /bin/ls -l\n");
  printf ("FORMAT: [command] x x\n");
}

// This function is a WIP
char* getBinPath(char* command){
  char cmdPath[24];  // 5 (/bin/) + 19
  strcpy(cmdPath, "/bin/");
  strcat(cmdPath, command);
  // printf("%s\n", cmdPath);
  return cmdPath;
}

// Note: any check for file access or existence of a file
// has an inherent race condition
// Stat needs exact path - /bin/ls will work
bool commandExists(char* command){
  // struct stat st;
  // if (stat(command, &st) == -1) { return false; }
  if (access(command, F_OK|X_OK) == 0)
    return true;
  else { //check if simple command in bin
    char cmdPath[24];  // 5 (/bin/) + 19
    strcpy(cmdPath, "/bin/");
    strcat(cmdPath, command);
    printf("%s\n", cmdPath);
    if (access(cmdPath, F_OK|X_OK) == 0)
      return true;
  }
  return false;
}

void execCommand(char* command, char** tokens){
  printf( "Executing command %s\n", command);
  // execv(const char *path, char *const argv[]);
  int parentPID = getpid();
  fork();
  if (getpid() != parentPID){
    printf( "Child [%d] forked\n", getpid());
    if(access(command, F_OK|X_OK) != 0) { //filepath not given
      char cmdPath[24];  // 5 (/bin/) + 19
      strcpy(cmdPath, "/bin/");
      strcat(cmdPath, command);
      command = cmdPath;
      tokens[0] = cmdPath;
    }
    execv(command, tokens);
    exit(1);
  }
  else{
    printf( "Child [%d] done\n", wait(NULL));
  }
}


char** readTokens(int maxTokenNum, int maxTokenSize, int* readTokenNum, char* buffer)
//Tokenize buffer
//Assumptions:
//  - the tokens are separated by " " and ended by newline
//Return: Tokenized substrings as array of strings
//        readTokenNum stores the total number of tokens
//Note:
//  - should use the freeTokenArray to free memory after use!
{
    char** tokenStrArr;
    char* token;
    int i;

    //allocate token array, each element is a char*
    tokenStrArr = (char**) malloc(sizeof(char*) * maxTokenNum);

    //Nullify all entries
    for (int i = 0; i < maxTokenNum; i++) {
        tokenStrArr[i] = NULL;
    }

    token = strtok(buffer, " \n");

    i = 0;
    while (i < maxTokenNum && (token != NULL)) {
         //Allocate space for token string
        tokenStrArr[i] = (char*) malloc(sizeof(char*) * maxTokenSize);

        //Ensure at most 19 + null characters are copied
        strncpy(tokenStrArr[i], token, maxTokenSize - 1);

        //Add NULL terminator in the worst case
        tokenStrArr[i][maxTokenSize-1] = '\0';

        i++;
        token = strtok(NULL, " \n");
    }

    *readTokenNum = i;

    return tokenStrArr;
}

void freeTokenArray(char** tokenStrArr, int size) {
    int i = 0;

    //Free every string stored in tokenStrArr
    for (i = 0; i < size; i++) {
        if (tokenStrArr[i] != NULL) {
            free(tokenStrArr[i]);
            tokenStrArr[i] = NULL;
        }
    }
    //Free entire tokenStrArr
    free(tokenStrArr);

    //Note: Caller still need to set the strArr parameter to NULL
    //      afterwards
}
