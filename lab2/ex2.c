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
#include <unistd.h>     //for fork(), wait(), pipe()
#include <string.h>     //for string comparison etc
#include <stdlib.h>     //for malloc()

#include <stdbool.h>    //for boolean functions

char** readTokens(int maxTokenNum, int maxTokenSize, int* readTokenNum, char* buffer);
void freeTokenArray(char** strArr, int size);
bool commandExists(char* command, char *cmdPath[]);
void execCommand(char* cmdPath, char** tokens);
void displayCmdMenu();

int main() {
  char line[219];        // An input line read from stdin
  int maxTokenNum = 10;
  int maxTokenSize = 19;
  int readTokenNum = 0;
  char* command = "";     // A string cmd entered by the user
  char** tokens; // A list of args in each command

  printf( "Welcome to Genie!");
  displayCmdMenu();

  while (strcmp(command, "quit") != 0)
  {
    printf ("\nGENIE > ");
    fgets (line, sizeof(line), stdin);      // read in a line

    tokens = readTokens(maxTokenNum, maxTokenSize, &readTokenNum, line);
    if (readTokenNum == 0){
      printf( "You must enter a valid command! ");
      break;
    }
    command = tokens[0];
    // printf( "Command: %s\n", command );

    if (strcmp(command, "quit") == 0) {
      printf( "Goodbye!\n");
      return 0; // exit program
    }

    // If not quit, check other commands
    char cmdPath[28];
    if (!commandExists(command, &cmdPath)) { // check if filepath exists
      printf( "%s not found\n", command);
    }
    else {
      execCommand(cmdPath, tokens);
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
// char* getBinPath(char* command){
//   char cmdPath[24];  // 5 (/bin/) + 19
//   strcpy(cmdPath, "/bin/");
//   strcat(cmdPath, command);
//   // printf("%s\n", cmdPath);
//   return cmdPath;
// }

/*
  Check for valid command path, and sets it to a given var
// Note: any check for file access or existence of a file
// has an inherent race condition
// Stat needs exact path - /bin/ls will work
*/
bool commandExists(char* command, char *cmdPath[]){
  // Current dir
  if (access(command, F_OK|X_OK) == 0){
    strcpy(cmdPath, command);
    return true;
  }

  // Check if simple command in bin
  char tempPath[28];  // 5 (/bin/) + 19
  strcpy(tempPath, "/bin/");
  strcat(tempPath, command);
  // printf("%s\n", cmdPath);
  if (access(tempPath, F_OK|X_OK) == 0){
    strcpy(cmdPath, tempPath);
    return true;
  }

  strcpy(tempPath, "/usr/bin/");
  strcat(tempPath, command);
  if (access(tempPath, F_OK|X_OK) == 0){
    strcpy(cmdPath, tempPath);
    return true;
  }

  return false;
}

void execCommand(char* cmdPath, char** tokens){
  // printf( "Executing command %s\n", cmdPath);
  int parentPID = getpid();
  fork();
  if (getpid() != parentPID){
    execv(cmdPath, tokens);
    exit(1);
  }
  else{
    wait(NULL);
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
