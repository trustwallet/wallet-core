/**
 * Copyright (c) 2019 Anatolii Kurotych
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

#include <TrezorCrypto/schnorr.h>

// r = H(Q, kpub, m)
static void calc_r(const curve_point *Q, const uint8_t pub_key[33],
                   const uint8_t *msg, const uint32_t msg_len, bignum256 *r) {
  uint8_t Q_compress[33];
  compress_coords(Q, Q_compress);

  SHA256_CTX ctx;
  uint8_t digest[SHA256_DIGEST_LENGTH];
  sha256_Init(&ctx);
  sha256_Update(&ctx, Q_compress, 33);
  sha256_Update(&ctx, pub_key, 33);
  sha256_Update(&ctx, msg, msg_len);
  sha256_Final(&ctx, digest);

  // Convert the raw bigendian 256 bit value to a normalized, partly reduced bignum
  bn_read_be(digest, r);
}

// Returns 0 if signing succeeded
int schnorr_sign(const ecdsa_curve *curve, const uint8_t *priv_key,
                 const bignum256 *k, const uint8_t *msg, const uint32_t msg_len,
                 schnorr_sign_pair *result) {
  uint8_t pub_key[33];
  curve_point Q;
  bignum256 private_key_scalar;
  bignum256 r_temp;
  bignum256 s_temp;
  bignum256 r_kpriv_result;

  bn_read_be(priv_key, &private_key_scalar);
  ecdsa_get_public_key33(curve, priv_key, pub_key);

  // Compute commitment Q = kG
  point_multiply(curve, k, &curve->G, &Q);

  // Compute challenge r = H(Q, kpub, m)
  calc_r(&Q, pub_key, msg, msg_len, &r_temp);
  
  // Fully reduce the bignum
  bn_mod(&r_temp, &curve->order);

  // Convert the normalized, fully reduced bignum to a raw bigendian 256 bit value
  bn_write_be(&r_temp, result->r);

  // Compute s = k - r*kpriv
  bn_copy(&r_temp, &r_kpriv_result);

  // r*kpriv result is partly reduced
  bn_multiply(&private_key_scalar, &r_kpriv_result, &curve->order);

  // k - r*kpriv result is normalized but not reduced
  bn_subtractmod(k, &r_kpriv_result, &s_temp, &curve->order);

  // Partly reduce the result
  bn_fast_mod(&s_temp, &curve->order);

  // Fully reduce the result
  bn_mod(&s_temp, &curve->order);

  // Convert the normalized, fully reduced bignum to a raw bigendian 256 bit value
  bn_write_be(&s_temp, result->s);

  if (bn_is_zero(&r_temp) || bn_is_zero(&s_temp)) return 1;

  return 0;
}

// Returns 0 if verification succeeded
int schnorr_verify(const ecdsa_curve *curve, const uint8_t *pub_key,
                   const uint8_t *msg, const uint32_t msg_len,
                   const schnorr_sign_pair *sign) {
  curve_point pub_key_point;
  curve_point sG, Q;
  bignum256 r_temp;
  bignum256 s_temp;
  bignum256 r_computed;

  if (msg_len == 0) return 1;

  // Convert the raw bigendian 256 bit values to normalized, partly reduced bignums
  bn_read_be(sign->r, &r_temp);
  bn_read_be(sign->s, &s_temp);

  // Check if r,s are in [1, ..., order-1]
  if (bn_is_zero(&r_temp)) return 2;
  if (bn_is_zero(&s_temp)) return 3;
  if (bn_is_less(&curve->order, &r_temp)) return 4;
  if (bn_is_less(&curve->order, &s_temp)) return 5;
  if (bn_is_equal(&curve->order, &r_temp)) return 6;
  if (bn_is_equal(&curve->order, &s_temp)) return 7;

  if (!ecdsa_read_pubkey(curve, pub_key, &pub_key_point)) {
    return 8;
  }

  // Compute Q = sG + r*kpub
  point_multiply(curve, &s_temp, &curve->G, &sG);
  point_multiply(curve, &r_temp, &pub_key_point, &Q);
  point_add(curve, &sG, &Q);

  // Compute r' = H(Q, kpub, m)
  calc_r(&Q, pub_key, msg, msg_len, &r_computed);

  // Fully reduce the bignum
  bn_mod(&r_computed, &curve->order);

  // Check r == r'
  if (bn_is_equal(&r_temp, &r_computed)) return 0;  // success

  return 10;
}
