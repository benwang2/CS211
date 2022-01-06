#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct cache_line_t {
    int valid, age;
    unsigned long int tag;
} cache_line;

typedef struct instruction_t {
    char mode;
    int set; int si;
    unsigned long int address, tag, id;
} instruction;

typedef struct conf_t {
    int cache_size;
    int block_size, block_offset, block_offset_bits;
    int num_sets, lines_per_set, set_bits;
    int lru;
} config;

typedef struct counter_t {
    int reads, writes;
    int hits, misses;
} counter;

// Recursively computes log2(n).
int rlog2(int n) {return (n > 1) ? 1 + rlog2(n / 2) : 0;}

// Checks if n is a power of 2.
int isPow2(int n){return n != 0 && (n & (n - 1)) == 0;}

// Increases all blocks age by 1.
void age(cache_line ***c, config conf){
    for (int j = 0; j < conf.num_sets*conf.lines_per_set; j++)
        if ((*c)[j]->valid)
            (*c)[j]->age++;
}

// Returns the oldest cache block in a set.
int get_oldest(cache_line ***c, config conf, instruction i){
    int oldest = i.si;
    for (int j = i.si; j < i.si + conf.lines_per_set; j++){
        if ((*c)[j]->valid){
            if ((*c)[j]->age > (*c)[oldest]->age)
                oldest = j;
        } else
            return j;
    }
    return oldest;
}

// Generates an instruction struct from an address.
instruction gen_instruction(config conf, unsigned long int address){
    instruction i;
    i.address = address;
    i.id = address >>  conf.block_offset_bits;
    i.set = i.id & ((1 << conf.set_bits) - 1);
    i.tag = i.id >> conf.set_bits;
    i.si = i.set * conf.lines_per_set;
    return i;
}

// Checks if a matching cache block exists, using an instruction struct.
int exists(cache_line ***c, config conf, instruction i){
    for (int j = i.si; j < i.si + conf.lines_per_set; j++)
        if ((*c)[j]->valid && (*c)[j]->tag == i.tag){
            if (conf.lru) (*c)[j]->age = 0;
            return 1;
        }
    return 0;
}

// Loads memory from an instruction into the oldest cache block.
void load(cache_line ***c, config d, instruction i){
    int oldest = get_oldest(c, d, i);
    (*c)[oldest]->age = 0;
    (*c)[oldest]->valid = 1;
    (*c)[oldest]->tag = i.tag;
}

// Performs a prefetching operation given an address.
void prefetch(cache_line ***cache, config conf, counter *cnt, instruction i){
    age(cache, conf);
    instruction prefetch = gen_instruction(conf, i.address + conf.block_size);
    if (exists(cache, conf, prefetch))
        return;
    cnt->reads++;
    load(cache, conf, prefetch);
}

// Simulates reading a byte given an instruction.
void read(cache_line ***cache, config conf, counter *cnt, instruction i, int pf, int lru){ 
    age(cache, conf);
    if (exists(cache, conf, i)){ 
        cnt->hits++;
    } else {
        cnt->misses++, cnt->reads++;
        load(cache, conf, i);
        if (pf) prefetch(cache, conf, cnt, i);
    }
}

// Simulates writing a byte given an instruction.
void write(cache_line ***cache, config conf, counter *cnt, instruction i, int pf, int lru){
    age(cache, conf);
    cnt->writes++;
    if (exists(cache, conf, i)){
        cnt->hits++;
    } else {
        cnt->reads++, cnt->misses++;
        load(cache, conf, i);
        if (pf) prefetch(cache, conf, cnt, i);
    }

}

void set_memory(cache_line ***cache, config conf){
    for (int i = 0; i < conf.num_sets * conf.lines_per_set; i++){
        (*cache)[i]->tag = 0;
        (*cache)[i]->age = 0;
        (*cache)[i]->valid = 0;
    }
}

void print_results(int pf, counter c){
    printf("Prefetch %d\n", pf);
    printf("Memory reads: %d\n",    c.reads);
    printf("Memory writes: %d\n",   c.writes);
    printf("Cache hits: %d\n",      c.hits);
    printf("Cache misses: %d\n",    c.misses);
}

int main(int argc, char *argv[]){
    config conf = {0,0,0,0,0,0,0,0};
    conf.lru = strcmp(argv[3],"lru") == 0; // FIFO = 0, LRU = 1
    conf.cache_size = atoi(argv[1]), conf.block_size = atoi(argv[4]);
    conf.num_sets = 1, conf.lines_per_set = 1;
    conf.block_offset_bits = rlog2(conf.block_size);

    if (strcmp(argv[2],"direct") == 0){ // 1 set to 1 line
        conf.num_sets = conf.cache_size / conf.block_size;
    } else if (strcmp(argv[2],"assoc") == 0) { // k lines to 1 set
        conf.lines_per_set = conf.cache_size / conf.block_size;
    } else { // k lines to n sets
        sscanf(argv[2], "assoc:%d\n", &(conf.lines_per_set));
        if (!(isPow2(conf.lines_per_set))) return EXIT_FAILURE;
        conf.num_sets = conf.cache_size / (conf.lines_per_set * conf.block_size);
    };

    conf.set_bits = rlog2(conf.num_sets);
    
    cache_line **cache = malloc(conf.num_sets*conf.lines_per_set * sizeof(cache_line));
    cache_line **pf_cache = malloc(conf.num_sets*conf.lines_per_set * sizeof(cache_line));
    counter c1 = {0,0,0,0}, c2 = {0,0,0,0};

    FILE* file = fopen(argv[5], "r");

    for (int i = 0; i < conf.num_sets*conf.lines_per_set; i++) 
        cache[i] = malloc(sizeof(cache_line)),
        pf_cache[i] = malloc(sizeof(cache_line));

    set_memory(&cache, conf);
    set_memory(&pf_cache, conf);

    int num_instructions = 0;
    for (; fscanf(file, "%*x: ") != EOF; num_instructions++){
        char mode = '0';
        fscanf(file,"%c",&mode);
        if (mode == '#') break;

        long int address = 0;
        fscanf(file,"%lx", &address); 

        instruction i = gen_instruction(conf, address);
        if (mode == 'R'){
            read(&cache, conf, &c1, i, 0, conf.lru);
            read(&pf_cache, conf, &c2, i, 1, conf.lru);
        } else if (mode == 'W'){
            write(&cache, conf, &c1, i, 0, conf.lru);
            write(&pf_cache, conf, &c2, i, 1, conf.lru);
        }
    }

    print_results(0, c1);
    print_results(1, c2);

    for (int i = 0; i < conf.num_sets*conf.lines_per_set; 
        free(cache[i]), free(pf_cache[i]), i++
    );
    
    free(cache);
    free(pf_cache);
    
    return 0;
}