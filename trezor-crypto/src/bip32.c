/**
 * Copyright (c) 2013-2016 Tomas Dzetkulic
 * Copyright (c) 2013-2016 Pavol Rusnak
 * Copyright (c) 2015-2016 Jochen Hoenicke
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include <string.h>
#include <stdbool.h>

#include "options.h"

#include <TrezorCrypto/address.h>
#include <TrezorCrypto/aes.h>
#include <TrezorCrypto/base58.h>
#include <TrezorCrypto/bignum.h>
#include <TrezorCrypto/bip32.h>
#include <TrezorCrypto/curves.h>
#include <TrezorCrypto/ecdsa.h>
#include <TrezorCrypto/ed25519-donna/ed25519-blake2b.h>
#include <TrezorCrypto/ed25519-donna/ed25519-keccak.h>
#include <TrezorCrypto/ed25519-donna/ed25519-sha3.h>
#include <TrezorCrypto/ed25519.h>
#include <TrezorCrypto/hmac.h>
#include <TrezorCrypto/memzero.h>
#include <TrezorCrypto/nem.h>
#include <TrezorCrypto/nist256p1.h>
#include <TrezorCrypto/pbkdf2.h>
#include <TrezorCrypto/secp256k1.h>
#include <TrezorCrypto/sha2.h>
#include <TrezorCrypto/sha3.h>
#include <TrezorCrypto/sodium/keypair.h>

const curve_info ed25519_info = {
	.bip32_name = "ed25519 seed",
	.params = NULL,
	.hasher_base58 = HASHER_SHA2D,
	.hasher_sign = HASHER_SHA2D,
	.hasher_pubkey = HASHER_SHA2_RIPEMD,
	.hasher_script = HASHER_SHA2,
};

const curve_info ed25519_cardano_info = {
	.bip32_name = "ed25519 cardano seed",
	.params = NULL,
	.hasher_base58 = HASHER_SHA2D,
	.hasher_sign = HASHER_SHA2D,
	.hasher_pubkey = HASHER_SHA2_RIPEMD,
	.hasher_script = HASHER_SHA2,
};

const curve_info ed25519_blake2b_nano_info = {
	.bip32_name = "ed25519 seed",
	.params = NULL,
	.hasher_base58 = HASHER_SHA2D,
	.hasher_sign = HASHER_SHA2D,
	.hasher_pubkey = HASHER_SHA2_RIPEMD,
	.hasher_script = HASHER_SHA2,
};

const curve_info ed25519_sha3_info = {
	.bip32_name = "ed25519-sha3 seed",
	.params = NULL,
	.hasher_base58 = HASHER_SHA2D,
	.hasher_sign = HASHER_SHA2D,
	.hasher_pubkey = HASHER_SHA2_RIPEMD,
	.hasher_script = HASHER_SHA2,
};

const curve_info ed25519_keccak_info = {
	.bip32_name = "ed25519-keccak seed",
	.params = NULL,
	.hasher_base58 = HASHER_SHA2D,
	.hasher_sign = HASHER_SHA2D,
	.hasher_pubkey = HASHER_SHA2_RIPEMD,
	.hasher_script = HASHER_SHA2,
};

const curve_info curve25519_info = {
	.bip32_name = "curve25519 seed",
	.params = NULL,
	.hasher_base58 = HASHER_SHA2D,
	.hasher_sign = HASHER_SHA2D,
	.hasher_pubkey = HASHER_SHA2_RIPEMD,
	.hasher_script = HASHER_SHA2,
};

int hdnode_from_xpub(uint32_t depth, uint32_t child_num, const uint8_t *chain_code, const uint8_t *public_key, const char* curve, HDNode *out)
{
	const curve_info *info = get_curve_by_name(curve);
	if (info == 0) {
		return 0;
	}
	if (public_key[0] != 0x02 && public_key[0] != 0x03) { // invalid pubkey
		return 0;
	}
	out->curve = info;
	out->depth = depth;
	out->child_num = child_num;
	memcpy(out->chain_code, chain_code, 32);
	memzero(out->private_key, 32);
	memzero(out->private_key_extension,32);
	memcpy(out->public_key, public_key, 33);
	return 1;
}

int hdnode_from_xprv(uint32_t depth, uint32_t child_num, const uint8_t *chain_code, const uint8_t *private_key, const char* curve, HDNode *out)
{
	bool failed = false;
	const curve_info *info = get_curve_by_name(curve);
	if (info == 0) {
		failed = true;
	} else if (info->params) {
		bignum256 a;
		bn_read_be(private_key, &a);
		if (bn_is_zero(&a)) { // == 0
			failed = true;
		} else {
			if (!bn_is_less(&a, &info->params->order)) { // >= order
				failed = true;
			}
		}
		memzero(&a, sizeof(a));
	}

	if (failed) {
		return 0;
	}

	out->curve = info;
	out->depth = depth;
	out->child_num = child_num;
	memcpy(out->chain_code, chain_code, 32);
	memcpy(out->private_key, private_key, 32);
	memzero(out->public_key, sizeof(out->public_key));
	memzero(out->private_key_extension, sizeof(out->private_key_extension));
	return 1;
}

int hdnode_from_seed(const uint8_t *seed, int seed_len, const char* curve, HDNode *out)
{
	CONFIDENTIAL uint8_t I[32 + 32];
	memset(out, 0, sizeof(HDNode));
	out->depth = 0;
	out->child_num = 0;
	out->curve = get_curve_by_name(curve);
	if (out->curve == 0) {
		return 0;
	}
	CONFIDENTIAL HMAC_SHA512_CTX ctx;
	hmac_sha512_Init(&ctx, (const uint8_t*) out->curve->bip32_name, strlen(out->curve->bip32_name));
	hmac_sha512_Update(&ctx, seed, seed_len);
	hmac_sha512_Final(&ctx, I);

	if (out->curve->params) {
		bignum256 a;
		while (true) {
			bn_read_be(I, &a);
			if (!bn_is_zero(&a) // != 0
				&& bn_is_less(&a, &out->curve->params->order)) { // < order
				break;
			}
			hmac_sha512_Init(&ctx, (const uint8_t*) out->curve->bip32_name, strlen(out->curve->bip32_name));
			hmac_sha512_Update(&ctx, I, sizeof(I));
			hmac_sha512_Final(&ctx, I);
		}
		memzero(&a, sizeof(a));
	}
	memcpy(out->private_key, I, 32);
	memcpy(out->chain_code, I + 32, 32);
	memzero(out->public_key, sizeof(out->public_key));
	memzero(I, sizeof(I));
	return 1;
}

uint32_t hdnode_fingerprint(HDNode *node)
{
	uint8_t digest[32];
	uint32_t fingerprint;

	hdnode_fill_public_key(node);
	hasher_Raw(node->curve->hasher_pubkey, node->public_key, 33, digest);
	fingerprint = ((uint32_t) digest[0] << 24) + (digest[1] << 16) + (digest[2] << 8) + digest[3];
	memzero(digest, sizeof(digest));
	return fingerprint;
}

int hdnode_private_ckd(HDNode *inout, uint32_t i)
{
	CONFIDENTIAL uint8_t data[1 + 32 + 4];
	CONFIDENTIAL uint8_t I[32 + 32];
	CONFIDENTIAL bignum256 a, b;

	if (i & 0x80000000) { // private derivation
		data[0] = 0;
		memcpy(data + 1, inout->private_key, 32);
	} else { // public derivation
		if (!inout->curve->params) {
			return 0;
		}
		hdnode_fill_public_key(inout);
		memcpy(data, inout->public_key, 33);
	}
	write_be(data + 33, i);

	bn_read_be(inout->private_key, &a);

	CONFIDENTIAL HMAC_SHA512_CTX ctx;
	hmac_sha512_Init(&ctx, inout->chain_code, 32);
	hmac_sha512_Update(&ctx, data, sizeof(data));
	hmac_sha512_Final(&ctx, I);

	if (inout->curve->params) {
		while (true) {
			bool failed = false;
			bn_read_be(I, &b);
			if (!bn_is_less(&b, &inout->curve->params->order)) { // >= order
				failed = true;
			} else {
				bn_add(&b, &a);
				bn_mod(&b, &inout->curve->params->order);
				if (bn_is_zero(&b)) {
					failed = true;
				}
			}

			if (!failed) {
				bn_write_be(&b, inout->private_key);
				break;
			}

			data[0] = 1;
			memcpy(data + 1, I + 32, 32);
			hmac_sha512_Init(&ctx, inout->chain_code, 32);
			hmac_sha512_Update(&ctx, data, sizeof(data));
			hmac_sha512_Final(&ctx, I);
		}
	} else {
		memcpy(inout->private_key, I, 32);
	}

	memcpy(inout->chain_code, I + 32, 32);
	inout->depth++;
	inout->child_num = i;
	memzero(inout->public_key, sizeof(inout->public_key));

	// making sure to wipe our memory
	memzero(&a, sizeof(a));
	memzero(&b, sizeof(b));
	memzero(I, sizeof(I));
	memzero(data, sizeof(data));
	return 1;
}

static void scalar_multiply8(const uint8_t *src, int bytes, uint8_t *dst)
{
	uint8_t prev_acc = 0;
	for (int i = 0; i < bytes; i++) {
		dst[i] = (src[i] << 3) + (prev_acc & 0x7);
		prev_acc = src[i] >> 5;
	}
	dst[bytes] = src[bytes - 1] >> 5;
}

static void scalar_add_256bits(const uint8_t *src1, const uint8_t *src2, uint8_t *dst)
{
	uint16_t r = 0;
	for (int i = 0; i < 32; i++) {
		r = r + (uint16_t)src1[i] + (uint16_t)src2[i];
		dst[i] = r & 0xff;
		r >>= 8;
	}
}

int hdnode_private_ckd_cardano(HDNode *inout, uint32_t index)
{
	// checks for hardened/non-hardened derivation, keysize 32 means we are dealing with public key and thus non-h, keysize 64 is for private key
	int keysize = 32;
	if (index & 0x80000000) {
		keysize = 64;
	}

	CONFIDENTIAL uint8_t data[1 + 64 + 4];
	CONFIDENTIAL uint8_t z[32 + 32];
	CONFIDENTIAL uint8_t priv_key[64];
	CONFIDENTIAL uint8_t res_key[64];

	write_le(data + keysize + 1, index);

	memcpy(priv_key, inout->private_key, 32);
	memcpy(priv_key + 32, inout->private_key_extension, 32);

	if (keysize == 64) { // private derivation
		data[0] = 0;
		memcpy(data + 1, inout->private_key, 32);
		memcpy(data + 1 + 32, inout->private_key_extension, 32);
	} else { // public derivation
		hdnode_fill_public_key(inout);
		data[0] = 2;
		memcpy(data + 1, inout->public_key + 1, 32);
	}

	CONFIDENTIAL HMAC_SHA512_CTX ctx;
	hmac_sha512_Init(&ctx, inout->chain_code, 32);
	hmac_sha512_Update(&ctx, data, 1 + keysize + 4);
	hmac_sha512_Final(&ctx, z);

	CONFIDENTIAL uint8_t zl8[32];
	memset(zl8, 0, 32);

	/* get 8 * Zl */
	scalar_multiply8(z, 28, zl8);
	/* Kl = 8*Zl + parent(K)l */
	scalar_add_256bits(zl8, priv_key, res_key);

	/* Kr = Zr + parent(K)r */
	scalar_add_256bits(z + 32, priv_key + 32, res_key + 32);

	memcpy(inout->private_key, res_key, 32);
	memcpy(inout->private_key_extension, res_key + 32, 32);

	if (keysize == 64) {
		data[0] = 1;
	} else {
		data[0] = 3;
	}
	hmac_sha512_Init(&ctx, inout->chain_code, 32);
	hmac_sha512_Update(&ctx, data, 1 + keysize + 4);
	hmac_sha512_Final(&ctx, z);

	memcpy(inout->chain_code, z + 32, 32);
	inout->depth++;
	inout->child_num = index;
	memzero(inout->public_key, sizeof(inout->public_key));

	// making sure to wipe our memory
	memzero(z, sizeof(z));
	memzero(data, sizeof(data));
	memzero(priv_key, sizeof(priv_key));
	memzero(res_key, sizeof(res_key));
	return 1;
}

