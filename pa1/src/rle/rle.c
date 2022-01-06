#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char *argv[]){
    int count = 1;
    
    int len = strlen(argv[1]);
    char *buffer = malloc(sizeof(char)*(len*2+1));

    int bufferLen = 0;

    for (int i = 0; i < len; i++){

        if (isdigit(argv[1][i])){
            free(buffer);
            printf("error\n");
            return 0;
        }

        if (argv[1][i] != argv[1][i+1]){
            buffer[bufferLen++] = argv[1][i]; // Append the duplicated character
            do {
                sprintf(&(buffer[bufferLen++]), "%d", count); // Append the count - it will only append the first digit
                count -= 10;
            } while (count > 0); // Repeat until no more digits
            count = 1;
        } else {
            count++;
        }

        if (bufferLen > len){
            printf("%s\n",argv[1]);
            free(buffer);
            return 0;
        }
        
    }

    for (int i = 0; i < bufferLen; i++) printf("%c",buffer[i]);
    printf("\n");

    free(buffer);
    return 0;
}