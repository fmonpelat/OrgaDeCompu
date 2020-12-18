#include "Cache.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>

cache_t cache;
unsigned int accesses_number;
memory_ram_t memory_ram;


/* Funciones auxiliares */

int get_blocks_per_set(){
    return cache.blocks_per_set;
}


int get_offset_bits(){
    return cache.offset_bits;
}


unsigned int get_tag(unsigned int address){
    address=address >> cache.offset_bits;
    unsigned int index_bits=log(cache.number_of_sets)/log(2);
    address=address >> index_bits;
    return address;
}


unsigned int find_way(unsigned int tag,unsigned int set){
    block_t* block_in_set=cache.cache_blocks[set];
    for (int i = 0; i < cache.number_of_ways; i++) {
        //Veo si algun bloque del conjunto tiene ese mismo tag y el bit
        //de valido en 1. Si es asi, devuelvo la via en la que se encuentra
		if (cache.cache_blocks[set][i].tag == tag && cache.cache_blocks[set][i].bit_v) {
			return i;
		}
	}
	return -1;
}


unsigned int find_offset(unsigned int address){
    return address % (1 << cache.offset_bits); //Chequear
}


/* Primitivas cache */


void init(){
    cache.number_of_sets=cache.cache_size/(cache.block_size*cache.number_of_ways);
    cache.blocks_per_set=cache.cache_size/(cache.block_size*cache.number_of_sets);
    cache.total_hits=0;
    cache.total_misses=0;
    cache.cache_blocks=malloc(sizeof(block_t*)*cache.number_of_ways);
    cache.offset_bits=log(cache.block_size)/log(2);
    cache.index_bits=log(cache.number_of_sets)/log(2);
    for(int i=0;i<cache.number_of_sets;i++){
        cache.cache_blocks[i]=malloc(sizeof(block_t)*cache.number_of_ways);
        for(int j=0;j< cache.number_of_ways;j++){
            cache.cache_blocks[i][j].data=malloc\
            (sizeof(unsigned char)*cache.block_size);
            cache.cache_blocks[i][j].bit_d=0;
            cache.cache_blocks[i][j].bit_v=0;
            cache.cache_blocks[i][j].last_access=0;
            cache.cache_blocks[i][j].tag=-1;
        }
    }
    for(int j=0;j< cache.number_of_ways;j++){
        cache.cache_blocks[0][j].last_access=0;
    }
    accesses_number=0;
}


/* Devuelve el conjunto de
caché al que mapea la dirección address.*/
unsigned int find_set(unsigned int address){
    address=address >> cache.offset_bits;
    address=address%cache.number_of_sets;
	return address;
}


/* Devuelve el bloque menos re-
cientemente usado dentro de un conjunto (o alguno de ellos si hay más
de uno), utilizando el campo correspondiente de los metadatos de los
bloques del conjunto. */
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


/* Devuelve el es-
tado del bit D del bloque correspondiente.*/
unsigned int is_dirty(int way, int setnum){
    return cache.cache_blocks[setnum][way].bit_d;
}


/* Lee el bloque blocknum
de memoria principal y lo guarda en el lugar que le corresponde en la memoria
caché.*/
void read_block(int blocknum){
    unsigned int set=find_set(blocknum << cache.offset_bits);
    unsigned int way=find_lru(set);
    unsigned int tag= blocknum >> cache.index_bits;
    unsigned int first_address= (blocknum << cache.offset_bits);
    unsigned int first_address_byte=(first_address/BITS_FOR_CHAR);
    //Si el bloque de cache que se va a reemplazar, ya estaba escrito
    //guardo lo que estaba escrito en memoria
    if(is_dirty(way,set)==1){
        write_block(way,set);
    }
    cache.cache_blocks[set][way].last_access=accesses_number;
    cache.cache_blocks[set][way].tag=tag;
    cache.cache_blocks[set][way].bit_v=1;
    //Copio en memoria cache el contenido de memoria ram, desde el bloque dado tomando 0 bits
    // de offset, copiando la totalidad de bytes dada por el tamaño de bloque
    memcpy(cache.cache_blocks[set][way].data,\
    &memory_ram[first_address_byte],cache.block_size/BITS_FOR_CHAR);
}


/* Escribe en me-
moria los datos contenidos en el bloque setnum de la vı́a way. */
void write_block(int way, int setnum){
    unsigned int number_of_block=\
    (cache.cache_blocks[setnum][way].tag << cache.index_bits)+setnum;
    unsigned int first_address= (number_of_block << cache.offset_bits);
    unsigned int first_address_byte=(first_address/BITS_FOR_CHAR);
    memcpy(&memory_ram[first_address_byte], \
    cache.cache_blocks[setnum][way].data, cache.block_size/BITS_FOR_CHAR);
}


/* Retorna el valor correspondien-
te a la posición de memoria address, buscándolo primero en el caché. */
unsigned char read_byte(unsigned int address){
    accesses_number +=1;
    unsigned int tag=get_tag(address);
    unsigned int set=find_set(address);
    unsigned int way=find_way(tag,set);
    unsigned int offset=find_offset(address);
    if(way!=-1){
        printf("Hit ");
        cache.total_hits++;
        return cache.cache_blocks[set][way].data[offset];
    }
    else{
        printf("Miss ");
        cache.total_misses++;
        read_block(address >> cache.offset_bits);
        return memory_ram[address/BITS_FOR_CHAR];
    }
}


/* Escribe el
valor value en la posición correcta del bloque que corresponde a
address.*/
void write_byte(unsigned int address, unsigned char value){
    accesses_number +=1;
    unsigned int tag=get_tag(address);
    unsigned int set=find_set(address);
    unsigned int way=find_way(tag,set);
    unsigned int offset=find_offset(address);
    
    if(way!=-1){ //Si esta en cache
        printf("Hit \n");
        cache.total_hits++;
        cache.cache_blocks[set][way].data[offset]=value;
        cache.cache_blocks[set][way].bit_d=1;
        return;
    }
    printf("Miss \n");
    cache.total_misses++;
    memory_ram[address/BITS_FOR_CHAR]=value;
    read_block(address >> cache.offset_bits);
}


/* Devuelve el porcentaje de misses
desde que se inicializó el cache.*/
int get_miss_rate(){
    return !(cache.total_hits + cache.total_misses)? 0: \
    (cache.total_misses * 100 / (cache.total_hits + cache.total_misses));
}
