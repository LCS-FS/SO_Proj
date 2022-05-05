#include <stdio.h>
#include <unistd.h>
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
    int *ms = mmap(NULL, m*n*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0);

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
    int c1, c2;
    for(int i = 0; i < m; i++){
        if((pids[i] = fork()) <0){
            perror("fork");
            return 1;
        }
        else if(pids[i] == 0){
            int index = i;
            //child in charge of column i
            for(int j = 0; j < n; j++){
                c1= m1[index];
                c2 = m2[index];
                ms[index] = c1+c2;
                index += m;
            }

            return 0;
        }
    }

    //wait for children
    for(int i = 0; i < m; i++){
        if (waitpid(pids[i], NULL, 0) == -1) {
        perror("wait");
        return -1;
        }
    }

    //parent print result matrix
    printf("%dx%d\n", n, m);
    int cnt = 0;
    for(int i = 0; i < n*m; i++){
        if(cnt % m == 0 && cnt != 0) printf("\n"); 
        printf("%d ", ms[i]);
        cnt++;
    }
    printf("\n");
    return 0;
}