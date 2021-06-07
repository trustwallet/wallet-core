//
// Created by Dusan Klinec on 10/05/2018.
//

#ifndef TREZOR_CRYPTO_RANGE_PROOF_H
#define TREZOR_CRYPTO_RANGE_PROOF_H

#include "xmr.h"
#define XMR_ATOMS 64

typedef uint64_t xmr_amount;
typedef xmr_key_t xmr_key64_t[64];

typedef struct xmr_boro_sig {
  xmr_key64_t s0;
  xmr_key64_t s1;
  xmr_key_t ee;
} xmr_boro_sig_t;

typedef struct range_sig {
  xmr_boro_sig_t asig;
  xmr_key64_t Ci;
} xmr_range_sig_t;

void xmr_gen_range_sig(xmr_range_sig_t* sig, ge25519* C, bignum256modm mask,
                       xmr_amount amount, bignum256modm* last_mask);
void xmr_gen_range_sig_ex(xmr_range_sig_t* sig, ge25519* C, bignum256modm mask,
                          xmr_amount amount, bignum256modm* last_mask,
                          bignum256modm ai[64], bignum256modm alpha[64]);

#endif  // TREZOR_CRYPTO_RANGE_PROOF_H
