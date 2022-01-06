#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    if (argc>1){
        int len = strlen(argv[1]);
        if (len > 0){
            int i;
            for (i=0; i<len; i++){
                printf("%c", toupper(argv[1][i]));
            }
            printf("!!\n");
        }
    }
    return 0;
}