#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define READ_END 0
#define WRITE_END 1

int main(){
  pid_t pid;
  int fd[2], fd2[2];
  int* fd_prev, fd_next;

  char line[100] = "ls -l | wc -w | wc -w | wc -w | wc -w";

  char *job, *str, *tofree; // Pointers for each job
  tofree = str = strdup(line);  // We own str's memory now.
  int count = 1;
  while ((job = strsep(&str, "|")))
  {
    printf("%s\n", job);

    pipe(fd);
    pid = fork();

    if(pid==0 && count==1) // child
    {
      if(count==1){
        dup2(fd[WRITE_END], STDOUT_FILENO);
        close(fd[READ_END]);
        close(fd[WRITE_END]);
        execlp("ls","ls","-l",(char*) NULL);
      }
      else{
        dup2(fd[WRITE_END], STDOUT_FILENO);
        close(fd[READ_END]);
        close(fd[WRITE_END]);
        execlp("ls","ls","-l",(char*) NULL);
      }
    }
    else
    {
      int status;
      close(fd[READ_END]);
      close(fd[WRITE_END]);
      waitpid(pid, &status, 0);
    }
  }
  return 0;
}

// pipe(fd);
// pid = fork();
//
// if(pid==0) // child
// {
//   dup2(fd[WRITE_END], STDOUT_FILENO);
//   close(fd[READ_END]);
//   close(fd[WRITE_END]);
//   execlp("ls","ls","-l",(char*) NULL);
// }
// else
// {
//   pid=fork();
//
//   if(pid==0) //child
//   {
//     dup2(fd[READ_END], STDIN_FILENO);
//     close(fd[WRITE_END]);
//     close(fd[READ_END]);
//     execlp("wc", "wc", "-w",(char*) NULL);
//   }
//   else
//   {
//     int status;
//     close(fd[READ_END]);
//     close(fd[WRITE_END]);
//     waitpid(pid, &status, 0);
//   }
// }



// #define READ_END 0
// #define WRITE_END 1
//
// int main()
// {
//   int pipeFd[2], pid, len;
//   char buf[100], *str = "Hello There!";
//
//   pipe( pipeFd);
//   if ((pid  = fork()) > 0) { /* parent */
//     close(pipeFd[READ_END]);
//     write(pipeFd[WRITE_END], str, strlen(str )+1);
//     printf("Parent wrote to pipe\n");
//     close(pipeFd[WRITE_END]);
//   }
//   else { /* child */
//     close(pipeFd[WRITE_END]);
//     len = read(pipeFd[READ_END], buf, sizeof(buf));
//     printf("Proc%d read: %s\n", pid, buf);
//     close(pipeFd[READ_END]);
//   }
// }
