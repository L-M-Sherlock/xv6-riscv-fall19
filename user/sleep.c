//
// Created by L.M.Sherlock on 2020/10/30.
//
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


void main(int argc, char *argv[]) {
    if(argc<2) {
        printf("input argument!\n");
        exit();
    }
    sleep(atoi(argv[1]));
    exit();
}