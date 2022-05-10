#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_STR_SIZE 64

int main(int argc, char* argv[]){
    FILE *f;
    char cprevious;
    char cnext;
    int phraseCounter = 0;
    int phraseNumber = 0;
    char whitespace = ' ';
    char phrase[1000] = "";
    char nullchar = '\0';

    int i = 1;
    if(argc == 3){ //-l
        i = 2;
        f = fopen(argv[2], "r");
    }
    else if(argc == 2){
        f = fopen(argv[1], "r");
    }
    else{
        printf("Incorrect number of arguments");
        return 1;
    }

    if (f == NULL){
        printf("File cannot be opened\n");
    }
    else{
        do{
            cprevious = cnext;
            cnext = fgetc(f);
            if (cnext == '.' || cnext == '!' || cnext == '?'){
                strncat(phrase, &cnext, 1);
                strncat(phrase, &nullchar, 1);
                phraseCounter++;
                phraseNumber++;
                if(i == 2) printf("[%d] %s\n", phraseNumber, phrase);
                phrase[0] = '\0';
            }
            else if ((cnext == ' ' && (cprevious == '.' || cprevious == '!' || cprevious == '?')) || (cnext == '\n' && (cprevious == '.' || cprevious == '!' || cprevious == '?'))){
                continue;
            }
            else if (cnext == '\n'){
                strncat(phrase, &whitespace, 1);
            }
            else if(cnext == EOF){
                strncat(phrase, &nullchar, 1);
                phraseCounter++;
                phraseNumber++;
                if(i == 2) printf("[%d] %s\n", phraseNumber, phrase);
            }
            else{
                strncat(phrase, &cnext, 1);
            }
        }while (cnext != EOF);

        if(i == 1) printf("Number of phrases: %d\n", phraseCounter);
        fclose(f);
    }

    return 0;

}