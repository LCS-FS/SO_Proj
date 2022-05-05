#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
    int n, m;
    FILE* f;
    char c;
    //load matrices from files

    //for 1st matrix
    f = fopen(argv[1], "r");
    c = fgetc(f); //n
    n = c - '0';
    c = fgetc(f); //x
    c = fgetc(f);//m
    m = c-'0';
    c = fgetc(f);// \n


    //mmap
    int *m1 = mmap(NULL, m*n*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0);
    int *m2 = mmap(NULL, m*n*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0);
    //int *ms = mmap(NULL, m*n*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0);

    if(m1 == MAP_FAILED || m2 == MAP_FAILED){
        printf("map failed");
        return 1;
    }
    //read row by row
    for(int row = 0; row < n; row++){
        int cnt = 0;
        c = '0'; //reset c
        //read col by col
        while(c!='\n'){
            c = fgetc(f);
            if(c != ' ' && c != '\n'){
                m1[m*row+cnt] = c - '0';
                cnt++;
            }
        }
    }

    //2nd matrix
    f = fopen(argv[2], "r");
    c = fgetc(f);
    c = fgetc(f);
    c = fgetc(f);
    c = fgetc(f);

    //read row by row
    for(int row = 0; row < n; row++){
        int cnt = 0;
        c = '0'; //reset c
        //read col by col
        while(c!='\n'){
            c = fgetc(f);
            if(c != ' ' && c != '\n'){
                m2[m*row+cnt] = c - '0';
                cnt++;
            }
        }
    }

    pid_t pids[m];
    for(int i = 0; i < m; i++){
        if((pids[i] = fork()) <0){
            perror("fork");
            return 1;
        }
        else if(pids[i] == 0){
            printf("[child] pid %d from [parent] pid %d\n", getpid(), getppid());
            return 0;
        }
    }

    //wait for children
    for(int i = 0; i < m; i++){
        if (waitpid(pids[i], NULL, 0) == -1) {
        perror("wait");
        return EXIT_FAILURE;
        }
        printf("child nr %d ended\n", i);
    }
    printf("parent");
    return 0;
}