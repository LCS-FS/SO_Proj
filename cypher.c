#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

char new[1000] = "";

void swapWords(char* quote, char* cy1[], char* cy2[], int nLines);

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
