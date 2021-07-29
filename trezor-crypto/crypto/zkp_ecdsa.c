#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include <TrezorCrypto/memzero.h>
#include <TrezorCrypto/secp256k1.h>
#include "zkp_context.h"

#include "secp256k1-zkp/include/secp256k1.h"
#include "secp256k1-zkp/include/secp256k1_extrakeys.h"
#include "secp256k1-zkp/include/secp256k1_preallocated.h"
#include "secp256k1-zkp/include/secp256k1_recovery.h"

#include "zkp_ecdsa.h"

// ECDSA compressed public key derivation
// curve has to be &secp256k1
// private_key_bytes has 32 bytes
// public_key_bytes has 32 bytes
void zkp_ecdsa_get_public_key33(const ecdsa_curve *curve,
                                const uint8_t *private_key_bytes,
                                uint8_t *public_key_bytes) {
  assert(curve == &secp256k1);
  int result = 0;

  secp256k1_pubkey public_key = {0};

  if (result == 0) {
    secp256k1_context *context_writeable = zkp_context_acquire_writeable();
    secp256k1_context_writeable_randomize(context_writeable);
    if (secp256k1_ec_pubkey_create(context_writeable, &public_key,
                                   private_key_bytes) != 1) {
      result = 1;
    }
    zkp_context_release_writeable();
  }

  if (result == 0) {
    size_t written = 33;
    const secp256k1_context *context_read_only = zkp_context_get_read_only();
    int returned = secp256k1_ec_pubkey_serialize(
        context_read_only, public_key_bytes, &written, &public_key,
        SECP256K1_EC_COMPRESSED);

    if (returned != 1 || written != 33) {
      result = 1;
    }
  }

  memzero(&public_key, sizeof(public_key));
  assert(result == 0);
}

// ECDSA uncompressed public key derivation
// curve has to be &secp256k1
// private_key_bytes has 32 bytes
// public_key_bytes has 65 bytes
void zkp_ecdsa_get_public_key65(const ecdsa_curve *curve,
                                const uint8_t *private_key_bytes,
                                uint8_t *public_key_bytes) {
  assert(curve == &secp256k1);
  int result = 0;

  secp256k1_pubkey public_key = {0};

  if (result == 0) {
    secp256k1_context *context_writeable = zkp_context_acquire_writeable();
    secp256k1_context_writeable_randomize(context_writeable);
    if (secp256k1_ec_pubkey_create(context_writeable, &public_key,
                                   private_key_bytes) != 1) {
      result = 1;
    }
    zkp_context_release_writeable();
  }

  if (result == 0) {
    size_t written = 65;
    const secp256k1_context *context_read_only = zkp_context_get_read_only();
    int returned = secp256k1_ec_pubkey_serialize(
        context_read_only, public_key_bytes, &written, &public_key,
        SECP256K1_EC_UNCOMPRESSED);

    if (returned != 1 || written != 65) {
      result = 1;
    }
  }

  memzero(&public_key, sizeof(public_key));
  assert(result == 0);
}

// ECDSA signing
// curve has to be &secp256k1
// private_key_bytes has 32 bytes
// digest has 32 bytes
// signature_bytes has 64 bytes
// pby is one byte
// is_canonical has to be NULL
int zkp_ecdsa_sign_digest(
    const ecdsa_curve *curve, const uint8_t *private_key_bytes,
    const uint8_t *digest, uint8_t *signature_bytes, uint8_t *pby,
    int (*is_canonical)(uint8_t by, uint8_t signature_bytes[64])) {
  assert(curve == &secp256k1);
  assert(is_canonical == NULL);
  int result = 0;

  secp256k1_ecdsa_recoverable_signature recoverable_signature = {0};

  if (result == 0) {
    secp256k1_context *ctx_writeable = zkp_context_acquire_writeable();
    secp256k1_context_writeable_randomize(ctx_writeable);
    if (secp256k1_ecdsa_sign_recoverable(ctx_writeable, &recoverable_signature,
                                         digest, private_key_bytes, NULL,
                                         NULL) != 1) {
      result = 1;
    }
    zkp_context_release_writeable();
  }

  if (result == 0) {
    int recid = 0;
    const secp256k1_context *context_read_only = zkp_context_get_read_only();
    if (secp256k1_ecdsa_recoverable_signature_serialize_compact(
            context_read_only, signature_bytes, &recid,
            &recoverable_signature) != 1) {
      result = 1;
    }
    if (pby != NULL) {
      *pby = (uint8_t)recid;
    }
  }

  memzero(&recoverable_signature, sizeof(recoverable_signature));

  return result;
}

