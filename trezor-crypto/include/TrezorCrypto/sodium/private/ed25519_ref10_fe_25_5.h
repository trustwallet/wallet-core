#ifndef __ED25519_REF10_FE_25_H__
#define __ED25519_REF10_FE_25_H__

#include <string.h>

#include "ed25519_ref10.h"
#include "fe_25_5/fe.h"

#ifdef __cplusplus
extern "C" {
#endif

void fe25519_0(fe25519 h);

void fe25519_1(fe25519 h);

void fe25519_add(fe25519 h, const fe25519 f, const fe25519 g);

void fe25519_sub(fe25519 h, const fe25519 f, const fe25519 g);

void fe25519_neg(fe25519 h, const fe25519 f);

void fe25519_cmov(fe25519 f, const fe25519 g, unsigned int b);

void fe25519_copy(fe25519 h, const fe25519 f);

int fe25519_isnegative(const fe25519 f);

int sodium_is_zero(const unsigned char *n, const size_t nlen);

int fe25519_iszero(const fe25519 f);

void fe25519_mul(fe25519 h, const fe25519 f, const fe25519 g);

void fe25519_sq(fe25519 h, const fe25519 f);

void fe25519_sq2(fe25519 h, const fe25519 f);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif