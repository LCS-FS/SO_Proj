#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/wait.h>

char new[10000] = "";

void swapWords(char* quote, char* cy1[], char* cy2[], int nLines);

int main(int argc, char* argv[]){

    int pipe1[2], pipe2[2];
    char quote[10000] = "";
    int pid;
    char readMessage[10000] = "";

    if (pipe(pipe1) == -1){
        perror("Unable to create pipe1\n");
    }

    if (pipe(pipe2) == -1){
        perror("Unable to create pipe2\n");
    }

    pid = fork();

    if (pid != 0){ //pai
        close(pipe1[0]);
        fgets(quote, sizeof(quote), stdin);
        write(pipe1[1], quote, sizeof(quote));
        if (waitpid(pid, NULL, 0) < 0) {
            perror("Cannot wait for child");
        }
        read(pipe2[0], readMessage, sizeof(readMessage));
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

        swapWords(quote, arr1, arr2, counter+1);
        
        write(pipe2[1], new, sizeof(new));
        close(pipe2[0]);
    }
}

//nLines = counter +1
void swapWords(char quote[], char* cy1[], char* cy2[], int nLines){
    
    bool found;
    int i = 0;
    char word[100] = "";
    char c;

    do{
        c = quote[i];
        i++;
        
        if(c == ' ' || c == '.' || c == '!' || c == '?' || c == '\n'){
            if(strcmp(word, "") == 0){
                strncat(new, &c, 1); //passa para a new
                strcpy(word, "");
            }
            else if(c == '\0') break;
            else{
                //tem a palavra completa
                found = false;
                //procura no lado esquerdo co cypher
                
                for(int j = 0; j < nLines; j++){
                    if(strcmp(word, cy1[j]) == 0){
                        found = true;
                        strcpy(word, cy2[j]);
                        break;
                    }
                }
                //procura no lado direito do cypher
                if(!found){
                    for(int j = 0; j < nLines; j++){
                        if(strcmp(word, cy2[j]) == 0){
                            strcpy(word, cy1[j]);
                            break;
                        }
                    }
                }
                //adiciona a new
                strcat(new, word);
                strncat(new, &c, 1);
                //coverte word para vazio
                strcpy(word, "");
            }
        }
        else strncat(word, &c, 1);
    }
    while(c != '\0');
}
