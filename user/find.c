#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char* getfilename(char *path){
    char *p;

    // Find first character after last slash.
    for(p=path+strlen(path); p >= path && *p != '/'; p--);
    p++;
    if(p==path) 
        return (char*)(0);
    return p;
}

void find(char *path, char *name)
{
    int fd;
    struct stat st;
    struct dirent de;
    char buf[512], *p;
		
    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch(st.type){
        case T_FILE:
            if(!strcmp(getfilename(path), name)){
                printf("%s\n", path);
			}
			close(fd);
            break;
        case T_DIR:
            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';
            while(read(fd, &de, sizeof(de)) == sizeof(de)){
                if(de.inum == 0 || !strcmp(de.name,".") || !strcmp(de.name, ".."))
                    continue;
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                find(buf, name);
            }
			close(fd);
            break;
    }
}

int main(int argc, char*argv[])
{
    if(argc == 3){
        find(argv[1], argv[2]);
    }
    exit(0);
}  
