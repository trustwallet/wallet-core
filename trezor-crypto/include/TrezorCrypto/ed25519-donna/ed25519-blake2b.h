#ifndef ED25519_BLAKE2B_H
#define ED25519_BLAKE2B_H

#include <TrezorCrypto/ed25519.h>

#if defined(__cplusplus)
extern "C" {
#endif

void ed25519_publickey_blake2b(const ed25519_secret_key sk, ed25519_public_key pk);

int ed25519_sign_open_blake2b(const unsigned char *m, size_t mlen, const ed25519_public_key pk, const ed25519_signature RS);
void ed25519_sign_blake2b(const unsigned char *m, size_t mlen, const ed25519_secret_key sk, const ed25519_public_key pk, ed25519_signature RS);

int ed25519_scalarmult_blake2b(ed25519_public_key res, const ed25519_secret_key sk, const ed25519_public_key pk);

#if defined(__cplusplus)
}
#endif

#endif // ED25519_BLAKE2B_H
