#ifndef __ENCRIPTADO_BASE64_H__
#define __ENCRIPTADO_BASE64_H__

char * encriptar_base64(const unsigned char *texto,
                    size_t longitud_texto,
                    size_t *longitud_encodeado);


unsigned char * desencriptar_base64(const char *texto,
                             size_t longitud_encodeado,
                             size_t *longitud_texto);

#endif /* __ENCRIPTADO_BASE64_H__ */
