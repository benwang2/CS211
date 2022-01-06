#include <stdio.h>
#include <stdlib.h>

int *multiply(int *matrix, int *matrix2, int dim){
    int *product = malloc(dim*dim*sizeof(int));
    for (int row = 0; row < dim; row++){
        for (int col = 0; col < dim; col++){
            product[row*dim + col] = 0;
            for (int i = 0; i < dim; i++){
                product[row*dim + col] += (matrix[row*dim + i] * matrix2[i*dim + col]);
            }
        }
    }
    return product;
}

int main(int argc, char **argv){
    FILE* file = fopen(argv[1],"r");

    int dim, exp;

    fscanf(file, "%d", &dim);

    int *matrix = malloc(dim*dim*sizeof(int)); // matrix to hold base values
    int *product = malloc(dim*dim*sizeof(int)); // matrix to hold product 

    for (int row = 0; row < dim; row++){
        for (int col = 0; col < dim; col++){
            int n;
            fscanf(file, "%d", &n);
            matrix[row*dim + col] = n;
            product[row*dim + col] = n;
        }
    }

    fscanf(file, "%d", &exp);
    for (int i = 0; i < exp-1; i++){
        int *calculated = multiply(matrix, product, dim);

        for (int i = 0; i < dim * dim; i++){product[i] = calculated[i];}

        free(calculated);
    }

    for (int row = 0; row < dim; row++){
        
        for (int col = 0; col < dim; col++){
            printf("%d",product[row*dim + col]);
            if (col != dim-1) printf(" ");
        }
        printf("\n");
    }

    free(matrix);
    free(product);

    return 0;
}