int hdnode_from_seed_cardano(const uint8_t *pass, int pass_len, const uint8_t *seed, int seed_len, HDNode *out) {
	CONFIDENTIAL uint8_t secret[96];
	pbkdf2_hmac_sha512(pass, pass_len, seed, seed_len, 4096, secret, 96);
	
	secret[0] &= 248;
	secret[31] &= 31;
	secret[31] |= 64;

	memset(out, 0, sizeof(HDNode));
	out->depth = 0;
	out->child_num = 0;
	out->curve = get_curve_by_name(ED25519_CARDANO_NAME);

	memcpy(out->private_key, secret, 32);
	memcpy(out->private_key_extension, secret + 32, 32);

	out->public_key[0] = 0;
	hdnode_fill_public_key(out);

	memcpy(out->chain_code, secret + 64, 32);
	memzero(secret, sizeof(secret));

	return 1;
}

int hdnode_public_ckd_cp(const ecdsa_curve *curve, const curve_point *parent, const uint8_t *parent_chain_code, uint32_t i, curve_point *child, uint8_t *child_chain_code) {
	uint8_t data[1 + 32 + 4];
	uint8_t I[32 + 32];
	bignum256 c;

	if (i & 0x80000000) { // private derivation
		return 0;
	}

	data[0] = 0x02 | (parent->y.val[0] & 0x01);
	bn_write_be(&parent->x, data + 1);
	write_be(data + 33, i);

	while (true) {
		hmac_sha512(parent_chain_code, 32, data, sizeof(data), I);
		bn_read_be(I, &c);
		if (bn_is_less(&c, &curve->order)) { // < order
			scalar_multiply(curve, &c, child); // b = c * G
			point_add(curve, parent, child);   // b = a + b
			if (!point_is_infinity(child)) {
				if (child_chain_code) {
					memcpy(child_chain_code, I + 32, 32);
				}

				// Wipe all stack data.
				memzero(data, sizeof(data));
				memzero(I, sizeof(I));
				memzero(&c, sizeof(c));
				return 1;
			}
		}

		data[0] = 1;
		memcpy(data + 1, I + 32, 32);
	}
}

