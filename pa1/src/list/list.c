#include <stdio.h>
#include <stdlib.h>

typedef struct list_node {
	int value;
	struct list_node *next;
} node;

int length = 0;

void insert(node ** this, int value){
	if (*this == NULL){ 
		*this = (node*)malloc(sizeof(node));
		(*this)->value = value;
		(*this)->next = NULL;
		length++;
	} else if (length == 0){
		
		(*this)->value = value;
		length++;
	} else {
		if (value < (*this)->value){
			node *new = malloc(sizeof(node));
			new->value = value;
			new->next = *this;

			*this = new;
			length++;
		} else if (value > (*this)->value) {
			insert(&(*this)->next, value);
		}
	}
}

void delete(node ** this, int value){
	if (*this == NULL) return;
	if (value > (*this)->value){
		delete(&(*this)->next, value);
	} else if (value == (*this)->value){
		node *tmp = *this;
		*this = (*this)->next;
		length--;
		free(tmp);
	}
}


void print_list(node **head){
	printf("%d :",length);
	if (length > 0){
		for (node *tmp = *head; tmp != NULL; tmp = tmp->next){
			printf(" %d",tmp->value);
		}
	}
	printf("\n");
}

void free_list(node **this){
	if (*this==NULL) return;
	free_list(&((*this)->next));
	free(*this);
}

int main(int argc, char **argv){
	node *root = malloc(sizeof(node));
	root->next=NULL;

	while (1){
        char c;
        int i;
        int res = scanf(" %c %d",&c,&i);
        if (res == EOF) break;
        if (c == 'i'){insert(&root, i);}
        else if (c == 'd'){delete(&root, i);}
		print_list(&root);
    }

	free_list(&root);
	
	return 0;
}