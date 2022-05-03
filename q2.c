#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

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

    int m1[n][m], m2[n][m];//, ms[n][m];
    //read row by row
    for(int row = 0; row < n; row++){
        int cnt = 0;
        c = '0'; //reset c
        //read col by col
        while(c!='\n'){
            c = fgetc(f);
            if(c != ' ' && c != '\n'){
                m1[row][cnt] = c - '0';
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
                m2[row][cnt] = c - '0';
                cnt++;
            }
        }
    }


    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++) printf("%d ", m1[i][j]);
        printf("\n");
    }
    printf("\n=============\n\n");
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++) printf("%d ", m2[i][j]);
        printf("\n");
    }

    printf("\n %d", m1[0][0]);
    return 0;
}