#ifndef _AES_H_
#define _AES_H_

#if 0
#ifndef ANDROID
typedef unsigned char 		uint8_t;
typedef unsigned short		uint16_t;
typedef unsigned int 		uint32_t;
typedef signed char			sint8_t;
typedef signed short		sint16_t;
typedef signed int			sint32_t;
#endif
#endif
#ifdef __cplusplus
extern "C"
{
#endif

void AES128_ECB_encrypt(uint8_t* input, uint8_t* key, uint8_t *output);
void AES128_ECB_decrypt(uint8_t* input, uint8_t* key, uint8_t *output);
#ifdef __cplusplus
}
#endif


#endif //_AES_H_
