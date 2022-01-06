#include <stdio.h>
#include <stdlib.h>

typedef struct tree_node {
    int value;
    struct tree_node *left;
    struct tree_node *right;
} leaf;

int num_inserted = 0;

int search(leaf ** this, int n){
    for (leaf *tmp = *this;
        tmp != NULL;
        tmp = (n > tmp->value) ? tmp->right : tmp->left
    ){
        if (tmp->value == n){
            printf("present\n");
            return 1;
        }
    }
    printf("absent\n");
    return 0;
}

int insert(leaf ** this, int n){
    
    if (*this == NULL){
        *this = (leaf*)malloc(sizeof(leaf));
        (*this)->value = n;
        (*this)->left = NULL;
        (*this)->right = NULL;
        num_inserted++;
        printf("inserted\n");
        return 1;
    } else if (num_inserted == 0){
        (*this)->value = n;
        num_inserted++;
        printf("inserted\n");
        return 1;
    } else {
        if (n > (*this)->value){
            return insert(&(*this)->right, n);
        } else if (n < (*this)->value){
            return insert(&(*this)->left, n);
        } else {
            printf("not inserted\n");
            return 0;
        }
    }
}

int delete(leaf ** this, int n){
    if (*this == NULL){
        printf("absent\n");
        return 0;
    } else if ((*this)->value == n){
        if ((*this)->left != NULL && (*this)->right != NULL){
            leaf * parent = *this;
            leaf * max = (*this)->left;
            
            while (max->right != NULL){
                parent = max;
                max = max->right;
            }

            (*this)->value = max->value;
            if (max == parent->left){
                parent->left = NULL;
            } else{
                parent->right = NULL;
            }
            free(max);
            max = NULL;
        } else if ((*this)->left != NULL || (*this)->right != NULL){
            if ((*this)->left != NULL){
                leaf *tmp = (*this)->left;

                (*this)->value = tmp->value;
                (*this)->left = tmp->left;
                (*this)->right = tmp->right;

                free(tmp);
                tmp = NULL;
            } else {
                leaf *tmp = (*this)->right;

                (*this)->value = tmp->value;
                (*this)->left = tmp->left;
                (*this)->right = tmp->right;
                
                free(tmp);
                tmp = NULL;
            }
        } else {
            free(*this);
            *this = NULL;
        }
        num_inserted--;
        printf("deleted\n");
        return 1;
    } else {
        if (n > (*this)->value){
            return delete(&(*this)->right, n);
        } else {
            return delete(&(*this)->left, n);
        }
    }

}

void print_node(leaf ** head){
    if (num_inserted == 0) return;
    if (*head == NULL) return;
    printf("(");
    print_node(&(*head)->left);
    printf("%d",(*head)->value);
    print_node(&(*head)->right);
    printf(")");
}

void free_leaves(leaf ** this){
    if (*this == NULL) return;
    free_leaves(&(*this)->left);
    free_leaves(&(*this)->right);
    free(*this);    
}

// dangling leaf in 2.04 after deleting 2
int main(int argc, char **argv){
    leaf *root = malloc(sizeof(leaf));
	root->left=NULL;
    root->right=NULL;

    char last = '0'; // 0 is instruction, 1 is integer
    while (1){
        if (last == '0' || last == 'p'){

            int res = scanf("%c", &last);
            if (res == EOF){break;}
            if (last == 'p'){
                print_node(&root);
                printf("\n");
            }
        } else {
            int n;
            scanf("%d", &n);

            if (last == 'i'){insert(&root, n);}
            else if (last == 'd'){delete(&root, n);}
            else if (last == 's'){search(&root, n);}

            last = '0';
        }
    }

    free_leaves(&root);

    return 0;
}