int hdnode_public_ckd(HDNode *inout, uint32_t i)
{
	curve_point parent, child;

	if (!ecdsa_read_pubkey(inout->curve->params, inout->public_key, &parent)) {
		return 0;
	}
	if (!hdnode_public_ckd_cp(inout->curve->params, &parent, inout->chain_code, i, &child, inout->chain_code)) {
		return 0;
	}
	memzero(inout->private_key, 32);
	inout->depth++;
	inout->child_num = i;
	inout->public_key[0] = 0x02 | (child.y.val[0] & 0x01);
	bn_write_be(&child.x, inout->public_key + 1);

	// Wipe all stack data.
	memzero(&parent, sizeof(parent));
	memzero(&child, sizeof(child));

	return 1;
}

void hdnode_public_ckd_address_optimized(const curve_point *pub, const uint8_t *chain_code, uint32_t i, uint32_t version, HasherType hasher_pubkey, HasherType hasher_base58, char *addr, int addrsize, int addrformat)
{
	uint8_t child_pubkey[33];
	curve_point b;

	hdnode_public_ckd_cp(&secp256k1, pub, chain_code, i, &b, NULL);
	child_pubkey[0] = 0x02 | (b.y.val[0] & 0x01);
	bn_write_be(&b.x, child_pubkey + 1);

	switch (addrformat) {
		case 1: // Segwit-in-P2SH
			ecdsa_get_address_segwit_p2sh(child_pubkey, version, hasher_pubkey, hasher_base58, addr, addrsize);
			break;
		default: // normal address
			ecdsa_get_address(child_pubkey, version, hasher_pubkey, hasher_base58, addr, addrsize);
			break;
	}
}

