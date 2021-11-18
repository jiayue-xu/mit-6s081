#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void prime(int *pd_in);

int main(int argc, char*argv[])
{
    int pd[2];
	
	if(pipe(pd)<0){
		write(2, "create pipe fail\n", 17);
		exit(1);
	}

    if(fork()){
        close(pd[0]);
        for(int i=2;i<=35;i++)
            write(pd[1], &i, 4);
        close(pd[1]);
        wait(0);
    }else{
        prime(pd);
    }
	exit(0);
}

void prime(int *pd_in){
    int pd_out[2];
    int num_in1,num_in2;
    close(pd_in[1]);

    if(pipe(pd_out)<0){
        write(2, "create pipe fail\n", 17);
		exit(1);
    }

    if(read(pd_in[0], &num_in1, 4)){
        printf("prime %d\n",num_in1);
    }else exit(0);
    if(read(pd_in[0], &num_in2, 4)){
        if(fork()){
            close(pd_out[0]);
            write(pd_out[1], &num_in2, 4);
            while(read(pd_in[0], &num_in2, 4)){
                if(num_in2%num_in1){
                    write(pd_out[1], &num_in2, 4);
                }
            }
			close(pd_out[1]);
			close(pd_in[0]);
            wait(0);
        }else{
            close(pd_in[0]);
            prime(pd_out);
        }
    }else exit(0);
}
