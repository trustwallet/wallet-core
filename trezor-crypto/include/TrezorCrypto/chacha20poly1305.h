#ifndef CHACHA20POLY1305_H
#define CHACHA20POLY1305_H

#include <stdint.h>
#include "chacha20poly1305/ecrypt-sync.h"
#include <TrezorCrypto/chacha20poly1305/poly1305-donna.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    ECRYPT_ctx       chacha20;
    poly1305_context poly1305;
} chacha20poly1305_ctx;

void xchacha20poly1305_init(chacha20poly1305_ctx *ctx, uint8_t key[32], uint8_t nonce[24]);
void chacha20poly1305_encrypt(chacha20poly1305_ctx *ctx, uint8_t *in, uint8_t *out, size_t n);
void chacha20poly1305_decrypt(chacha20poly1305_ctx *ctx, uint8_t *in, uint8_t *out, size_t n);
void chacha20poly1305_auth(chacha20poly1305_ctx *ctx, uint8_t *in, size_t n);
void chacha20poly1305_finish(chacha20poly1305_ctx *ctx, uint8_t mac[16]);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // CHACHA20POLY1305_H
