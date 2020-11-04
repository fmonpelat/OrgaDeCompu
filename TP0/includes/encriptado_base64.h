#ifndef __ENCRIPTADO_BASE64_H__
#define __ENCRIPTADO_BASE64_H__

void encriptar_base64(const unsigned char *texto,
                    size_t tamanio_texto,
                    char *cadena_encodeada,
                    size_t *tamanio_decodificacion);


void desencriptar_base64(const char *texto_encodeado,
                             size_t longitud_encodeado,
                             char *cadena_decodeada,
                             size_t *longitud_decodeada);

#endif /* __ENCRIPTADO_BASE64_H__ */
