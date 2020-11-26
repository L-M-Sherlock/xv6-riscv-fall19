#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

#define MAXARGS 10

void pipecmd(char* argv[], int argc);

char whitespace[] = " \t\r\n\v";

int
getcmd(char *buf, int nbuf)
{
  fprintf(2, "@ ");
  memset(buf, 0, nbuf);
  gets(buf, nbuf);
  if(buf[0] == 0) // EOF
    return -1;
  return 0;
}

void
parseargs(char *cmd, char* argv[], int* argc)
{
  int i ,j;
  for(i = 0, j = 0; cmd[j] != '\n' && cmd[j] != '\0'; j++)
  {
    while (strchr(whitespace, cmd[j]))
    {
      j++;
    }
    argv[i++] = cmd + j;
    while(!strchr(whitespace, cmd[j]))
    {
      j++;
    }
    cmd[j] = '\0';
  }
  argv[i] = 0;
  *argc = i;
}


void
runcmd(char* argv[], int argc)
{
  
  for (int i = 0; i < argc; i++)
  {
    if (!strcmp(argv[i], "|"))
    {
      pipecmd(argv, argc);
    }
  }
  
  for (int i = 0; i < argc; i++)
  {
    if (!strcmp(argv[i], ">") && argc != i)
    {
      close(1);
      open(argv[i+1], O_CREATE|O_WRONLY);
      argv[i] = 0;
    }
  }

  for (int i = 0; i < argc; i++)
  {
    if (!strcmp(argv[i], "<") && argc != i)
    {
      close(0);
      open(argv[i+1], O_RDONLY);
      argv[i] = 0;
    }
  }

  // for (int i = 0; i < argc; i++)
  // {
  //   fprintf(2, "%s ", argv[i]);
  // }
  exec(argv[0], argv);
}

void
pipecmd(char* argv[], int argc)
{
  int i;
  for (i = 0; i < argc; i++)
  {
    if (!strcmp(argv[i], "|"))
    {
      argv[i] = 0;
      break;
    }
  }

  int pd[2];
  pipe(pd);

  if(fork()==0)
  {
    close(1);
    dup(pd[1]);
    close(pd[0]);
    close(pd[1]);
    // fprintf(2, "first %s %d", *argv, i);
    runcmd(argv, i);
  }
  if(fork()==0)
  {
    close(0);
    dup(pd[0]);
    close(pd[0]);
    close(pd[1]);
    // fprintf(2, "second %s %d", *(argv + i + 1), argc - i - 1);
    runcmd(argv + i + 1, argc - i - 1);
  } 
  close(pd[0]);
  close(pd[1]);
  wait(0);
  wait(0);
  exit(0);
}

int
main(void)
{
  static char buf[100];
  int pd;

  // Ensure that three file descriptors are open.
  while((pd = open("console", O_RDWR)) >= 0){
    if(pd >= 3){
      close(pd);
      break;
    }
  }

  // Read and run input commands.
  while(getcmd(buf, sizeof(buf)) >= 0){

    if(fork() == 0) {
      char* argv[MAXARGS];
      int argc = -1;
      parseargs(buf, argv, &argc);
      runcmd(argv, argc);
    }
    wait(0);
  }
  exit(0);
}