#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int unexpected_delim(char **stack, int idx, char delim){
    free(*stack);
    printf("%d: %c\n", idx, delim);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]){
    int len = strlen(argv[1]);
    char *stack = malloc(1+len);

    for (int i = 0; i < len; i++){ // Set all value in stack to 0 so we can read them
        stack[i] = '0';
    }

    char *ptr = &(stack[0]);
    char *head = ptr;

    for (int i = 0; i < len; i++){
        char dlm = argv[1][i];
        
        // Ignore non-delimiter characters
        if (dlm != '(' && dlm != ')' && dlm != '[' && dlm != ']' && dlm != '{' && dlm != '}') continue;
        if (dlm == '(' || dlm == '{' || dlm == '[' ){
            *ptr = dlm;
            ptr++;
        } else {
            if (ptr > head) {
                char last = *(ptr-1);
                int diff = (int)(dlm)-(int)(last);
                switch(last){
                    case '(':
                    case '[':
                    case '{':
                        if (diff == 1 || diff == 2){ // For paired delims, diff will be either 1 or2
                            *(ptr-1) = '0';
                            ptr--;
                        }  else { // Unexpected delimiter, error case
                            return unexpected_delim(&stack, i, dlm);
                        }
                }

            } else { // Can not have a delimiter in the first character
                return unexpected_delim(&stack, i, dlm);
            }     
        }
    }

    if (*head != '0'){
        printf("open: ");
        ptr--;
        while (ptr >= head){
            switch(*ptr){
                case '(':
                    printf(")");
                    break;
                case '{':  
                    printf("}");
                    break;
                case '[':
                    printf("]");
                    break;
            }
            ptr--;
        }
        exit(EXIT_FAILURE);    
    }

    exit(EXIT_SUCCESS);
    free(stack);
    return 0;
}