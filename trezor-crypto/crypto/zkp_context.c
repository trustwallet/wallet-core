#include <assert.h>
#include <stdatomic.h>
#include <stdbool.h>

#include <TrezorCrypto/memzero.h>
#include <TrezorCrypto/rand.h>
#include <TrezorCrypto/zkp_context.h>

#include "secp256k1-zkp/include/secp256k1.h"

static uint8_t context_buffer[SECP256K1_CONTEXT_SIZE];
static secp256k1_context *context;
static volatile atomic_flag locked;
static bool initialized = false;

void secp256k1_context_writeable_randomize(
    secp256k1_context *context_writeable) {
  uint8_t seed[32] = {0};
  random_buffer(seed, sizeof(seed));
  int returned = secp256k1_context_randomize(context_writeable, seed);
  memzero(seed, sizeof(seed));
  assert(returned == 1);
}

bool zkp_context_is_initialized(void) { return initialized; }

void zkp_context_init() {
  assert(initialized == false);

  const unsigned int context_flags =
      SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY;

  const size_t context_size =
      secp256k1_context_preallocated_size(context_flags);
  assert(context_size != 0);
  assert(context_size <= SECP256K1_CONTEXT_SIZE);

  context =
      secp256k1_context_preallocated_create(context_buffer, context_flags);
  assert(context != NULL);

  secp256k1_context_writeable_randomize(context);

  atomic_flag_clear(&locked);
  initialized = true;
}

void zkp_context_destroy() {
  assert(initialized == true);

  initialized = false;
  secp256k1_context_preallocated_destroy(context);
  memzero(context_buffer, sizeof(context_buffer));
  atomic_flag_clear(&locked);
}

const secp256k1_context *zkp_context_get_read_only() {
  assert(initialized == true);

  return context;
}

secp256k1_context *zkp_context_acquire_writeable() {
  assert(initialized == true);

  // We don't expect the context to be used by multiple threads
  assert(!atomic_flag_test_and_set(&locked));
  return context;
}

void zkp_context_release_writeable() {
  assert(initialized == true);

  atomic_flag_clear(&locked);
}
