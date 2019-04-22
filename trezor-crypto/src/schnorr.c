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
#include <TrezorCrypto/memzero.h>
#include "stdio.h"

 static void hex_to_raw_bignum(const char *str, uint8_t bn_raw[32]) {
  for (size_t i = 0; i < 32; i++) {
    uint8_t c = 0;
    if (str[i * 2] >= '0' && str[i * 2] <= '9') c += (str[i * 2] - '0') << 4;
    if ((str[i * 2] & ~0x20) >= 'A' && (str[i * 2] & ~0x20) <= 'F')
      c += (10 + (str[i * 2] & ~0x20) - 'A') << 4;
    if (str[i * 2 + 1] >= '0' && str[i * 2 + 1] <= '9')
      c += (str[i * 2 + 1] - '0');
    if ((str[i * 2 + 1] & ~0x20) >= 'A' && (str[i * 2 + 1] & ~0x20) <= 'F')
      c += (10 + (str[i * 2 + 1] & ~0x20) - 'A');
    bn_raw[i] = c;
  }
}

static void bn_be_to_hex_string(const bignum256 *b, char result[64]) {
  uint8_t raw_number[32] = {0};
  bn_write_be(b, raw_number);
  for (int i = 0; i < 32; ++i)
    sprintf(result + i * 2, "%02x", ((unsigned char *)raw_number)[i]);
}

void schnorr_to_hex_str(const schnorr_sign_pair *sign, char hex_str[128]) {
  bn_be_to_hex_string(&sign->r, hex_str);
  bn_be_to_hex_string(&sign->s, hex_str + 64);
}

void schnorr_from_hex_str(const char hex_str[128], schnorr_sign_pair *sign) {
  uint8_t buf[32];
  hex_to_raw_bignum(hex_str, buf);
  bn_read_be(buf, &sign->r);
  hex_to_raw_bignum(hex_str + 64, buf);
  bn_read_be(buf, &sign->s);
}

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
  bn_read_be(digest, r);
}

// returns 0 if signing succeeded
int schnorr_sign(const ecdsa_curve *curve, const uint8_t *priv_key,
                 const bignum256 *k, const uint8_t *msg, const uint32_t msg_len,
                 schnorr_sign_pair *result) {
  bignum256 private_key_scalar;
  bn_read_be(priv_key, &private_key_scalar);
  uint8_t pub_key[33];
  ecdsa_get_public_key33(curve, priv_key, pub_key);

  /* Q = kG */
  curve_point Q;
  scalar_multiply(curve, k, &Q);

  /* r = H(Q, kpub, m) */
  calc_r(&Q, pub_key, msg, msg_len, &result->r);

  /* s = k - r*kpriv mod(order) */
  bignum256 s_temp;
  bn_copy(&result->r, &s_temp);
  bn_multiply(&private_key_scalar, &s_temp, &curve->order);
  bn_subtractmod(k, &s_temp, &result->s, &curve->order);
  memzero(&private_key_scalar, sizeof(private_key_scalar));

  while (bn_is_less(&curve->order, &result->s)) {
    bn_mod(&result->s, &curve->order);
  }

  if (bn_is_zero(&result->s) || bn_is_zero(&result->r)) {
    return 1;
  }

  return 0;
}

// returns 0 if verification succeeded
int schnorr_verify(const ecdsa_curve *curve, const uint8_t *pub_key,
                   const uint8_t *msg, const uint32_t msg_len,
                   const schnorr_sign_pair *sign) {
  if (msg_len == 0) return 1;
  if (bn_is_zero(&sign->r)) return 2;
  if (bn_is_zero(&sign->s)) return 3;
  if (bn_is_less(&curve->order, &sign->r)) return 4;
  if (bn_is_less(&curve->order, &sign->s)) return 5;

  curve_point pub_key_point;
  if (!ecdsa_read_pubkey(curve, pub_key, &pub_key_point)) {
    return 6;
  }

  // Compute Q = sG + r*kpub
  curve_point sG, Q;
  scalar_multiply(curve, &sign->s, &sG);
  point_multiply(curve, &sign->r, &pub_key_point, &Q);
  point_add(curve, &sG, &Q);

  /* r = H(Q, kpub, m) */
  bignum256 r;
  calc_r(&Q, pub_key, msg, msg_len, &r);

  if (bn_is_equal(&r, &sign->r)) return 0;  // success

  return 10;
}
