#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int main(int argc, char*argv[])
{
    int i, ac;
    char c;
    char *cmd;
    char arg[32][128];

    if(argc < 2){
        exit(1);
    }else{
        for(int i=0;i<32;i++){
            memset(arg[i], 0, 128);
        }
        cmd = argv[1];
        for(i = 2; i < argc; i++){
            strcpy(arg[i-1], argv[i]);
        }
        ac = i - 1;
        while(read(0, &c, 1)==1){
            if(c=='\n'){
                arg[ac][0] = 0;
                if(fork()){
                    wait(0);
                    for(int j=i-1;j<32;j++){
                         memset(arg[j], 0, 128);
                    }
                    ac = i-1;
                }else{
                    exec(cmd, (char**)arg);
                }
            }else if(c==' '){
                if(arg[ac][0]){
                    ac++;
                }
            }else{
                memmove(arg[ac], &c, 1);
            }
        }
    }
    exit(0);
}