void hdnode_get_address_raw(HDNode *node, uint32_t version, uint8_t *addr_raw)
{
	hdnode_fill_public_key(node);
	ecdsa_get_address_raw(node->public_key, version, node->curve->hasher_pubkey, addr_raw);
}

void hdnode_get_address(HDNode *node, uint32_t version, char *addr, int addrsize)
{
	hdnode_fill_public_key(node);
	ecdsa_get_address(node->public_key, version, node->curve->hasher_pubkey, node->curve->hasher_base58, addr, addrsize);
}

void hdnode_fill_public_key(HDNode *node)
{
	if (node->public_key[0] != 0)
		return;

#if USE_BIP32_25519_CURVES
	if (node->curve->params) {
		ecdsa_get_public_key33(node->curve->params, node->private_key, node->public_key);
	} else {
		node->public_key[0] = 1;
		if (node->curve == &ed25519_info) {
			ed25519_publickey(node->private_key, node->public_key + 1);
		} else if (node->curve == &ed25519_blake2b_nano_info) {
			ed25519_publickey_blake2b(node->private_key, node->public_key + 1);
		} else if (node->curve == &ed25519_sha3_info) {
			ed25519_publickey_sha3(node->private_key, node->public_key + 1);
		} else if (node->curve == &ed25519_keccak_info) {
			ed25519_publickey_keccak(node->private_key, node->public_key + 1);
		} else if (node->curve == &curve25519_info) {
			curve25519_scalarmult_basepoint(node->public_key + 1, node->private_key);
		} else if (node->curve == &ed25519_cardano_info) {
			ed25519_publickey_ext(node->private_key, node->private_key_extension, node->public_key + 1);
		}
	}
#else

	ecdsa_get_public_key33(node->curve->params, node->private_key, node->public_key);
#endif
}

