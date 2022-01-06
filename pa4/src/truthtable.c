#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef enum {AND, OR, NAND, NOR, XOR, NOT, PASS, DECODER, MULTIPLEXER, NONE} type_t;
char *types[17] = {"AND","OR","NAND","NOR","XOR","NOT","PASS","DECODER","MULTIPLEXER","NONE"};

typedef struct gate_t {
    type_t type;
    int num_inputs;
    int num_outputs;
    int num_selectors;
    int *inputs;
    int *outputs;
    struct gate_t *next;
} gate;

typedef struct circuit_t {
    int num_inputs;
    int num_outputs;
    int num_variables;
    int num_gates;
    char *outputs;
    char *inputs;
    char *variables;
    int *indexes;
    gate *head;
} circuit;

int L_AND(int x, int y)   {return x & y;}
int L_OR(int x, int y)    {return x | y;}
int L_NAND(int x, int y)  {return !(x & y);} // works
int L_XOR(int x, int y)   {return (x & ~y) | (~x & y);}
int L_NOT(int x)          {return !x;}
int L_NOR(int x, int y)   {return !(x | y);} // works

int L_DECODER(int n, int *i) {
    int v = 0;
    for (int j = n-1; j >= 0; j--)
        v += pow(2,n-j-1)*i[j];
    return v;
}

int L_MULTIPLEXER(int n, int *i){
    int ns = log2(n);
    int s = 0;
    for (int j = 0; j < ns; j++)
        s += i[n+j] * pow(2,ns-j-1);

    return i[s];
}

type_t strtoenum (char type[17]){
    for (int i = 0; i < 9; i++)
        if (strcmp(type,types[i])==0) 
            return i;
    return NONE;
}

int* itob(int bits, int i){
    int n = pow(2,bits);
    int *b = malloc(sizeof(int)*n);

    for (int j = 0; j < n; j++) b[j] = 0;
    for (int j = 0; i > 0 ; j++, i /= 2) b[j] = i%2;

    return b;
}

void free_gates(gate **g){
    if (*g == NULL) return;
    free_gates(&((*g)->next));
    free((*g)->inputs);
    free((*g)->outputs);
    free(*g);
}

void push(circuit **circ, gate **head, char type[17], int num_inputs, int num_outputs, int *inputs, int *outputs){
    gate *new = malloc(sizeof(gate));
    new->num_inputs = num_inputs;
    new->num_outputs = num_outputs;
    new->inputs = inputs;
    new->outputs = outputs;
    new->type = strtoenum(type);
    new->next = NULL;

    if (new->type == MULTIPLEXER){ 
        
        new->num_selectors = log2(num_inputs);
        new->num_inputs += new->num_selectors;
    } else new->num_selectors = 0;

    if (*head == NULL) (*head) = new;
    else {
        gate *tmp = *head;
        for (; tmp->next != NULL; tmp = tmp->next){} 
        tmp->next = new;
    }

    (*circ)->num_gates++;
}

int compute_gate(gate *g, int *i){
    switch(g->type){
        case AND:           return L_AND(i[0], i[1]);
        case OR:            return L_OR(i[0], i[1]);
        case NAND:          return L_NAND(i[0], i[1]);
        case NOR:           return L_NOR(i[0], i[1]);
        case XOR:           return L_XOR(i[0], i[1]);
        case NOT:           return L_NOT(i[0]);
        case PASS:          return i[0];
        case DECODER:       return L_DECODER(g->num_inputs, i);
        case MULTIPLEXER:   return L_MULTIPLEXER(g->num_inputs - g->num_selectors, i);
        default:    printf("INVALID GATE TYPE: %d=%s\n",g->type,types[g->type]); return 0;
    }
}

void new_var(circuit **circ, char *k){
    (*circ)->variables = realloc((*circ)->variables, ((*circ)->num_variables+1)*17*sizeof(int));
    (*circ)->indexes = realloc((*circ)->indexes, ((*circ)->num_variables+1)*sizeof(int));

    sprintf((*circ)->variables + ((*circ)->num_variables)*17,"%s",k);
    (*circ)->indexes[(*circ)->num_variables] = (*circ)->num_variables;

    (*circ)->num_variables ++;
}

int get_var(circuit *circ, char *k){
    for (int i = 0; i < circ->num_variables; i++)
        if (strcmp(circ->variables + (i*17), k)==0)
            return i;
    
    return -1;
}

