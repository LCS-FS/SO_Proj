#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


int main(int argc, char* argv[]){ 
    char cQuote;
    int pipe1[2], pipe2[2];
    char quote[__INT_MAX__] = "";
    int pid;

    if (pipe(pipe1) == -1){
        printf("Unable to create pipe1\n");
    }

    if (pipe(pipe2) == -1){
    printf("Unable to create pipe2\n");
    }

    pid = fork();

    if (pid != 0){ //pai
    close(pipe1[0]);
    close(pipe2[1]);

    //ler a string
    
    }
    else{ //filho
    close(pipe1[1]);
    close(pipe2[0]);
    }
}


