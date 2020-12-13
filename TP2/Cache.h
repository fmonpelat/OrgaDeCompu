#ifndef CACHE_H
#define CACHE_H
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

typedef struct block{
    size_t bit_v;
    size_t bit_d;
    size_t tag;
};

typedef struct cache{
    size_t total_hits;
    size_t total_misses;
    size_t number_of_ways;
    size_t cache_size;
    size_t block_size;
};




void init();

/* debe devolver el conjunto de
caché al que mapea la dirección address.*/
unsigned int find_set(int address);

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

char read_byte(int address);

/* debe escribir el
valor value en la posición correcta del bloque que corresponde a
address.*/

void write_byte(int address, char value);

/* debe devolver el porcentaje de misses
desde que se inicializó el cache.*/

int get_miss_rate();

#endif