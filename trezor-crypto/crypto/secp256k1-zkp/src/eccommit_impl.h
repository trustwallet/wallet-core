/**********************************************************************
 * Copyright (c) 2020 The libsecp256k1 Developers                     *
 * Distributed under the MIT software license, see the accompanying   *
 * file COPYING or http://www.opensource.org/licenses/mit-license.php.*
 **********************************************************************/

#include <stddef.h>

#include "eckey.h"
#include "hash.h"

/* from secp256k1.c */
static int secp256k1_ec_seckey_tweak_add_helper(secp256k1_scalar *sec, const unsigned char *tweak);
static int secp256k1_ec_pubkey_tweak_add_helper(const secp256k1_ecmult_context* ecmult_ctx, secp256k1_ge *pubp, const unsigned char *tweak);

static int secp256k1_ec_commit_pubkey_serialize_const(secp256k1_ge *pubp, unsigned char *buf33) {
    if (secp256k1_ge_is_infinity(pubp)) {
        return 0;
    }
    secp256k1_fe_normalize(&pubp->x);
    secp256k1_fe_normalize(&pubp->y);
    secp256k1_fe_get_b32(&buf33[1], &pubp->x);
    buf33[0] = secp256k1_fe_is_odd(&pubp->y) ? SECP256K1_TAG_PUBKEY_ODD : SECP256K1_TAG_PUBKEY_EVEN;
    return 1;
}

/* Compute an ec commitment tweak as hash(pubp, data). */
static int secp256k1_ec_commit_tweak(unsigned char *tweak32, secp256k1_ge* pubp, secp256k1_sha256* sha, const unsigned char *data, size_t data_size)
{
    unsigned char rbuf[33];

    if (!secp256k1_ec_commit_pubkey_serialize_const(pubp, rbuf)) {
        return 0;
    }
    secp256k1_sha256_write(sha, rbuf, sizeof(rbuf));
    secp256k1_sha256_write(sha, data, data_size);
    secp256k1_sha256_finalize(sha, tweak32);
    return 1;
}

/* Compute an ec commitment as pubp + hash(pubp, data)*G. */
static int secp256k1_ec_commit(const secp256k1_ecmult_context* ecmult_ctx, secp256k1_ge* commitp, const secp256k1_ge* pubp, secp256k1_sha256* sha, const unsigned char *data, size_t data_size) {
    unsigned char tweak[32];

    *commitp = *pubp;
    return secp256k1_ec_commit_tweak(tweak, commitp, sha, data, data_size)
           && secp256k1_ec_pubkey_tweak_add_helper(ecmult_ctx, commitp, tweak);
}

/* Compute the seckey of an ec commitment from the original secret key of the pubkey as seckey +
 * hash(pubp, data). */
static int secp256k1_ec_commit_seckey(secp256k1_scalar* seckey, secp256k1_ge* pubp, secp256k1_sha256* sha, const unsigned char *data, size_t data_size) {
    unsigned char tweak[32];
    return secp256k1_ec_commit_tweak(tweak, pubp, sha, data, data_size)
           && secp256k1_ec_seckey_tweak_add_helper(seckey, tweak);
}

/* Verify an ec commitment as pubp + hash(pubp, data)*G ?= commitment. */
static int secp256k1_ec_commit_verify(const secp256k1_ecmult_context* ecmult_ctx, const secp256k1_ge* commitp, const secp256k1_ge* pubp, secp256k1_sha256* sha, const unsigned char *data, size_t data_size) {
    secp256k1_gej pj;
    secp256k1_ge p;

    if (!secp256k1_ec_commit(ecmult_ctx, &p, pubp, sha, data, data_size)) {
        return 0;
    }

    /* Return p == commitp */
    secp256k1_ge_neg(&p, &p);
    secp256k1_gej_set_ge(&pj, &p);
    secp256k1_gej_add_ge_var(&pj, &pj, commitp, NULL);
    return secp256k1_gej_is_infinity(&pj);
}

