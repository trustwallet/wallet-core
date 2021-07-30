#ifndef __ZKP_CONTEXT_H__
#define __ZKP_CONTEXT_H__

#include <stdint.h>

#include "../../crypto/secp256k1-zkp/include/secp256k1_preallocated.h"

#ifdef __cplusplus
extern "C" {
#endif

void secp256k1_context_writeable_randomize(secp256k1_context *context);
void zkp_context_init(void);
void zkp_context_destroy(void);
const secp256k1_context *zkp_context_get_read_only(void);
secp256k1_context *zkp_context_acquire_writeable(void);
void zkp_context_release_writeable(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