int hdnode_get_ethereum_pubkeyhash(const HDNode *node, uint8_t *pubkeyhash)
{
	uint8_t buf[65];
	SHA3_CTX ctx;

	/* get uncompressed public key */
	ecdsa_get_public_key65(node->curve->params, node->private_key, buf);

	/* compute sha3 of x and y coordinate without 04 prefix */
	sha3_256_Init(&ctx);
	sha3_Update(&ctx, buf + 1, 64);
	keccak_Final(&ctx, buf);

	/* result are the least significant 160 bits */
	memcpy(pubkeyhash, buf + 12, 20);

	return 1;
}

int hdnode_get_nem_address(HDNode *node, uint8_t version, char *address) {
	if (node->curve != &ed25519_keccak_info) {
		return 0;
	}

	hdnode_fill_public_key(node);
	return nem_get_address(&node->public_key[1], version, address);
}

int hdnode_get_nem_shared_key(const HDNode *node, const ed25519_public_key peer_public_key, const uint8_t *salt, ed25519_public_key mul, uint8_t *shared_key) {
	if (node->curve != &ed25519_keccak_info) {
		return 0;
	}

	// sizeof(ed25519_public_key) == SHA3_256_DIGEST_LENGTH
	if (mul == NULL) mul = shared_key;

	if (ed25519_scalarmult_keccak(mul, node->private_key, peer_public_key)) {
		return 0;
	}

	for (size_t i = 0; i < 32; i++) {
		shared_key[i] = mul[i] ^ salt[i];
	}

	keccak_256(shared_key, 32, shared_key);
	return 1;
}

int hdnode_nem_encrypt(const HDNode *node, const ed25519_public_key public_key, const uint8_t *iv_immut, const uint8_t *salt, const uint8_t *payload, size_t size, uint8_t *buffer) {
	uint8_t last_block[AES_BLOCK_SIZE];
	uint8_t remainder = size % AES_BLOCK_SIZE;

	// Round down to last whole block
	size -= remainder;
	// Copy old last block
	memcpy(last_block, &payload[size], remainder);
	// Pad new last block with number of missing bytes
	memset(&last_block[remainder], AES_BLOCK_SIZE - remainder, AES_BLOCK_SIZE - remainder);

	// the IV gets mutated, so we make a copy not to touch the original
	uint8_t iv[AES_BLOCK_SIZE];
	memcpy(iv, iv_immut, AES_BLOCK_SIZE);

	uint8_t shared_key[SHA3_256_DIGEST_LENGTH];
	if (!hdnode_get_nem_shared_key(node, public_key, salt, NULL, shared_key)) {
		return 0;
	}

	aes_encrypt_ctx ctx;

	int ret = aes_encrypt_key256(shared_key, &ctx);
	memzero(shared_key, sizeof(shared_key));

	if (ret != EXIT_SUCCESS) {
		return 0;
	}

	if (aes_cbc_encrypt(payload, buffer, size, iv, &ctx) != EXIT_SUCCESS) {
		return 0;
	}

	if (aes_cbc_encrypt(last_block, &buffer[size], sizeof(last_block), iv, &ctx) != EXIT_SUCCESS) {
		return 0;
	}

	return 1;
}

