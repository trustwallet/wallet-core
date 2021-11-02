#ifndef __FE_H__
#define __FE_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 fe means field element.
 Here the field is \Z/(2^255-19).
 */

typedef int32_t fe25519[10];

void fe25519_frombytes(fe25519 h, const unsigned char *s);

void fe25519_tobytes(unsigned char *s, const fe25519 h);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif