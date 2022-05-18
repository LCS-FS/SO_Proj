#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/wait.h>

void swapWords(char* quote, char* arr1[], char* arr2[], int nLines);

int main(int argc, char* argv[]){

    int pipe1[2], pipe2[2];
    char quote[10000] = "";
    int pid;
    char readMessage[10000];
    char input;

    if (pipe(pipe1) == -1){
        perror("Unable to create pipe1\n");
    }

    if (pipe(pipe2) == -1){
        perror("Unable to create pipe2\n");
    }

    pid = fork();

    if (pid != 0){ //pai
        close(pipe1[0]);
        //read input
        input = getchar();
        while(input != '\0'){
            strncat(quote, &input, 1);
            input = getchar();
        }
        strncat(quote, &input, 1);
        printf("before child");

        write(pipe1[1], quote, sizeof(quote));
        if (waitpid(pid, NULL, 0) < 0) {
            perror("Cannot wait for child");
        }
        read(pipe2[0], readMessage, sizeof(readMessage));
        printf("after child");
        printf("%s", readMessage);
        close(pipe2[1]);
    //ler a string
    }
    else{ //filho
        close(pipe1[1]);

        read(pipe1[0], readMessage, sizeof(readMessage));

        FILE* cypher;
        char c;
        char* arr1[__INT8_MAX__];
        char* arr2[__INT8_MAX__];
        int counter = 0;
        char word[__INT8_MAX__] = "";

        cypher = fopen("cypher.txt", "r");

        if (cypher == NULL){
            perror("Cypher file missing");
        }
        else {
            do {
                c = fgetc(cypher);
                if (c == ' ') {
                    arr1[counter] = malloc(strlen(word) + 1);
                    strcpy(arr1[counter], word);
                    strcpy(word, "");
                } else if (c == '\n') {
                    arr2[counter] = malloc(strlen(word) + 1);
                    strcpy(arr2[counter], word);
                    counter++;
                    strcpy(word, "");
                } else {
                    strncat(word, &c, 1);
                }
            } while (c != EOF);
        }

        //swap words
        int nLines = counter +1;
        bool found;
        int i = 0;
        char word2[100] = "";
        char c2;
        char new[10000] = "";

        do{
            c2 = readMessage[i];
            i++;
            
            if(c2 == ' ' || c2 == '.' || c2 == '!' || c2 == '?' || c2 == '\n'){
                if(strcmp(word2, "") == 0){
                    strncat(new, &c2, 1); //passa para a new
                    strcpy(word2, "");
                }
                else if(c2 == '\0') break;
                else{
                    //tem a palavra completa
                    found = false;
                    //procura no lado esquerdo co cypher
                    
                    for(int j = 0; j < nLines; j++){
                        if(strcmp(word2, arr1[j]) == 0){
                            found = true;
                            strcpy(word2, arr2[j]);
                            break;
                        }
                    }
                    //procura no lado direito do cypher
                    if(!found){
                        for(int j = 0; j < nLines; j++){
                            if(strcmp(word2, arr2[j]) == 0){
                                strcpy(word2, arr1[j]);
                                break;
                            }
                        }
                    }
                    //adiciona a new
                    strcat(new, word2);
                    strncat(new, &c2, 1);
                    //coverte word para vazio
                    strcpy(word2, "");
                }
            }
            else strncat(word2, &c2, 1);
        }
        while(c2 != '\0');
        //end of swap words
        
        write(pipe2[1], new, sizeof(new));
        close(pipe2[0]);
    }
}

//nLines = counter +1
void swapWords(char quote[], char* arr1[], char* arr2[], int nLines){
    
    }