int hdnode_nem_decrypt(const HDNode *node, const ed25519_public_key public_key, uint8_t *iv, const uint8_t *salt, const uint8_t *payload, size_t size, uint8_t *buffer) {
	uint8_t shared_key[SHA3_256_DIGEST_LENGTH];

	if (!hdnode_get_nem_shared_key(node, public_key, salt, NULL, shared_key)) {
		return 0;
	}

	aes_decrypt_ctx ctx;

	int ret = aes_decrypt_key256(shared_key, &ctx);
	memzero(shared_key, sizeof(shared_key));

	if (ret != EXIT_SUCCESS) {
		return 0;
	}

	if (aes_cbc_decrypt(payload, buffer, size, iv, &ctx) != EXIT_SUCCESS) {
		return 0;
	}

	return 1;
}

// msg is a data to be signed
// msg_len is the message length
int hdnode_sign(HDNode *node, const uint8_t *msg, uint32_t msg_len, HasherType hasher_sign, uint8_t *sig, uint8_t *pby, int (*is_canonical)(uint8_t by, uint8_t sig[64]))
{
	if (node->curve->params) {
		return ecdsa_sign(node->curve->params, hasher_sign, node->private_key, msg, msg_len, sig, pby, is_canonical);
        } else {
            hdnode_fill_public_key(node);
            if (node->curve == &ed25519_info) {
                ed25519_sign(msg, msg_len, node->private_key, node->public_key + 1, sig);
		} else if (node->curve == &ed25519_blake2b_nano_info) {
			ed25519_sign_blake2b(msg, msg_len, node->private_key, node->public_key + 1, sig);
		} else if (node->curve == &ed25519_sha3_info) {
			ed25519_sign_sha3(msg, msg_len, node->private_key, node->public_key + 1, sig);
		} else if (node->curve == &ed25519_keccak_info) {
			ed25519_sign_keccak(msg, msg_len, node->private_key, node->public_key + 1, sig);
                } else if (node->curve == &curve25519_info) {
                    uint8_t ed25519_public_key[32];
                    memset(ed25519_public_key, 0, 32);
                    curve25519_pk_to_ed25519(ed25519_public_key, node->public_key + 1);
                    ed25519_sign(msg, msg_len, node->private_key, ed25519_public_key, sig);
                    const uint8_t sign_bit = ed25519_public_key[31] & 0x80;
                    sig[63] = sig[63] & 127;
                    sig[63] |= sign_bit;
                }
                return 0;
        }
}

int hdnode_sign_digest(HDNode *node, const uint8_t *digest, uint8_t *sig, uint8_t *pby, int (*is_canonical)(uint8_t by, uint8_t sig[64]))
{
	if (node->curve->params) {
		return ecdsa_sign_digest(node->curve->params, node->private_key, digest, sig, pby, is_canonical);
	} else {
		return hdnode_sign(node, digest, 32, 0, sig, pby, is_canonical);
	}
}

int hdnode_get_shared_key(const HDNode *node, const uint8_t *peer_public_key, uint8_t *session_key, int *result_size)
{
	// Use elliptic curve Diffie-Helman to compute shared session key
	if (node->curve->params) {
		if (ecdh_multiply(node->curve->params, node->private_key, peer_public_key, session_key) != 0) {
			return 1;
		}
		*result_size = 65;
		return 0;
	} else if (node->curve == &curve25519_info) {
		session_key[0] = 0x04;
		if (peer_public_key[0] != 0x40) {
			return 1;  // Curve25519 public key should start with 0x40 byte.
		}
		curve25519_scalarmult(session_key + 1, node->private_key, peer_public_key + 1);
		*result_size = 33;
		return 0;
	} else {
		*result_size = 0;
		return 1;  // ECDH is not supported
	}
}

