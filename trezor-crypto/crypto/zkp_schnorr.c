#include <stdbool.h>
#include <string.h>

#include <TrezorCrypto/memzero.h>
#include "zkp_context.h"

#include "secp256k1-zkp/include/secp256k1.h"
#include "secp256k1-zkp/include/secp256k1_extrakeys.h"
#include "secp256k1-zkp/include/secp256k1_schnorrsig.h"

#include "zkp_schnorr.h"

static bool is_zero_digest(const uint8_t *digest) {
  const uint8_t zeroes[32] = {0};
  return memcmp(digest, zeroes, 32) == 0;
}

// BIP340 Schnorr public key derivation
// private_key_bytes has 32 bytes
// public_key_bytes has 32 bytes
int zkp_schnorr_get_public_key(const uint8_t *private_key_bytes,
                               uint8_t *public_key_bytes) {
  int result = 0;

  secp256k1_keypair keypair = {0};

  if (result == 0) {
    secp256k1_context *context_writeable = zkp_context_acquire_writeable();
    secp256k1_context_writeable_randomize(context_writeable);
    if (secp256k1_keypair_create(context_writeable, &keypair,
                                 private_key_bytes) != 1) {
      result = -1;
    }
    zkp_context_release_writeable();
  }

  secp256k1_xonly_pubkey xonly_pubkey = {0};
  const secp256k1_context *context_read_only = zkp_context_get_read_only();

  if (result == 0) {
    if (secp256k1_keypair_xonly_pub(context_read_only, &xonly_pubkey, NULL,
                                    &keypair) != 1) {
      result = -1;
    }
  }

  memzero(&keypair, sizeof(keypair));

  if (result == 0) {
    if (secp256k1_xonly_pubkey_serialize(context_read_only, public_key_bytes,
                                         &xonly_pubkey) != 1) {
      result = -1;
    }
  }

  memzero(&xonly_pubkey, sizeof(xonly_pubkey));

  return result;
}

// BIP340 Schnorr signature signing
// private_key_bytes has 32 bytes
// digest has 32 bytes
// signature_bytes has 64 bytes
// auxiliary_data has 32 bytes or is NULL
int zkp_schnorr_sign_digest(const uint8_t *private_key_bytes,
                            const uint8_t *digest, uint8_t *signature_bytes,
                            uint8_t *auxiliary_data) {
  int result = 0;

  secp256k1_keypair keypair = {0};

  if (result == 0) {
    secp256k1_context *context_writeable = zkp_context_acquire_writeable();
    secp256k1_context_writeable_randomize(context_writeable);
    if (secp256k1_keypair_create(context_writeable, &keypair,
                                 private_key_bytes) != 1) {
      result = -1;
    }
    zkp_context_release_writeable();
  }

  if (result == 0) {
    secp256k1_context *context_writeable = zkp_context_acquire_writeable();
    secp256k1_context_writeable_randomize(context_writeable);
    if (secp256k1_schnorrsig_sign(context_writeable, signature_bytes, digest,
                                  &keypair, NULL, auxiliary_data) != 1) {
      result = -1;
    }
    zkp_context_release_writeable();
  }

  memzero(&keypair, sizeof(keypair));

  return result;
}

// BIP340 Schnorr signature verification
// public_key_bytes has 32 bytes
// signature_bytes has 64 bytes
// digest has 32 bytes
// returns 0 if verification succeeded
int zkp_schnorr_verify_digest(const uint8_t *public_key_bytes,
                              const uint8_t *signature_bytes,
                              const uint8_t *digest) {
  int result = 0;

  if (is_zero_digest(digest)) {
    result = 3;
  }

  secp256k1_xonly_pubkey xonly_pubkey = {0};
  const secp256k1_context *context_read_only = zkp_context_get_read_only();

  if (result == 0) {
    if (secp256k1_xonly_pubkey_parse(context_read_only, &xonly_pubkey,
                                     public_key_bytes) != 1) {
      result = 1;
    }
  }

  if (result == 0) {
    if (secp256k1_schnorrsig_verify(context_read_only, signature_bytes, digest,
                                    &xonly_pubkey) != 1) {
      result = 5;
    }
  }

  memzero(&xonly_pubkey, sizeof(xonly_pubkey));

  return result;
}
