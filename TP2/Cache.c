#include "Cache.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define BYTES_FOR_CHAR 8

cache_t cache;
#define MEMORY_RAM_SIZE 65536
unsigned int accesses_number;
typedef unsigned char memory_ram_t[MEMORY_RAM_SIZE];
memory_ram_t memory_ram;
//unsigned int number_of_access;


int block_size_global=0;


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
            cache.cache_blocks[i][j].data=malloc(sizeof(unsigned char)*cache.block_size/BYTES_FOR_CHAR);
            cache.cache_blocks[i][j].bit_d=0;
            cache.cache_blocks[i][j].bit_v=0;
            cache.cache_blocks[i][j].last_access=0;
            cache.cache_blocks[i][j].tag=-1;
        }
    }
    //No se inicializa en 0 el primer set sino
    for(int j=0;j< cache.number_of_ways;j++){
        cache.cache_blocks[0][j].last_access=0;
    }
    accesses_number =0;
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
        //printf("Min es %i way es %i \n",min,i);
        //printf("LAST access es %i \n",cache.cache_blocks[setnum][i].last_access);
        if(min>cache.cache_blocks[setnum][i].last_access){
            min=cache.cache_blocks[setnum][i].last_access;
            less_used_block=i;
        }
    }
    return less_used_block;
}

unsigned int is_dirty(int way, int setnum){
    return cache.cache_blocks[setnum][way].bit_d;
}


void read_block(int blocknum){
    unsigned int set=find_set(blocknum << cache.offset_bits);
    unsigned int way=find_lru(set);
    unsigned int tag= blocknum >> cache.index_bits;
    //printf("Set a escribir es %i , way es %i \n",set,way);
    //printf("tag es %i",tag);
    unsigned int first_address= (blocknum << cache.offset_bits);
    unsigned int bytes_for_word=cache.block_size/BYTES_FOR_CHAR;
    //unsigned int first_address_byte=(first_address/BYTES_FOR_CHAR)-bytes_for_word;
    unsigned int first_address_byte=(first_address/BYTES_FOR_CHAR);
    //printf("First addres byte es %i \n",first_address_byte);
    //cache.number_of_access++;
    //printf("accesses number es %i \n",accesses_number);
    cache.cache_blocks[set][way].last_access=accesses_number;

    cache.cache_blocks[set][way].tag=tag;
    //cache.cache_blocks[set][way].last_access=cache.number_of_access;
    cache.cache_blocks[set][way].bit_v=1;
    //Copio en memoria cache el contenido de memoria ram, desde el bloque dado tomando 0 bits
    // de offset, copiando la totalidad de bytes dada por el tamaño de bloque
    printf("Set a escribir es %i , way es %i \n",set,way);
    //printf("tag es %i",tag);
   // unsigned int j=0;
    //unsigned char data_to_copy[bytes_for_word];
    
    memcpy(cache.cache_blocks[set][way].data,&memory_ram[first_address_byte],cache.block_size/BYTES_FOR_CHAR);
    //printf("lo que se escribio en cache es %s \n",cache.cache_blocks[set][way].data);
}


void write_block(int way, int setnum){

    unsigned int number_of_block=(cache.cache_blocks[setnum][way].tag << cache.index_bits)+setnum;
    unsigned int first_address= (number_of_block << cache.offset_bits);
    unsigned int bytes_for_word=cache.block_size/BYTES_FOR_CHAR;
    //unsigned int first_address_byte=(first_address/BYTES_FOR_CHAR)-bytes_for_word;
    unsigned int first_address_byte=(first_address/BYTES_FOR_CHAR);
    //printf("Number of block es %i \n",first_address_byte);
    //printf("lo que esta es %s \n",cache.cache_blocks[setnum][way].data);

    memcpy(&memory_ram[first_address_byte], \
    cache.cache_blocks[setnum][way].data, cache.block_size/BYTES_FOR_CHAR);
}

unsigned int find_way(unsigned int tag,unsigned int set){
   // block_t* block_in_set=malloc(sizeof(block_t*))
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
    unsigned int offset=address % (1 << cache.offset_bits); //Chequear
}

void write_byte(unsigned int address, unsigned char value){
    accesses_number +=1;
    unsigned int tag=get_tag(address);
    //printf("El tag de %i es %i \n",address,tag);
    unsigned int set=find_set(address);
    //printf("El set de %i es %i \n",address,set);
    unsigned int way=find_way(tag,set);
    //El offset va a ser por Byte, no por 8 Bytes
    unsigned int offset=find_offset(address);
    //printf("El offset es %i \n",offset);
    
    if(way!=-1){ //Si esta en cache
        printf("Hit! \n");
        //cache.number_of_access++;
        cache.total_hits++;
        cache.cache_blocks[set][way].data[offset]=value;
        cache.cache_blocks[set][way].bit_d=1;
        return;
    }
    printf("Miss! \n");
    //printf("Bloque a leer es %i \n",address >> cache.offset_bits);
    read_block(address >> cache.offset_bits);
    //printf("Way es %i \n",way);
}

unsigned char read_byte(unsigned int address){
    accesses_number +=1;
    unsigned int tag=get_tag(address);
    //printf("El tag de %i es %i \n",address,tag);
    unsigned int set=find_set(address);

    unsigned int way=find_way(tag,set);

    unsigned int offset=find_offset(address);

    if(way!=-1){
        printf("Hit! \n");
        //number_of_access++;
        cache.total_hits++;
        return cache.cache_blocks[set][way].data[offset];
    }
    else{
        printf("asd \n");
        printf("Miss! \n");
        return memory_ram[address];
    }
}

void pruebas(){
    init();

    int tam=get_offset_bits();
    //printf("La cantidad de bits de offset es %i \n",tam);
    unsigned int set=find_set(45);
    //printf("El set es %i \n",set);
    unsigned int dirty_test=is_dirty(1,2);
    //printf("Dirty es %i \n",dirty_test);
    *(memory_ram)='c';
    *(memory_ram+1)='h';
    *(memory_ram+2)='a';
    *(memory_ram+3)='u';


    *(memory_ram+8)='h';
    *(memory_ram+9)='o';
    *(memory_ram+10)='l';
    *(memory_ram+11)='a';
    read_block(0);
   // read_block(1);
    //read_block(0);
    //printf("lo que se escribio en cache es %s \n",cache.cache_blocks[0][0].data);
    //write_block(0,2);
}

void prueba_mem_1(){
    init();
    //printf("la es %i \n",cache.cache_blocks[0][0].last_access);
    //write_byte(0,255);
    write_byte(16384,254);
    
    write_byte(32768,248);
    write_byte(16384,'b');
   // write_byte(49152,96);
    //write_byte(0,24);
   // unsigned char carac=read_byte(0);
   // printf("Carac es %c \n",carac);
    //unsigned char carac=read_byte(1);
    //unsigned char carac=read_byte(1);
    //unsigned char carac=read_byte(1);
    /*
    R 0
    R 16384
    R 32768
    R 49152
    MR*/
}

int main(int argc,char* argv[]){
    //number_of_access=0;
    block_size_global=32;
    cache.block_size=32;
    cache.number_of_ways=4;
    cache.cache_size=4096;
    prueba_mem_1();
    /*pruebas();
    write_byte(0,'a');
    write_byte(4,'a');
    write_byte(33,'c');
    write_byte(16384,'b');
    write_byte(32768,'b');
    write_byte(49152,'b');
    write_byte(33,'b');
    write_byte(33,'c');
    write_byte(33,'d');
    unsigned char carac=read_byte(1);
    printf("El caracter leido es %c \n",carac);*/

    return 0;
}

