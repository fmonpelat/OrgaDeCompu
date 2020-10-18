#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *encriptar_base64(const unsigned char *texto,
                    size_t longitud_texto,
                    size_t *longitud_encodeado);


unsigned char *desencriptar_base64(const char *texto,
                             size_t longitud_encodeado,
                             size_t *longitud_texto);
