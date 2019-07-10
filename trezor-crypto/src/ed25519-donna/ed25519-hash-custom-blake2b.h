/*
	a custom hash must have a 512bit digest and implement:

	struct ed25519_hash_context;

	void ed25519_hash_init(ed25519_hash_context *ctx);
	void ed25519_hash_update(ed25519_hash_context *ctx, const uint8_t *in, size_t inlen);
	void ed25519_hash_final(ed25519_hash_context *ctx, uint8_t *hash);
	void ed25519_hash(uint8_t *hash, const uint8_t *in, size_t inlen);
*/

#ifndef ED25519_HASH_CUSTOM
#define ED25519_HASH_CUSTOM

#include <TrezorCrypto/blake2b.h>

#define ed25519_hash_context BLAKE2B_CTX
#define ed25519_hash_init(ctx) blake2b_Init(ctx, 64)
#define ed25519_hash_update(ctx, in, inlen) blake2b_Update((ctx), (in), (inlen))
#define ed25519_hash_final(ctx, hash) blake2b_Final((ctx), (hash), 64)
#define ed25519_hash(hash, in, inlen) blake2b((in), (inlen), (hash), 64)

#endif // ED25519_HASH_CUSTOM
