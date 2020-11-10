//
// Created by L.M.Sherlock on 2020/11/10.
//
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void primes();

void main(int argc, char *argv[]) {
  int i;
  int fd[2];
  
  pipe(fd);
  if (fork() == 0) {
    close(0);
    dup(fd[0]);
    close(fd[0]);
    close(fd[1]);
    primes();
  } else {
    close(1);
    dup(fd[1]);
    close(fd[0]);
    close(fd[1]);
    for (i = 2; i <= 35; i++) {
      write(1, &i, sizeof(int));
    }
    close(1);
    wait();
  }
  exit();
}

void primes() {
    int n, p, len;
    int fd[2];

    if((len = read(0, &n, sizeof(int)) <= 0 || n <= 0)) {
        close(1);
        exit();
    }
    printf("prime %d\n", n);
    pipe(fd);
    if(fork() == 0) {
        close(0);
        dup(fd[0]);
        close(fd[0]);
        close(fd[1]);
        primes();
    } else {
        close(1);
        dup(fd[1]);
        close(fd[0]);
        close(fd[1]);
        while ((len = read(0, &p, sizeof(int))) > 0 && p > 0) {
            if (p % n != 0) {
            write(1, &p, sizeof(int));
            }
        }
        if (len <= 0 || p <= 0) {
        close(1);
        exit();
        }
    }
}