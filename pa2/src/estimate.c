#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct matrix_t {
    int *rows;
    int *cols;
    double *data;
} matrix;

void print_matrix(matrix *m){
    int cols = *m->cols;
    for (int i = 0; i < *m->rows; i++){
        for (int j = 0; j < cols; ++j){
            printf("%.0f\n", m->data[i*cols+j]);
        }
    }
}

matrix *new(int t_rows, int t_cols){
    matrix* m = malloc(sizeof(matrix));

    int *rows = malloc(sizeof(int));
    *rows = t_rows;

    int *cols = malloc(sizeof(int));
    *cols = t_cols;

    m->rows = rows;
    m->cols = cols;
    m->data = malloc(t_rows*t_cols*sizeof(double));

    return m;
}

void del(matrix *m){
    free(m->data);
    free(m->rows);
    free(m->cols);
    free(m);
    return;
}

matrix *multiply(matrix *m1, matrix *m2){

    if (*m1->cols != *m2->rows){
        printf("error: dimension mismatch\n");
        exit(EXIT_FAILURE);
    }
    
    int rows = *(m1->rows);
    int cols = *(m2->cols);
    matrix *p = new(rows, cols);

    for (int row = 0; row < rows; row++){
        for (int col = 0; col < cols; col++){
            p->data[row*cols + col] = 0;
            for (int i = 0; i < *m1->cols; i++){
                p->data[row*cols + col] += (
                    m1->data[(row* (*m1->cols)) + i] *
                    m2->data[(i * (*m2->cols)) + col]);
            }
        }
    }

    return p;
}

matrix *transpose(matrix *m){
    int rows = *m->rows, cols = *m->cols;

    matrix *t = new(cols, rows);

    for (int row = 0; row < rows; row++){
        for (int col = 0; col < cols; col++){
            t->data[col * rows + row] = m->data[row * cols + col];
        }
    }

    return t;
}

matrix *inverse(matrix *m){
    int rows = *m->rows, cols = *m->cols;
    matrix *i = new(rows, cols);

    for (int row = 0; row < rows; row++){
        for (int col = 0; col < rows; col++){
            i->data[row*rows + col] = (row==col) ? 1 : 0;
        }
    }
    
    for (int row = 0; row < rows; row++){
        double f = m->data[row*rows + row];

        for (int col = 0; col < cols; col++){
            m->data[row*rows + col] /= f;
            i->data[row*rows + col] /= f;
        }

        for (int row2 = row+1; row2 < rows; row2++){
            f = m->data[row2*rows + row];
            for (int col = 0; col < cols; col++){
                m->data[row2*rows + col] -= (m->data[row*rows + col] * f);
                i->data[row2*rows + col] -= (i->data[row*rows + col] * f);
            }
        }
    }

    for (int col = cols-1; col >= 0; col--){
        for (int row = col-1; row >= 0; row--){
            double f = m->data[row*rows + col];
            for (int col2 = 0; col2 < rows; col2++){
                m->data[row*rows + col2] -= (m->data[col*rows + col2]*f);
                i->data[row*rows + col2] -= (i->data[col*rows + col2]*f);
            }
        }
    }

    return i;
}

int main(int argc, char **argv){
    FILE *train = fopen(argv[1], "r");
    FILE *data = fopen(argv[2], "r");

    // make sure first file is training file
    // put this in brackets because i don't
    // want to reuse this variable later for
    // the data set
    {char data_type[6];fscanf(train, "%5s ", data_type);if (strcmp(data_type,"train")!=0) return 1;}


    int t_num_attr, t_num_houses;
    fscanf(train, "%d", &t_num_attr);
    fscanf(train, "%d", &t_num_houses);

    matrix *mat_x = new(t_num_houses, t_num_attr+1);
    matrix *mat_y = new(t_num_houses, 1);

    for (int i = 0; i < t_num_houses; i++){
        mat_x->data[i*(t_num_attr+1)] = 1; // first col is filled by 1s
        for (int j = 0; j < t_num_attr; j++){
            double tmp;// = 0;
            fscanf(train, "%lf", &tmp);
            mat_x->data[i*(t_num_attr+1) + j + 1] = tmp;
        }
        fscanf(train, "%lf", &(mat_y->data[i]));
    }

    matrix *x_transposed = transpose(mat_x);
    matrix *x_transposed_times_x = multiply(x_transposed, mat_x);
    matrix *inverse_of_x_xt = inverse(x_transposed_times_x); 
    matrix *inverse_times_transposed = multiply(inverse_of_x_xt, x_transposed);
    matrix *mat_w = multiply(inverse_times_transposed,mat_y);

    del(mat_x);
    del(mat_y);
    del(x_transposed);
    del(x_transposed_times_x);
    del(inverse_of_x_xt);
    del(inverse_times_transposed);
    
    {char data_type[6];fscanf(data, "%5s ", data_type);if (strcmp(data_type,"data")!=0) return 1;}

    int d_num_houses, d_num_attr;
    fscanf(data, "%d", &d_num_attr);
    fscanf(data, "%d", &d_num_houses);

    if (d_num_attr != t_num_attr){
        printf("error");
        return 1;
    }

    matrix *data_matrix = new(d_num_houses, d_num_attr+1);
    
    for (int i = 0; i < d_num_houses; i++){
        data_matrix->data[i*(d_num_attr+1)] = 1;
        for (int j = 0; j < d_num_attr; j++){
            double tmp;
            fscanf(data, "%lf", &tmp);
            data_matrix->data[i*(d_num_attr+1) + j+1] = tmp;
        }
    }

    matrix *predicted_prices = multiply(data_matrix, mat_w);

    print_matrix(predicted_prices);

    del(mat_w);
    del(data_matrix);
    del(predicted_prices);

    return 0;
}