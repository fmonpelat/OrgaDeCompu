#include "../includes/encriptado_base64.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define BUFFER_SIZE 100

int main(int argc, char** argv){

	FILE* archivo;
	FILE* archivo_salida;
	archivo=fopen("quijote.txt","rb");

	char buffer[BUFFER_SIZE];

	size_t tamanio=fread(buffer, 1, BUFFER_SIZE, archivo);
	char* texto_leido=malloc(sizeof(unsigned char)*tamanio);
	size_t tamanio_encodeado;
	unsigned char* texto_decodificado;

	memcpy(texto_leido,buffer,tamanio);

	char* encodeado=encriptar_base64(texto_leido,tamanio);

	texto_decodificado=desencriptar_base64(encodeado,tamanio_encodeado);

	archivo_salida=fopen("salida.txt","w");

	fwrite(texto_decodificado,1,strlen(texto_decodificado)+1,archivo_salida);

	fclose(archivo);
	fclose(archivo_salida);

	return 0;
}
