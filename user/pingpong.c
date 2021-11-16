 #include "kernel/types.h"
 #include "kernel/stat.h"
 #include "user/user.h"
 
 int main(int argc, char *argv[])
 {
     int pd_ptoc[2]; // pipe from parent to child process
     int pd_ctop[2]; // pipe from child to parent proces 
     char buf[4];
 
     if(pipe(pd_ptoc)<0 || pipe(pd_ctop)<0){
         write(2, "create pipe failed\n", 19);
         exit(1);
     }
 
     if(fork()){      // parent
         close(pd_ptoc[0]);
         close(pd_ctop[1]);
         write(pd_ptoc[1], "ping", 4);
         close(pd_ptoc[1]);
         read(pd_ctop[0], buf, 4);
         printf("%d: received pong\n",getpid());
         close(pd_ctop[0]);
     }else{           // child
         close(pd_ptoc[1]);
         close(pd_ctop[0]);
         read(pd_ptoc[0], buf, 4);
         close(pd_ptoc[0]);
         printf("%d: received ping\n",getpid());
         write(pd_ctop[1], "pong", 4);
         close(pd_ctop[1]);
     }
     exit(0);
 }