void compute_circuit(circuit *c, int n, int **input){
    int num_vars = c->num_variables;

    int *mem_v = malloc(num_vars*sizeof(int));
    mem_v[0] = 0, mem_v[1] = 1;

    for (int i = 2; i < n+2; i++) mem_v[i] = (*input)[n-i+1];
    for (int i = 2; i < n+2; i++) printf("%d ",mem_v[i]); // Print inputs

    printf("|");

    for (gate *tmp = c->head; tmp != NULL; tmp = tmp->next){
        int *inputs = malloc(tmp->num_inputs*sizeof(int));
        // printf("\n%s\n", types[tmp->type]);
        // printf("=> INPUTS, n=%d\n", tmp->num_inputs);
        for (int i = 0; i < tmp->num_inputs; i++){
            inputs[i] = mem_v[tmp->inputs[i]];
            // printf("  => %d | %s = %d\n",i,c->variables+(tmp->inputs[i]*17),inputs[i]);
        }

        int output = compute_gate(tmp, inputs);
        // printf("=> OUTPUTS\n");
        if (tmp->type == DECODER){
            for (int i = 0; i < tmp->num_outputs; i++){
                mem_v[tmp->outputs[i]] = (i == output);
                // printf("  => %s = %d\n",c->variables+tmp->outputs[i]*17, mem_v[i]);
            }
        } else {
            mem_v[tmp->outputs[0]] = output;
            // printf("\n => %s = %d\n",c->variables+(tmp->outputs[0]*17), output);
        }

        free(inputs);
    }


    for (int i = 0; i < c->num_outputs; i++){
        //printf(" %d",mem_v[get_var(c,c->outputs+(i*17))]);
        printf(" %d",mem_v[c->num_inputs+i+2]);
    }

    printf("\n");
    free(mem_v);
}

void truthtable(circuit *c){
    int bits = pow(2, c->num_inputs);
    int *input = malloc(sizeof(int)*bits);
    for (int i = 0; i < c->num_inputs; i++) input[i] = 0;
    for (int i = 0; i < bits; i++){
        compute_circuit(c, c->num_inputs, &input);

        for (int i = 0; i < bits; i++){ // increment bits
            input[i] = !input[i];
            if (input[i]==1) break;
        }
    }
    free(input);
}

int main(int argc, char *argv[]) {  
    FILE* file = fopen(argv[1],"r");
    circuit *circ = malloc(sizeof(circuit));
    circ->num_variables = 2;
    circ->num_gates = 1;
    circ->head = NULL;
    fscanf(file, "INPUT %d", &circ->num_inputs);#eof
    circ->inputs = malloc((circ->num_inputs)*sizeof(char)*17);

    circ->variables = malloc(17*circ->num_inputs*sizeof(char));
    circ->indexes = malloc(circ->num_inputs*sizeof(int));
    
    for (int i = 0; i < 2; i++){
        sprintf(circ->variables+(i*17), "%d", i);
        circ->indexes[i] = i;
    }
    
    for (int i = 0; i < circ->num_inputs; i++){
        fscanf(file, "%s ", circ->inputs+(17*i));
        new_var(&circ, circ->inputs+(17*i));
    }

    fscanf(file, "OUTPUT %d", &circ->num_outputs);
    circ->outputs = malloc(17*circ->num_outputs*sizeof(char));
    for (int i = 0; i < circ->num_outputs; i++){
        fscanf(file, "%s ", circ->outputs+(17*i));
        new_var(&circ, circ->outputs+(17*i));
    }

    char gate_name[17];
    while (fscanf(file, "%16s", gate_name) != EOF) {
        int in = 0, out = 1, sel = 0;
        
        if (strcmp(gate_name,"MULTIPLEXER")==0 || strcmp(gate_name,"DECODER")==0){
            fscanf(file, "%d", &in);
            if (strcmp(gate_name,"DECODER")==0) out = pow(2,in);
            else                                sel = in, in = pow(2,in);
        } else
            if (strcmp(gate_name,"PASS")==0 || strcmp(gate_name,"NOT")==0)      in = 1;
            else                                                                in = 2;

        int *inputs = malloc((in+sel)*sizeof(int));
        int *outputs = malloc(out*sizeof(int));

        for (int i = 0; i < in+out+sel; i++, out=out){
            char var[17] = "";
            fscanf(file, "%s", var);
            int vi = get_var(circ, var);

            if (i < in+sel) inputs[i] = (vi == -1) ? circ->num_variables : vi;                
            else            outputs[i-in-sel] = (vi == -1) ? circ->num_variables : vi;
            
            if (vi == -1) new_var(&circ, var);
        }

        push(&circ, &(circ->head), gate_name, in, out, inputs, outputs);
    }

    truthtable(circ);    
    
    free(circ->inputs);
    free(circ->outputs);
    free(circ->indexes);
    free(circ->variables);
    free_gates(&circ->head);
    free(circ);
    return 0;
};