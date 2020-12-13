#include "Cache.h"
#include <stdlib.h>
#include <math.h>

cache_t cache;
#define MEMORY_RAM_SIZE 65536
typedef unsigned char memory_ram_t[MEMORY_RAM_SIZE];
memory_ram_t memory_ram;
unsigned int number_of_access;


/* Funciones auxiliares */

int get_blocks_per_set(){
    return cache.blocks_per_set;
}

int get_offset_bits(){
    return cache.offset_bits;
}

unsigned int get_tag(address){
    address=address >> cache.offset_bits;
    unsigned int index_bits=log(cache.number_of_sets)/log(2);
    address=address >> index_bits;
    return address;
}

/* Primitivas cache */

void init(){
    printf("block size es %i \n",cache.block_size);
    cache.number_of_sets=cache.cache_size/(cache.block_size*cache.number_of_ways);
    cache.blocks_per_set=cache.cache_size/(cache.block_size*cache.number_of_sets);
    cache.total_hits=0;
    cache.total_misses=0;
    cache.cache_blocks=malloc(sizeof(block_t*)*cache.number_of_ways);
    cache.offset_bits=log(cache.block_size)/log(2);
    for(int i=0;i<cache.number_of_sets;i++){
        cache.cache_blocks[i]=malloc(sizeof(block_t)*cache.number_of_ways);
        for(int j=0;j< cache.number_of_ways;j++){
            cache.cache_blocks[i][j].data=malloc(sizeof(unsigned char)*cache.block_size);
            cache.cache_blocks[i][j].data=NULL;
            cache.cache_blocks[i][j].bit_d=0;
            cache.cache_blocks[i][j].bit_v=0;
            cache.cache_blocks[i][j].last_access=0;
        }
    }

}

unsigned int find_set(unsigned int address){
    address=address >> cache.offset_bits;
    address=address%cache.number_of_sets;
	return address;
}

unsigned int find_lru(int setnum){
    unsigned int min=cache.cache_blocks[setnum][0].last_access;
    unsigned int less_used_block=0;

    for(int i=0;i<cache.number_of_ways;i++){
        if(min>cache.cache_blocks[setnum][i].last_access){
            min=cache.cache_blocks[setnum][i].last_access;
            less_used_block=i;
        }
    }
    return less_used_block;
}




void read_block(int blocknum){
    unsigned int set=find_set(blocknum);
    unsigned int way=find_lru(set);
    unsigned int index_bits=log(cache.number_of_sets)/log(2);
    unsigned int tag= blocknum >> index_bits;
    unsigned int first_address= blocknum << cache.offset_bits;
    number_of_access++;

    cache.cache_blocks[set][way].tag=tag;
    cache.cache_blocks[set][way].last_access=number_of_access;
    cache.cache_blocks[set][way].bit_v=1;
    cache.cache_blocks[set][way].data=memory_ram[blocknum];
    //Copio en memoria cache el contenido de memoria ram, desde el bloque dado tomando 0 bits
    // de offset, copiando la totalidad de bytes dada por el tamaño de bloque
    memcpy(cache.cache_blocks[set][way].data, &memory_ram[first_address], cache.block_size);

}
unsigned int is_dirty(int way, int setnum){
    return cache.cache_blocks[setnum][way].bit_d;
}

void read_block(int blocknum);



int main(int argc,char* argv[]){
    number_of_access=0;
    cache.block_size=32;
    cache.number_of_ways=4;
    cache.cache_size=4096;
    init();
    int tam=get_offset_bits();
    printf("La cantidad de bits de offset es %i \n",tam);
    unsigned int set=find_set(45);
    printf("El set es %i \n",set);
    unsigned int dirty_test=is_dirty(1,2);
    printf("Dirty es %i \n",dirty_test);
    *(memory_ram+2)='a';
    printf("Memory ram %c \n",memory_ram[2]);
    return 0;
}

