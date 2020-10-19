#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../includes/encriptado_base64.h"

char* construir_tabla_decodificadora() {

	char tabla_codificacion[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
	                                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
	                                'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
	                                'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
	                                'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
	                                'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
	                                'w', 'x', 'y', 'z', '0', '1', '2', '3',
	                                '4', '5', '6', '7', '8', '9', '+', '/'};

    char* tabla_decodificacion = malloc(256*sizeof(char));

    for (int i = 0; i < 64; i++){
        tabla_decodificacion[(unsigned char) tabla_codificacion[i]] = i;
    }
    return tabla_decodificacion;
}


char *encriptar_base64(const unsigned char *texto,
                    size_t tamanio_texto,
                    size_t *tamanio_decodificacion) {

	char tabla_codificacion[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
	                                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
	                                'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
	                                'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
	                                'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
	                                'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
	                                'w', 'x', 'y', 'z', '0', '1', '2', '3',
	                                '4', '5', '6', '7', '8', '9', '+', '/'};

	int tabla_de_restos[] = {0, 2, 1};

    *tamanio_decodificacion = 4 * ((tamanio_texto + 2) / 3);

    char *cadena_encodeada = malloc(*tamanio_decodificacion);
    if (cadena_encodeada == NULL) return NULL;

    for (int i = 0, j = 0; i < tamanio_texto;) {

        uint32_t octet_a = i < tamanio_texto ? (unsigned char)texto[i++] : 0;
        uint32_t octet_b = i < tamanio_texto ? (unsigned char)texto[i++] : 0;
        uint32_t octet_c = i < tamanio_texto ? (unsigned char)texto[i++] : 0;

        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

        cadena_encodeada[j++] = tabla_codificacion[(triple >> 3 * 6) & 0x3F];
        cadena_encodeada[j++] = tabla_codificacion[(triple >> 2 * 6) & 0x3F];
        cadena_encodeada[j++] = tabla_codificacion[(triple >> 1 * 6) & 0x3F];
        cadena_encodeada[j++] = tabla_codificacion[(triple >> 0 * 6) & 0x3F];
    }

    for (int i = 0; i < tabla_de_restos[tamanio_texto % 3]; i++)
        cadena_encodeada[*tamanio_decodificacion - 1 - i] = '=';

    return cadena_encodeada;
}


unsigned char *desencriptar_base64(const char *texto_encodeado,
                             size_t longitud_encodeado,
                             size_t *longitud_texto) {

	char* tabla_decodificacion=construir_tabla_decodificadora();

    if (longitud_encodeado % 4 != 0) return NULL;

    *longitud_texto = longitud_encodeado / 4 * 3;
    if (texto_encodeado[longitud_encodeado - 1] == '=') (*longitud_texto)--;
    if (texto_encodeado[longitud_encodeado - 2] == '=') (*longitud_texto)--;

    unsigned char *decoded_data = malloc(*longitud_texto*sizeof(char));
    if (decoded_data == NULL) return NULL;

    for (int i = 0, j = 0; i < longitud_encodeado;) {

        uint32_t sextet_a = texto_encodeado[i] == '=' ? 0 & i++ : tabla_decodificacion[texto_encodeado[i++]];
        uint32_t sextet_b = texto_encodeado[i] == '=' ? 0 & i++ : tabla_decodificacion[texto_encodeado[i++]];
        uint32_t sextet_c = texto_encodeado[i] == '=' ? 0 & i++ : tabla_decodificacion[texto_encodeado[i++]];
        uint32_t sextet_d = texto_encodeado[i] == '=' ? 0 & i++ : tabla_decodificacion[texto_encodeado[i++]];

        uint32_t triple = (sextet_a << 3 * 6)
        + (sextet_b << 2 * 6)
        + (sextet_c << 1 * 6)
        + (sextet_d << 0 * 6);

        if (j < *longitud_texto) decoded_data[j++] = (triple >> 2 * 8) & 0xFF;
        if (j < *longitud_texto) decoded_data[j++] = (triple >> 1 * 8) & 0xFF;
        if (j < *longitud_texto) decoded_data[j++] = (triple >> 0 * 8) & 0xFF;
    }

    return decoded_data;
}
