#ifndef __KEYPAIR_H__
#define __KEYPAIR_H__

#include <string.h>

#include "private/ed25519_ref10.h"

#ifdef __cplusplus
extern "C" {
#endif

int ed25519_pk_to_curve25519(unsigned char *curve25519_pk, const unsigned char *ed25519_pk);

int curve25519_pk_to_ed25519(unsigned char *ed25519_pk, const unsigned char *curve25519_pk);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif