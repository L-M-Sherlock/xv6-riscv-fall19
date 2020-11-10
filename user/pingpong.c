//
// Created by L.M.Sherlock on 2020/10/30.
//
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void main(int argc, char *argv[]) {
    int parent_fd[2];
    int child_fd[2];
    pipe(parent_fd);
    pipe(child_fd);
    write(parent_fd[1], "ping", 4);
    char s[4];
    int pid = fork();
    if(pid < 0){
        printf("fork failed");
        exit();
    }
    if(pid == 0) {
        char s[4];
        read(parent_fd[0], s, 4);
        int child_pid = getpid();
        printf("%d: received %s\n", child_pid, s);
        write(child_fd[1], "pong", 4);
        exit();
    }
    read(child_fd[0], s, 4);
    int parent_pid = getpid();
    printf("%d: received %s\n", parent_pid, s);
    exit();
}

