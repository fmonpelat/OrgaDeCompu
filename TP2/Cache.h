#ifndef CACHE_H
#define CACHE_H
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#define BYTES_FOR_CHAR 8
#define MEMORY_RAM_SIZE 65536

typedef struct block{
    int bit_v;
    int bit_d;
    int tag;
    int last_access; //Sirve para usar el algoritmo de lru
    unsigned char* data;
}block_t;

typedef struct cache{
    int total_hits;
    int total_misses;
    int number_of_ways;
    int cache_size;
    int block_size;
    int number_of_sets;
    int blocks_per_set;
    int offset_bits;
    unsigned int index_bits;
    block_t** cache_blocks;
}cache_t;

typedef unsigned char memory_ram_t[MEMORY_RAM_SIZE];


int get_blocks_per_set();

void init();

/* debe devolver el conjunto de
caché al que mapea la dirección address.*/
unsigned int find_set(unsigned int address);

/* debe devolver el bloque menos re-
cientemente usado dentro de un conjunto (o alguno de ellos si hay más
de uno), utilizando el campo correspondiente de los metadatos de los
bloques del conjunto. */
unsigned int find_lru(int setnum);

/* debe devolver el es-
tado del bit D del bloque correspondiente.*/

unsigned int is_dirty(int way, int setnum);

/* debe leer el bloque blocknum
de memoria y guardarlo en el lugar que le corresponda en la memoria
caché.*/

void read_block(int blocknum);

/* debe escribir en me-
moria los datos contenidos en el bloque setnum de la vı́a way. */
void write_block(int way, int setnum);

/* debe retornar el valor correspondien-
te a la posición de memoria address, buscándolo primero en el caché. */

unsigned char read_byte(unsigned int address);

/* debe escribir el
valor value en la posición correcta del bloque que corresponde a
address.*/

void write_byte(unsigned int address,unsigned char value);

/* debe devolver el porcentaje de misses
desde que se inicializó el cache.*/

int get_miss_rate();

/* funciones de prueba */
/*
void prueba_mem_1(void);
void prueba_mem_2(void);
void prueba_mem_3(void);
void prueba_mem_4(void);
void prueba_mem_5(void);
*/

#endif