static int hdnode_serialize(const HDNode *node, uint32_t fingerprint, uint32_t version, char use_public, char *str, int strsize)
{
	uint8_t node_data[78];
	write_be(node_data, version);
	node_data[4] = node->depth;
	write_be(node_data + 5, fingerprint);
	write_be(node_data + 9, node->child_num);
	memcpy(node_data + 13, node->chain_code, 32);
	if (use_public) {
		memcpy(node_data + 45, node->public_key, 33);
	} else {
		node_data[45] = 0;
		memcpy(node_data + 46, node->private_key, 32);
	}
	int ret = base58_encode_check(node_data, sizeof(node_data), node->curve->hasher_base58, str, strsize);
	memzero(node_data, sizeof(node_data));
	return ret;
}

int hdnode_serialize_public(const HDNode *node, uint32_t fingerprint, uint32_t version, char *str, int strsize)
{
	return hdnode_serialize(node, fingerprint, version, 1, str, strsize);
}

int hdnode_serialize_private(const HDNode *node, uint32_t fingerprint, uint32_t version, char *str, int strsize)
{
	return hdnode_serialize(node, fingerprint, version, 0, str, strsize);
}

// check for validity of curve point in case of public data not performed
int hdnode_deserialize(const char *str, uint32_t version_public, uint32_t version_private, const char *curve, HDNode *node, uint32_t *fingerprint)
{
	uint8_t node_data[78];
	memset(node, 0, sizeof(HDNode));
	node->curve = get_curve_by_name(curve);
	if (base58_decode_check(str, node->curve->hasher_base58, node_data, sizeof(node_data)) != sizeof(node_data)) {
		return -1;
	}
	uint32_t version = read_be(node_data);
	if (version == version_public) {
		memzero(node->private_key, sizeof(node->private_key));
		memcpy(node->public_key, node_data + 45, 33);
	} else if (version == version_private) { // private node
		if (node_data[45]) { // invalid data
			return -2;
		}
		memcpy(node->private_key, node_data + 46, 32);
		memzero(node->public_key, sizeof(node->public_key));
	} else {
		return -3; // invalid version
	}
	node->depth = node_data[4];
	if (fingerprint) {
		*fingerprint = read_be(node_data + 5);
	}
	node->child_num = read_be(node_data + 9);
	memcpy(node->chain_code, node_data + 13, 32);
	return 0;
}

const curve_info *get_curve_by_name(const char *curve_name) {
	if (curve_name == 0) {
		return 0;
	}
	if (strcmp(curve_name, SECP256K1_NAME) == 0) {
		return &secp256k1_info;
	}
	if (strcmp(curve_name, SECP256K1_DECRED_NAME) == 0) {
		return &secp256k1_decred_info;
	}
	if (strcmp(curve_name, SECP256K1_SMART_NAME) == 0) {
		return &secp256k1_smart_info;
	}
	if (strcmp(curve_name, NIST256P1_NAME) == 0) {
		return &nist256p1_info;
	}
	if (strcmp(curve_name, ED25519_NAME) == 0) {
		return &ed25519_info;
	}
	if (strcmp(curve_name, ED25519_CARDANO_NAME) == 0) {
		return &ed25519_cardano_info;
	}
	if (strcmp(curve_name, ED25519_BLAKE2B_NANO_NAME) == 0) {
		return &ed25519_blake2b_nano_info;
	}
	if (strcmp(curve_name, ED25519_SHA3_NAME) == 0) {
		return &ed25519_sha3_info;
	}
	if (strcmp(curve_name, ED25519_KECCAK_NAME) == 0) {
		return &ed25519_keccak_info;
	}
	if (strcmp(curve_name, CURVE25519_NAME) == 0) {
		return &curve25519_info;
	}
	return 0;
}
