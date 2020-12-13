#include "Cache.h"

cache_t cache;

void init(){
    printf("block size es %i \n",cache.block_size);
    cache.number_of_sets=cache.cache_size/(cache.block_size*cache.number_of_ways);
    cache.blocks_per_set=cache.cache_size/(cache.block_size*cache.number_of_sets);
}

int get_blocks_per_set(){
    return cache.blocks_per_set;
}

int main(int argc,char* argv[]){

    cache.block_size=16;
    cache.number_of_ways=4;
    cache.cache_size=256;
    init();
    int tam=get_blocks_per_set();
    printf("Tam es %i \n",tam);

    return 0;
}