// ECDSA public key recovery
// public_key_bytes has 64 bytes
// signature_bytes has 64 bytes
// digest has 32 bytes
// recid is 0, 1, 2 or 3
int zkp_ecdsa_recover_pub_from_sig(const ecdsa_curve *curve,
                                   uint8_t *public_key_bytes,
                                   const uint8_t *signature_bytes,
                                   const uint8_t *digest, int recid) {
  assert(curve == &secp256k1);
  int result = 0;

  const secp256k1_context *context_read_only = zkp_context_get_read_only();
  secp256k1_ecdsa_recoverable_signature recoverable_signature = {0};

  if (result == 0) {
    if (secp256k1_ecdsa_recoverable_signature_parse_compact(
            context_read_only, &recoverable_signature, signature_bytes,
            recid) != 1) {
      result = 1;
    }
  }

  secp256k1_pubkey public_key = {0};

  if (result == 0) {
    if (secp256k1_ecdsa_recover(context_read_only, &public_key,
                                &recoverable_signature, digest) != 1) {
      result = 1;
    }
  }

  memzero(&recoverable_signature, sizeof(recoverable_signature));

  if (result == 0) {
    size_t written = 65;
    int returned = secp256k1_ec_pubkey_serialize(
        context_read_only, public_key_bytes, &written, &public_key,
        SECP256K1_EC_UNCOMPRESSED);
    if (returned != 1 || written != 65) {
      result = 1;
    }
  }

  memzero(&public_key, sizeof(public_key));

  return result;
}

static bool is_zero_digest(const uint8_t *digest) {
  const uint8_t zeroes[32] = {0};
  return memcmp(digest, zeroes, 32) == 0;
}

// ECDSA verification
// curve has to be &secp256k1
// public_key_bytes has 33 or 65 bytes
// signature_bytes has 64 bytes
// digest has 32 bytes
int zkp_ecdsa_verify_digest(const ecdsa_curve *curve,
                            const uint8_t *public_key_bytes,
                            const uint8_t *signature_bytes,
                            const uint8_t *digest) {
  assert(curve == &secp256k1);
  int result = 0;

  int public_key_length = 0;

  if (result == 0) {
    if (public_key_bytes[0] == 0x04) {
      public_key_length = 65;
    } else if (public_key_bytes[0] == 0x02 || public_key_bytes[0] == 0x03) {
      public_key_length = 33;
    } else {
      result = 1;
    }
  }

  if (result == 0) {
    if (is_zero_digest(digest)) {
      result = 3;
    }
  }

  const secp256k1_context *context_read_only = zkp_context_get_read_only();
  secp256k1_pubkey public_key = {0};

  if (result == 0) {
    if (secp256k1_ec_pubkey_parse(context_read_only, &public_key,
                                  public_key_bytes, public_key_length) != 1) {
      result = 1;
    }
  }

  secp256k1_ecdsa_signature signature = {0};

  if (result == 0) {
    if (secp256k1_ecdsa_signature_parse_compact(context_read_only, &signature,
                                                signature_bytes) != 1) {
      result = 2;
    }
  }

  if (result == 0) {
    secp256k1_ecdsa_signature_normalize(context_read_only, &signature,
                                        &signature);

    if (secp256k1_ecdsa_verify(context_read_only, &signature, digest,
                               &public_key) != 1) {
      result = 5;
    }
  }

  memzero(&public_key, sizeof(public_key));
  memzero(&signature, sizeof(signature));

  return result;
}

// ECDSA verification
// curve has to be &secp256k1
// public_key_bytes has 33 or 65 bytes
// signature_bytes has 64 bytes
// returns 0 if verification succeeded
int zkp_ecdsa_verify(const ecdsa_curve *curve, HasherType hasher_type,
                     const uint8_t *public_key_bytes,
                     const uint8_t *signature_bytes, const uint8_t *message,
                     uint32_t message_length) {
  uint8_t hash[32] = {0};
  hasher_Raw(hasher_type, message, message_length, hash);
  int result =
      zkp_ecdsa_verify_digest(curve, public_key_bytes, signature_bytes, hash);
  memzero(hash, sizeof(hash));
  return result;
}
