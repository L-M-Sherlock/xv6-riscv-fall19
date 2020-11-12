#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]){
    int i;
    int j = 0;
    int k;
    int l,m = 0;
    char block[32];
    char buf[32];
    char *p = buf;
    char *lineSplit[32];
    for(i = 1; i < argc; i++){
        // 复制命令
        lineSplit[j++] = argv[i];
    }
    // 读标准输入
    while( (k = read(0, block, sizeof(block))) > 0){
        for(l = 0; l < k; l++){
            switch(block[l]){
                case '\n':
                    buf[m] = 0;
                    m = 0;
                    lineSplit[j++] = p;
                    p = buf;
                    lineSplit[j] = 0;
                    j = argc - 1;
                    if(fork() == 0){
                        // 执行命令
                        exec(argv[1], lineSplit);
                    }
                    wait();
                    break;
                case ' ':
                    buf[m++] = 0;
                    // 存参数
                    lineSplit[j++] = p;
                    p = &buf[m];
                    break;
                default:
                    // 复制参数
                    buf[m++] = block[l];
            }
        }
    }
    exit();
}