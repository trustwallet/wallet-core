// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "crypto_ops.h"

static int64_t signum(int64_t a) {
    return a > 0 ? 1 : a < 0 ? -1 : 0;
}

uint64_t load_3(const unsigned char *in) {
    uint64_t result;
    result = (uint64_t) in[0];
    result |= ((uint64_t) in[1]) << 8;
    result |= ((uint64_t) in[2]) << 16;
    return result;
}

uint64_t load_4(const unsigned char *in)
{
    uint64_t result;
    result = (uint64_t) in[0];
    result |= ((uint64_t) in[1]) << 8;
    result |= ((uint64_t) in[2]) << 16;
    result |= ((uint64_t) in[3]) << 24;
    return result;
}

void sc_reduce32(unsigned char *s) {
    int64_t s0 = 2097151 & load_3(s);
    int64_t s1 = 2097151 & (load_4(s + 2) >> 5);
    int64_t s2 = 2097151 & (load_3(s + 5) >> 2);
    int64_t s3 = 2097151 & (load_4(s + 7) >> 7);
    int64_t s4 = 2097151 & (load_4(s + 10) >> 4);
    int64_t s5 = 2097151 & (load_3(s + 13) >> 1);
    int64_t s6 = 2097151 & (load_4(s + 15) >> 6);
    int64_t s7 = 2097151 & (load_3(s + 18) >> 3);
    int64_t s8 = 2097151 & load_3(s + 21);
    int64_t s9 = 2097151 & (load_4(s + 23) >> 5);
    int64_t s10 = 2097151 & (load_3(s + 26) >> 2);
    int64_t s11 = (load_4(s + 28) >> 7);
    int64_t s12 = 0;
    int64_t carry0;
    int64_t carry1;
    int64_t carry2;
    int64_t carry3;
    int64_t carry4;
    int64_t carry5;
    int64_t carry6;
    int64_t carry7;
    int64_t carry8;
    int64_t carry9;
    int64_t carry10;
    int64_t carry11;

    carry0 = (s0 + (1<<20)) >> 21; s1 += carry0; s0 -= carry0 << 21;
    carry2 = (s2 + (1<<20)) >> 21; s3 += carry2; s2 -= carry2 << 21;
    carry4 = (s4 + (1<<20)) >> 21; s5 += carry4; s4 -= carry4 << 21;
    carry6 = (s6 + (1<<20)) >> 21; s7 += carry6; s6 -= carry6 << 21;
    carry8 = (s8 + (1<<20)) >> 21; s9 += carry8; s8 -= carry8 << 21;
    carry10 = (s10 + (1<<20)) >> 21; s11 += carry10; s10 -= carry10 << 21;

    carry1 = (s1 + (1<<20)) >> 21; s2 += carry1; s1 -= carry1 << 21;
    carry3 = (s3 + (1<<20)) >> 21; s4 += carry3; s3 -= carry3 << 21;
    carry5 = (s5 + (1<<20)) >> 21; s6 += carry5; s5 -= carry5 << 21;
    carry7 = (s7 + (1<<20)) >> 21; s8 += carry7; s7 -= carry7 << 21;
    carry9 = (s9 + (1<<20)) >> 21; s10 += carry9; s9 -= carry9 << 21;
    carry11 = (s11 + (1<<20)) >> 21; s12 += carry11; s11 -= carry11 << 21;

    s0 += s12 * 666643;
    s1 += s12 * 470296;
    s2 += s12 * 654183;
    s3 -= s12 * 997805;
    s4 += s12 * 136657;
    s5 -= s12 * 683901;
    s12 = 0;

    carry0 = s0 >> 21; s1 += carry0; s0 -= carry0 << 21;
    carry1 = s1 >> 21; s2 += carry1; s1 -= carry1 << 21;
    carry2 = s2 >> 21; s3 += carry2; s2 -= carry2 << 21;
    carry3 = s3 >> 21; s4 += carry3; s3 -= carry3 << 21;
    carry4 = s4 >> 21; s5 += carry4; s4 -= carry4 << 21;
    carry5 = s5 >> 21; s6 += carry5; s5 -= carry5 << 21;
    carry6 = s6 >> 21; s7 += carry6; s6 -= carry6 << 21;
    carry7 = s7 >> 21; s8 += carry7; s7 -= carry7 << 21;
    carry8 = s8 >> 21; s9 += carry8; s8 -= carry8 << 21;
    carry9 = s9 >> 21; s10 += carry9; s9 -= carry9 << 21;
    carry10 = s10 >> 21; s11 += carry10; s10 -= carry10 << 21;
    carry11 = s11 >> 21; s12 += carry11; s11 -= carry11 << 21;

    s0 += s12 * 666643;
    s1 += s12 * 470296;
    s2 += s12 * 654183;
    s3 -= s12 * 997805;
    s4 += s12 * 136657;
    s5 -= s12 * 683901;

    carry0 = s0 >> 21; s1 += carry0; s0 -= carry0 << 21;
    carry1 = s1 >> 21; s2 += carry1; s1 -= carry1 << 21;
    carry2 = s2 >> 21; s3 += carry2; s2 -= carry2 << 21;
    carry3 = s3 >> 21; s4 += carry3; s3 -= carry3 << 21;
    carry4 = s4 >> 21; s5 += carry4; s4 -= carry4 << 21;
    carry5 = s5 >> 21; s6 += carry5; s5 -= carry5 << 21;
    carry6 = s6 >> 21; s7 += carry6; s6 -= carry6 << 21;
    carry7 = s7 >> 21; s8 += carry7; s7 -= carry7 << 21;
    carry8 = s8 >> 21; s9 += carry8; s8 -= carry8 << 21;
    carry9 = s9 >> 21; s10 += carry9; s9 -= carry9 << 21;
    carry10 = s10 >> 21; s11 += carry10; s10 -= carry10 << 21;

    s[0] = s0 >> 0;
    s[1] = s0 >> 8;
    s[2] = (s0 >> 16) | (s1 << 5);
    s[3] = s1 >> 3;
    s[4] = s1 >> 11;
    s[5] = (s1 >> 19) | (s2 << 2);
    s[6] = s2 >> 6;
    s[7] = (s2 >> 14) | (s3 << 7);
    s[8] = s3 >> 1;
    s[9] = s3 >> 9;
    s[10] = (s3 >> 17) | (s4 << 4);
    s[11] = s4 >> 4;
    s[12] = s4 >> 12;
    s[13] = (s4 >> 20) | (s5 << 1);
    s[14] = s5 >> 7;
    s[15] = (s5 >> 15) | (s6 << 6);
    s[16] = s6 >> 2;
    s[17] = s6 >> 10;
    s[18] = (s6 >> 18) | (s7 << 3);
    s[19] = s7 >> 5;
    s[20] = s7 >> 13;
    s[21] = s8 >> 0;
    s[22] = s8 >> 8;
    s[23] = (s8 >> 16) | (s9 << 5);
    s[24] = s9 >> 3;
    s[25] = s9 >> 11;
    s[26] = (s9 >> 19) | (s10 << 2);
    s[27] = s10 >> 6;
    s[28] = (s10 >> 14) | (s11 << 7);
    s[29] = s11 >> 1;
    s[30] = s11 >> 9;
    s[31] = s11 >> 17;
}

int sc_check(const unsigned char *s) {
    int64_t s0 = load_4(s);
    int64_t s1 = load_4(s + 4);
    int64_t s2 = load_4(s + 8);
    int64_t s3 = load_4(s + 12);
    int64_t s4 = load_4(s + 16);
    int64_t s5 = load_4(s + 20);
    int64_t s6 = load_4(s + 24);
    int64_t s7 = load_4(s + 28);
    return (signum(1559614444 - s0) + (signum(1477600026 - s1) << 1) + (signum(2734136534 - s2) << 2) + (signum(350157278 - s3) << 3) + (signum(-s4) << 4) + (signum(-s5) << 5) + (signum(-s6) << 6) + (signum(268435456 - s7) << 7)) >> 8;
}

void ge_scalarmult_base(ge_p3 *h, const unsigned char *a) {
    signed char e[64];
    signed char carry;
    ge_p1p1 r;
    ge_p2 s;
    ge_precomp t;
    int i;

    for (i = 0; i < 32; ++i) {
        e[2 * i + 0] = (a[i] >> 0) & 15;
        e[2 * i + 1] = (a[i] >> 4) & 15;
    }
    /* each e[i] is between 0 and 15 */
    /* e[63] is between 0 and 7 */

    carry = 0;
    for (i = 0; i < 63; ++i) {
        e[i] += carry;
        carry = e[i] + 8;
        carry >>= 4;
        e[i] -= carry << 4;
    }
    e[63] += carry;
    /* each e[i] is between -8 and 8 */

    ge_p3_0(h);
    for (i = 1; i < 64; i += 2) {
        select(&t, i / 2, e[i]);
        ge_madd(&r, h, &t); ge_p1p1_to_p3(h, &r);
    }

    ge_p3_dbl(&r, h);  ge_p1p1_to_p2(&s, &r);
    ge_p2_dbl(&r, &s); ge_p1p1_to_p2(&s, &r);
    ge_p2_dbl(&r, &s); ge_p1p1_to_p2(&s, &r);
    ge_p2_dbl(&r, &s); ge_p1p1_to_p3(h, &r);

    for (i = 0; i < 64; i += 2) {
        select(&t, i / 2, e[i]);
        ge_madd(&r, h, &t); ge_p1p1_to_p3(h, &r);
    }
}

void ge_scalarmult(ge_p2 *r, const unsigned char *a, const ge_p3 *A) {
    signed char e[64];
    int carry, carry2, i;
    ge_cached Ai[8]; /* 1 * A, 2 * A, ..., 8 * A */
    ge_p1p1 t;
    ge_p3 u;

    carry = 0; /* 0..1 */
    for (i = 0; i < 31; i++) {
        carry += a[i]; /* 0..256 */
        carry2 = (carry + 8) >> 4; /* 0..16 */
        e[2 * i] = carry - (carry2 << 4); /* -8..7 */
        carry = (carry2 + 8) >> 4; /* 0..1 */
        e[2 * i + 1] = carry2 - (carry << 4); /* -8..7 */
    }
    carry += a[31]; /* 0..128 */
    carry2 = (carry + 8) >> 4; /* 0..8 */
    e[62] = carry - (carry2 << 4); /* -8..7 */
    e[63] = carry2; /* 0..8 */

    ge_p3_to_cached(&Ai[0], A);
    for (i = 0; i < 7; i++) {
        ge_add(&t, A, &Ai[i]);
        ge_p1p1_to_p3(&u, &t);
        ge_p3_to_cached(&Ai[i + 1], &u);
    }

    ge_p2_0(r);
    for (i = 63; i >= 0; i--) {
        signed char b = e[i];
        unsigned char bnegative = negative(b);
        unsigned char babs = b - (((-bnegative) & b) << 1);
        ge_cached cur, minuscur;
        ge_p2_dbl(&t, r);
        ge_p1p1_to_p2(r, &t);
        ge_p2_dbl(&t, r);
        ge_p1p1_to_p2(r, &t);
        ge_p2_dbl(&t, r);
        ge_p1p1_to_p2(r, &t);
        ge_p2_dbl(&t, r);
        ge_p1p1_to_p3(&u, &t);
        ge_cached_0(&cur);
        ge_cached_cmov(&cur, &Ai[0], equal(babs, 1));
        ge_cached_cmov(&cur, &Ai[1], equal(babs, 2));
        ge_cached_cmov(&cur, &Ai[2], equal(babs, 3));
        ge_cached_cmov(&cur, &Ai[3], equal(babs, 4));
        ge_cached_cmov(&cur, &Ai[4], equal(babs, 5));
        ge_cached_cmov(&cur, &Ai[5], equal(babs, 6));
        ge_cached_cmov(&cur, &Ai[6], equal(babs, 7));
        ge_cached_cmov(&cur, &Ai[7], equal(babs, 8));
        fe_copy(minuscur.YplusX, cur.YminusX);
        fe_copy(minuscur.YminusX, cur.YplusX);
        fe_copy(minuscur.Z, cur.Z);
        fe_neg(minuscur.T2d, cur.T2d);
        ge_cached_cmov(&cur, &minuscur, bnegative);
        ge_add(&t, &u, &cur);
        ge_p1p1_to_p2(r, &t);
    }
}

void fe_0(fe h)
{
    h[0] = 0;
    h[1] = 0;
    h[2] = 0;
    h[3] = 0;
    h[4] = 0;
    h[5] = 0;
    h[6] = 0;
    h[7] = 0;
    h[8] = 0;
    h[9] = 0;
}

void fe_1(fe h)
{
    h[0] = 1;
    h[1] = 0;
    h[2] = 0;
    h[3] = 0;
    h[4] = 0;
    h[5] = 0;
    h[6] = 0;
    h[7] = 0;
    h[8] = 0;
    h[9] = 0;
}

void fe_add(fe h,const fe f,const fe g)
{
    crypto_int32 f0 = f[0];
    crypto_int32 f1 = f[1];
    crypto_int32 f2 = f[2];
    crypto_int32 f3 = f[3];
    crypto_int32 f4 = f[4];
    crypto_int32 f5 = f[5];
    crypto_int32 f6 = f[6];
    crypto_int32 f7 = f[7];
    crypto_int32 f8 = f[8];
    crypto_int32 f9 = f[9];
    crypto_int32 g0 = g[0];
    crypto_int32 g1 = g[1];
    crypto_int32 g2 = g[2];
    crypto_int32 g3 = g[3];
    crypto_int32 g4 = g[4];
    crypto_int32 g5 = g[5];
    crypto_int32 g6 = g[6];
    crypto_int32 g7 = g[7];
    crypto_int32 g8 = g[8];
    crypto_int32 g9 = g[9];
    crypto_int32 h0 = f0 + g0;
    crypto_int32 h1 = f1 + g1;
    crypto_int32 h2 = f2 + g2;
    crypto_int32 h3 = f3 + g3;
    crypto_int32 h4 = f4 + g4;
    crypto_int32 h5 = f5 + g5;
    crypto_int32 h6 = f6 + g6;
    crypto_int32 h7 = f7 + g7;
    crypto_int32 h8 = f8 + g8;
    crypto_int32 h9 = f9 + g9;
    h[0] = h0;
    h[1] = h1;
    h[2] = h2;
    h[3] = h3;
    h[4] = h4;
    h[5] = h5;
    h[6] = h6;
    h[7] = h7;
    h[8] = h8;
    h[9] = h9;
}

void fe_sub(fe h,const fe f,const fe g)
{
    crypto_int32 f0 = f[0];
    crypto_int32 f1 = f[1];
    crypto_int32 f2 = f[2];
    crypto_int32 f3 = f[3];
    crypto_int32 f4 = f[4];
    crypto_int32 f5 = f[5];
    crypto_int32 f6 = f[6];
    crypto_int32 f7 = f[7];
    crypto_int32 f8 = f[8];
    crypto_int32 f9 = f[9];
    crypto_int32 g0 = g[0];
    crypto_int32 g1 = g[1];
    crypto_int32 g2 = g[2];
    crypto_int32 g3 = g[3];
    crypto_int32 g4 = g[4];
    crypto_int32 g5 = g[5];
    crypto_int32 g6 = g[6];
    crypto_int32 g7 = g[7];
    crypto_int32 g8 = g[8];
    crypto_int32 g9 = g[9];
    crypto_int32 h0 = f0 - g0;
    crypto_int32 h1 = f1 - g1;
    crypto_int32 h2 = f2 - g2;
    crypto_int32 h3 = f3 - g3;
    crypto_int32 h4 = f4 - g4;
    crypto_int32 h5 = f5 - g5;
    crypto_int32 h6 = f6 - g6;
    crypto_int32 h7 = f7 - g7;
    crypto_int32 h8 = f8 - g8;
    crypto_int32 h9 = f9 - g9;
    h[0] = h0;
    h[1] = h1;
    h[2] = h2;
    h[3] = h3;
    h[4] = h4;
    h[5] = h5;
    h[6] = h6;
    h[7] = h7;
    h[8] = h8;
    h[9] = h9;
}

void fe_mul(fe h,const fe f,const fe g)
{
    crypto_int32 f0 = f[0];
    crypto_int32 f1 = f[1];
    crypto_int32 f2 = f[2];
    crypto_int32 f3 = f[3];
    crypto_int32 f4 = f[4];
    crypto_int32 f5 = f[5];
    crypto_int32 f6 = f[6];
    crypto_int32 f7 = f[7];
    crypto_int32 f8 = f[8];
    crypto_int32 f9 = f[9];
    crypto_int32 g0 = g[0];
    crypto_int32 g1 = g[1];
    crypto_int32 g2 = g[2];
    crypto_int32 g3 = g[3];
    crypto_int32 g4 = g[4];
    crypto_int32 g5 = g[5];
    crypto_int32 g6 = g[6];
    crypto_int32 g7 = g[7];
    crypto_int32 g8 = g[8];
    crypto_int32 g9 = g[9];
    crypto_int32 g1_19 = 19 * g1; /* 1.959375*2^29 */
    crypto_int32 g2_19 = 19 * g2; /* 1.959375*2^30; still ok */
    crypto_int32 g3_19 = 19 * g3;
    crypto_int32 g4_19 = 19 * g4;
    crypto_int32 g5_19 = 19 * g5;
    crypto_int32 g6_19 = 19 * g6;
    crypto_int32 g7_19 = 19 * g7;
    crypto_int32 g8_19 = 19 * g8;
    crypto_int32 g9_19 = 19 * g9;
    crypto_int32 f1_2 = 2 * f1;
    crypto_int32 f3_2 = 2 * f3;
    crypto_int32 f5_2 = 2 * f5;
    crypto_int32 f7_2 = 2 * f7;
    crypto_int32 f9_2 = 2 * f9;
    crypto_int64 f0g0    = f0   * (crypto_int64) g0;
    crypto_int64 f0g1    = f0   * (crypto_int64) g1;
    crypto_int64 f0g2    = f0   * (crypto_int64) g2;
    crypto_int64 f0g3    = f0   * (crypto_int64) g3;
    crypto_int64 f0g4    = f0   * (crypto_int64) g4;
    crypto_int64 f0g5    = f0   * (crypto_int64) g5;
    crypto_int64 f0g6    = f0   * (crypto_int64) g6;
    crypto_int64 f0g7    = f0   * (crypto_int64) g7;
    crypto_int64 f0g8    = f0   * (crypto_int64) g8;
    crypto_int64 f0g9    = f0   * (crypto_int64) g9;
    crypto_int64 f1g0    = f1   * (crypto_int64) g0;
    crypto_int64 f1g1_2  = f1_2 * (crypto_int64) g1;
    crypto_int64 f1g2    = f1   * (crypto_int64) g2;
    crypto_int64 f1g3_2  = f1_2 * (crypto_int64) g3;
    crypto_int64 f1g4    = f1   * (crypto_int64) g4;
    crypto_int64 f1g5_2  = f1_2 * (crypto_int64) g5;
    crypto_int64 f1g6    = f1   * (crypto_int64) g6;
    crypto_int64 f1g7_2  = f1_2 * (crypto_int64) g7;
    crypto_int64 f1g8    = f1   * (crypto_int64) g8;
    crypto_int64 f1g9_38 = f1_2 * (crypto_int64) g9_19;
    crypto_int64 f2g0    = f2   * (crypto_int64) g0;
    crypto_int64 f2g1    = f2   * (crypto_int64) g1;
    crypto_int64 f2g2    = f2   * (crypto_int64) g2;
    crypto_int64 f2g3    = f2   * (crypto_int64) g3;
    crypto_int64 f2g4    = f2   * (crypto_int64) g4;
    crypto_int64 f2g5    = f2   * (crypto_int64) g5;
    crypto_int64 f2g6    = f2   * (crypto_int64) g6;
    crypto_int64 f2g7    = f2   * (crypto_int64) g7;
    crypto_int64 f2g8_19 = f2   * (crypto_int64) g8_19;
    crypto_int64 f2g9_19 = f2   * (crypto_int64) g9_19;
    crypto_int64 f3g0    = f3   * (crypto_int64) g0;
    crypto_int64 f3g1_2  = f3_2 * (crypto_int64) g1;
    crypto_int64 f3g2    = f3   * (crypto_int64) g2;
    crypto_int64 f3g3_2  = f3_2 * (crypto_int64) g3;
    crypto_int64 f3g4    = f3   * (crypto_int64) g4;
    crypto_int64 f3g5_2  = f3_2 * (crypto_int64) g5;
    crypto_int64 f3g6    = f3   * (crypto_int64) g6;
    crypto_int64 f3g7_38 = f3_2 * (crypto_int64) g7_19;
    crypto_int64 f3g8_19 = f3   * (crypto_int64) g8_19;
    crypto_int64 f3g9_38 = f3_2 * (crypto_int64) g9_19;
    crypto_int64 f4g0    = f4   * (crypto_int64) g0;
    crypto_int64 f4g1    = f4   * (crypto_int64) g1;
    crypto_int64 f4g2    = f4   * (crypto_int64) g2;
    crypto_int64 f4g3    = f4   * (crypto_int64) g3;
    crypto_int64 f4g4    = f4   * (crypto_int64) g4;
    crypto_int64 f4g5    = f4   * (crypto_int64) g5;
    crypto_int64 f4g6_19 = f4   * (crypto_int64) g6_19;
    crypto_int64 f4g7_19 = f4   * (crypto_int64) g7_19;
    crypto_int64 f4g8_19 = f4   * (crypto_int64) g8_19;
    crypto_int64 f4g9_19 = f4   * (crypto_int64) g9_19;
    crypto_int64 f5g0    = f5   * (crypto_int64) g0;
    crypto_int64 f5g1_2  = f5_2 * (crypto_int64) g1;
    crypto_int64 f5g2    = f5   * (crypto_int64) g2;
    crypto_int64 f5g3_2  = f5_2 * (crypto_int64) g3;
    crypto_int64 f5g4    = f5   * (crypto_int64) g4;
    crypto_int64 f5g5_38 = f5_2 * (crypto_int64) g5_19;
    crypto_int64 f5g6_19 = f5   * (crypto_int64) g6_19;
    crypto_int64 f5g7_38 = f5_2 * (crypto_int64) g7_19;
    crypto_int64 f5g8_19 = f5   * (crypto_int64) g8_19;
    crypto_int64 f5g9_38 = f5_2 * (crypto_int64) g9_19;
    crypto_int64 f6g0    = f6   * (crypto_int64) g0;
    crypto_int64 f6g1    = f6   * (crypto_int64) g1;
    crypto_int64 f6g2    = f6   * (crypto_int64) g2;
    crypto_int64 f6g3    = f6   * (crypto_int64) g3;
    crypto_int64 f6g4_19 = f6   * (crypto_int64) g4_19;
    crypto_int64 f6g5_19 = f6   * (crypto_int64) g5_19;
    crypto_int64 f6g6_19 = f6   * (crypto_int64) g6_19;
    crypto_int64 f6g7_19 = f6   * (crypto_int64) g7_19;
    crypto_int64 f6g8_19 = f6   * (crypto_int64) g8_19;
    crypto_int64 f6g9_19 = f6   * (crypto_int64) g9_19;
    crypto_int64 f7g0    = f7   * (crypto_int64) g0;
    crypto_int64 f7g1_2  = f7_2 * (crypto_int64) g1;
    crypto_int64 f7g2    = f7   * (crypto_int64) g2;
    crypto_int64 f7g3_38 = f7_2 * (crypto_int64) g3_19;
    crypto_int64 f7g4_19 = f7   * (crypto_int64) g4_19;
    crypto_int64 f7g5_38 = f7_2 * (crypto_int64) g5_19;
    crypto_int64 f7g6_19 = f7   * (crypto_int64) g6_19;
    crypto_int64 f7g7_38 = f7_2 * (crypto_int64) g7_19;
    crypto_int64 f7g8_19 = f7   * (crypto_int64) g8_19;
    crypto_int64 f7g9_38 = f7_2 * (crypto_int64) g9_19;
    crypto_int64 f8g0    = f8   * (crypto_int64) g0;
    crypto_int64 f8g1    = f8   * (crypto_int64) g1;
    crypto_int64 f8g2_19 = f8   * (crypto_int64) g2_19;
    crypto_int64 f8g3_19 = f8   * (crypto_int64) g3_19;
    crypto_int64 f8g4_19 = f8   * (crypto_int64) g4_19;
    crypto_int64 f8g5_19 = f8   * (crypto_int64) g5_19;
    crypto_int64 f8g6_19 = f8   * (crypto_int64) g6_19;
    crypto_int64 f8g7_19 = f8   * (crypto_int64) g7_19;
    crypto_int64 f8g8_19 = f8   * (crypto_int64) g8_19;
    crypto_int64 f8g9_19 = f8   * (crypto_int64) g9_19;
    crypto_int64 f9g0    = f9   * (crypto_int64) g0;
    crypto_int64 f9g1_38 = f9_2 * (crypto_int64) g1_19;
    crypto_int64 f9g2_19 = f9   * (crypto_int64) g2_19;
    crypto_int64 f9g3_38 = f9_2 * (crypto_int64) g3_19;
    crypto_int64 f9g4_19 = f9   * (crypto_int64) g4_19;
    crypto_int64 f9g5_38 = f9_2 * (crypto_int64) g5_19;
    crypto_int64 f9g6_19 = f9   * (crypto_int64) g6_19;
    crypto_int64 f9g7_38 = f9_2 * (crypto_int64) g7_19;
    crypto_int64 f9g8_19 = f9   * (crypto_int64) g8_19;
    crypto_int64 f9g9_38 = f9_2 * (crypto_int64) g9_19;
    crypto_int64 h0 = f0g0+f1g9_38+f2g8_19+f3g7_38+f4g6_19+f5g5_38+f6g4_19+f7g3_38+f8g2_19+f9g1_38;
    crypto_int64 h1 = f0g1+f1g0   +f2g9_19+f3g8_19+f4g7_19+f5g6_19+f6g5_19+f7g4_19+f8g3_19+f9g2_19;
    crypto_int64 h2 = f0g2+f1g1_2 +f2g0   +f3g9_38+f4g8_19+f5g7_38+f6g6_19+f7g5_38+f8g4_19+f9g3_38;
    crypto_int64 h3 = f0g3+f1g2   +f2g1   +f3g0   +f4g9_19+f5g8_19+f6g7_19+f7g6_19+f8g5_19+f9g4_19;
    crypto_int64 h4 = f0g4+f1g3_2 +f2g2   +f3g1_2 +f4g0   +f5g9_38+f6g8_19+f7g7_38+f8g6_19+f9g5_38;
    crypto_int64 h5 = f0g5+f1g4   +f2g3   +f3g2   +f4g1   +f5g0   +f6g9_19+f7g8_19+f8g7_19+f9g6_19;
    crypto_int64 h6 = f0g6+f1g5_2 +f2g4   +f3g3_2 +f4g2   +f5g1_2 +f6g0   +f7g9_38+f8g8_19+f9g7_38;
    crypto_int64 h7 = f0g7+f1g6   +f2g5   +f3g4   +f4g3   +f5g2   +f6g1   +f7g0   +f8g9_19+f9g8_19;
    crypto_int64 h8 = f0g8+f1g7_2 +f2g6   +f3g5_2 +f4g4   +f5g3_2 +f6g2   +f7g1_2 +f8g0   +f9g9_38;
    crypto_int64 h9 = f0g9+f1g8   +f2g7   +f3g6   +f4g5   +f5g4   +f6g3   +f7g2   +f8g1   +f9g0   ;
    crypto_int64 carry0;
    crypto_int64 carry1;
    crypto_int64 carry2;
    crypto_int64 carry3;
    crypto_int64 carry4;
    crypto_int64 carry5;
    crypto_int64 carry6;
    crypto_int64 carry7;
    crypto_int64 carry8;
    crypto_int64 carry9;

    /*
    |h0| <= (1.65*1.65*2^52*(1+19+19+19+19)+1.65*1.65*2^50*(38+38+38+38+38))
      i.e. |h0| <= 1.4*2^60; narrower ranges for h2, h4, h6, h8
    |h1| <= (1.65*1.65*2^51*(1+1+19+19+19+19+19+19+19+19))
      i.e. |h1| <= 1.7*2^59; narrower ranges for h3, h5, h7, h9
    */

    carry0 = (h0 + (crypto_int64) (1<<25)) >> 26; h1 += carry0; h0 -= carry0 << 26;
    carry4 = (h4 + (crypto_int64) (1<<25)) >> 26; h5 += carry4; h4 -= carry4 << 26;
    /* |h0| <= 2^25 */
    /* |h4| <= 2^25 */
    /* |h1| <= 1.71*2^59 */
    /* |h5| <= 1.71*2^59 */

    carry1 = (h1 + (crypto_int64) (1<<24)) >> 25; h2 += carry1; h1 -= carry1 << 25;
    carry5 = (h5 + (crypto_int64) (1<<24)) >> 25; h6 += carry5; h5 -= carry5 << 25;
    /* |h1| <= 2^24; from now on fits into int32 */
    /* |h5| <= 2^24; from now on fits into int32 */
    /* |h2| <= 1.41*2^60 */
    /* |h6| <= 1.41*2^60 */

    carry2 = (h2 + (crypto_int64) (1<<25)) >> 26; h3 += carry2; h2 -= carry2 << 26;
    carry6 = (h6 + (crypto_int64) (1<<25)) >> 26; h7 += carry6; h6 -= carry6 << 26;
    /* |h2| <= 2^25; from now on fits into int32 unchanged */
    /* |h6| <= 2^25; from now on fits into int32 unchanged */
    /* |h3| <= 1.71*2^59 */
    /* |h7| <= 1.71*2^59 */

    carry3 = (h3 + (crypto_int64) (1<<24)) >> 25; h4 += carry3; h3 -= carry3 << 25;
    carry7 = (h7 + (crypto_int64) (1<<24)) >> 25; h8 += carry7; h7 -= carry7 << 25;
    /* |h3| <= 2^24; from now on fits into int32 unchanged */
    /* |h7| <= 2^24; from now on fits into int32 unchanged */
    /* |h4| <= 1.72*2^34 */
    /* |h8| <= 1.41*2^60 */

    carry4 = (h4 + (crypto_int64) (1<<25)) >> 26; h5 += carry4; h4 -= carry4 << 26;
    carry8 = (h8 + (crypto_int64) (1<<25)) >> 26; h9 += carry8; h8 -= carry8 << 26;
    /* |h4| <= 2^25; from now on fits into int32 unchanged */
    /* |h8| <= 2^25; from now on fits into int32 unchanged */
    /* |h5| <= 1.01*2^24 */
    /* |h9| <= 1.71*2^59 */

    carry9 = (h9 + (crypto_int64) (1<<24)) >> 25; h0 += carry9 * 19; h9 -= carry9 << 25;
    /* |h9| <= 2^24; from now on fits into int32 unchanged */
    /* |h0| <= 1.1*2^39 */

    carry0 = (h0 + (crypto_int64) (1<<25)) >> 26; h1 += carry0; h0 -= carry0 << 26;
    /* |h0| <= 2^25; from now on fits into int32 unchanged */
    /* |h1| <= 1.01*2^24 */

    h[0] = h0;
    h[1] = h1;
    h[2] = h2;
    h[3] = h3;
    h[4] = h4;
    h[5] = h5;
    h[6] = h6;
    h[7] = h7;
    h[8] = h8;
    h[9] = h9;
}

void fe_cmov(fe f,const fe g,unsigned int b)
{
    crypto_int32 f0 = f[0];
    crypto_int32 f1 = f[1];
    crypto_int32 f2 = f[2];
    crypto_int32 f3 = f[3];
    crypto_int32 f4 = f[4];
    crypto_int32 f5 = f[5];
    crypto_int32 f6 = f[6];
    crypto_int32 f7 = f[7];
    crypto_int32 f8 = f[8];
    crypto_int32 f9 = f[9];
    crypto_int32 g0 = g[0];
    crypto_int32 g1 = g[1];
    crypto_int32 g2 = g[2];
    crypto_int32 g3 = g[3];
    crypto_int32 g4 = g[4];
    crypto_int32 g5 = g[5];
    crypto_int32 g6 = g[6];
    crypto_int32 g7 = g[7];
    crypto_int32 g8 = g[8];
    crypto_int32 g9 = g[9];
    crypto_int32 x0 = f0 ^ g0;
    crypto_int32 x1 = f1 ^ g1;
    crypto_int32 x2 = f2 ^ g2;
    crypto_int32 x3 = f3 ^ g3;
    crypto_int32 x4 = f4 ^ g4;
    crypto_int32 x5 = f5 ^ g5;
    crypto_int32 x6 = f6 ^ g6;
    crypto_int32 x7 = f7 ^ g7;
    crypto_int32 x8 = f8 ^ g8;
    crypto_int32 x9 = f9 ^ g9;
    b = -b;
    x0 &= b;
    x1 &= b;
    x2 &= b;
    x3 &= b;
    x4 &= b;
    x5 &= b;
    x6 &= b;
    x7 &= b;
    x8 &= b;
    x9 &= b;
    f[0] = f0 ^ x0;
    f[1] = f1 ^ x1;
    f[2] = f2 ^ x2;
    f[3] = f3 ^ x3;
    f[4] = f4 ^ x4;
    f[5] = f5 ^ x5;
    f[6] = f6 ^ x6;
    f[7] = f7 ^ x7;
    f[8] = f8 ^ x8;
    f[9] = f9 ^ x9;
}

void fe_copy(fe h,const fe f)
{
    crypto_int32 f0 = f[0];
    crypto_int32 f1 = f[1];
    crypto_int32 f2 = f[2];
    crypto_int32 f3 = f[3];
    crypto_int32 f4 = f[4];
    crypto_int32 f5 = f[5];
    crypto_int32 f6 = f[6];
    crypto_int32 f7 = f[7];
    crypto_int32 f8 = f[8];
    crypto_int32 f9 = f[9];
    h[0] = f0;
    h[1] = f1;
    h[2] = f2;
    h[3] = f3;
    h[4] = f4;
    h[5] = f5;
    h[6] = f6;
    h[7] = f7;
    h[8] = f8;
    h[9] = f9;
}

void fe_neg(fe h,const fe f)
{
    crypto_int32 f0 = f[0];
    crypto_int32 f1 = f[1];
    crypto_int32 f2 = f[2];
    crypto_int32 f3 = f[3];
    crypto_int32 f4 = f[4];
    crypto_int32 f5 = f[5];
    crypto_int32 f6 = f[6];
    crypto_int32 f7 = f[7];
    crypto_int32 f8 = f[8];
    crypto_int32 f9 = f[9];
    crypto_int32 h0 = -f0;
    crypto_int32 h1 = -f1;
    crypto_int32 h2 = -f2;
    crypto_int32 h3 = -f3;
    crypto_int32 h4 = -f4;
    crypto_int32 h5 = -f5;
    crypto_int32 h6 = -f6;
    crypto_int32 h7 = -f7;
    crypto_int32 h8 = -f8;
    crypto_int32 h9 = -f9;
    h[0] = h0;
    h[1] = h1;
    h[2] = h2;
    h[3] = h3;
    h[4] = h4;
    h[5] = h5;
    h[6] = h6;
    h[7] = h7;
    h[8] = h8;
    h[9] = h9;
}

void fe_sq(fe h,const fe f)
{
    crypto_int32 f0 = f[0];
    crypto_int32 f1 = f[1];
    crypto_int32 f2 = f[2];
    crypto_int32 f3 = f[3];
    crypto_int32 f4 = f[4];
    crypto_int32 f5 = f[5];
    crypto_int32 f6 = f[6];
    crypto_int32 f7 = f[7];
    crypto_int32 f8 = f[8];
    crypto_int32 f9 = f[9];
    crypto_int32 f0_2 = 2 * f0;
    crypto_int32 f1_2 = 2 * f1;
    crypto_int32 f2_2 = 2 * f2;
    crypto_int32 f3_2 = 2 * f3;
    crypto_int32 f4_2 = 2 * f4;
    crypto_int32 f5_2 = 2 * f5;
    crypto_int32 f6_2 = 2 * f6;
    crypto_int32 f7_2 = 2 * f7;
    crypto_int32 f5_38 = 38 * f5; /* 1.959375*2^30 */
    crypto_int32 f6_19 = 19 * f6; /* 1.959375*2^30 */
    crypto_int32 f7_38 = 38 * f7; /* 1.959375*2^30 */
    crypto_int32 f8_19 = 19 * f8; /* 1.959375*2^30 */
    crypto_int32 f9_38 = 38 * f9; /* 1.959375*2^30 */
    crypto_int64 f0f0    = f0   * (crypto_int64) f0;
    crypto_int64 f0f1_2  = f0_2 * (crypto_int64) f1;
    crypto_int64 f0f2_2  = f0_2 * (crypto_int64) f2;
    crypto_int64 f0f3_2  = f0_2 * (crypto_int64) f3;
    crypto_int64 f0f4_2  = f0_2 * (crypto_int64) f4;
    crypto_int64 f0f5_2  = f0_2 * (crypto_int64) f5;
    crypto_int64 f0f6_2  = f0_2 * (crypto_int64) f6;
    crypto_int64 f0f7_2  = f0_2 * (crypto_int64) f7;
    crypto_int64 f0f8_2  = f0_2 * (crypto_int64) f8;
    crypto_int64 f0f9_2  = f0_2 * (crypto_int64) f9;
    crypto_int64 f1f1_2  = f1_2 * (crypto_int64) f1;
    crypto_int64 f1f2_2  = f1_2 * (crypto_int64) f2;
    crypto_int64 f1f3_4  = f1_2 * (crypto_int64) f3_2;
    crypto_int64 f1f4_2  = f1_2 * (crypto_int64) f4;
    crypto_int64 f1f5_4  = f1_2 * (crypto_int64) f5_2;
    crypto_int64 f1f6_2  = f1_2 * (crypto_int64) f6;
    crypto_int64 f1f7_4  = f1_2 * (crypto_int64) f7_2;
    crypto_int64 f1f8_2  = f1_2 * (crypto_int64) f8;
    crypto_int64 f1f9_76 = f1_2 * (crypto_int64) f9_38;
    crypto_int64 f2f2    = f2   * (crypto_int64) f2;
    crypto_int64 f2f3_2  = f2_2 * (crypto_int64) f3;
    crypto_int64 f2f4_2  = f2_2 * (crypto_int64) f4;
    crypto_int64 f2f5_2  = f2_2 * (crypto_int64) f5;
    crypto_int64 f2f6_2  = f2_2 * (crypto_int64) f6;
    crypto_int64 f2f7_2  = f2_2 * (crypto_int64) f7;
    crypto_int64 f2f8_38 = f2_2 * (crypto_int64) f8_19;
    crypto_int64 f2f9_38 = f2   * (crypto_int64) f9_38;
    crypto_int64 f3f3_2  = f3_2 * (crypto_int64) f3;
    crypto_int64 f3f4_2  = f3_2 * (crypto_int64) f4;
    crypto_int64 f3f5_4  = f3_2 * (crypto_int64) f5_2;
    crypto_int64 f3f6_2  = f3_2 * (crypto_int64) f6;
    crypto_int64 f3f7_76 = f3_2 * (crypto_int64) f7_38;
    crypto_int64 f3f8_38 = f3_2 * (crypto_int64) f8_19;
    crypto_int64 f3f9_76 = f3_2 * (crypto_int64) f9_38;
    crypto_int64 f4f4    = f4   * (crypto_int64) f4;
    crypto_int64 f4f5_2  = f4_2 * (crypto_int64) f5;
    crypto_int64 f4f6_38 = f4_2 * (crypto_int64) f6_19;
    crypto_int64 f4f7_38 = f4   * (crypto_int64) f7_38;
    crypto_int64 f4f8_38 = f4_2 * (crypto_int64) f8_19;
    crypto_int64 f4f9_38 = f4   * (crypto_int64) f9_38;
    crypto_int64 f5f5_38 = f5   * (crypto_int64) f5_38;
    crypto_int64 f5f6_38 = f5_2 * (crypto_int64) f6_19;
    crypto_int64 f5f7_76 = f5_2 * (crypto_int64) f7_38;
    crypto_int64 f5f8_38 = f5_2 * (crypto_int64) f8_19;
    crypto_int64 f5f9_76 = f5_2 * (crypto_int64) f9_38;
    crypto_int64 f6f6_19 = f6   * (crypto_int64) f6_19;
    crypto_int64 f6f7_38 = f6   * (crypto_int64) f7_38;
    crypto_int64 f6f8_38 = f6_2 * (crypto_int64) f8_19;
    crypto_int64 f6f9_38 = f6   * (crypto_int64) f9_38;
    crypto_int64 f7f7_38 = f7   * (crypto_int64) f7_38;
    crypto_int64 f7f8_38 = f7_2 * (crypto_int64) f8_19;
    crypto_int64 f7f9_76 = f7_2 * (crypto_int64) f9_38;
    crypto_int64 f8f8_19 = f8   * (crypto_int64) f8_19;
    crypto_int64 f8f9_38 = f8   * (crypto_int64) f9_38;
    crypto_int64 f9f9_38 = f9   * (crypto_int64) f9_38;
    crypto_int64 h0 = f0f0  +f1f9_76+f2f8_38+f3f7_76+f4f6_38+f5f5_38;
    crypto_int64 h1 = f0f1_2+f2f9_38+f3f8_38+f4f7_38+f5f6_38;
    crypto_int64 h2 = f0f2_2+f1f1_2 +f3f9_76+f4f8_38+f5f7_76+f6f6_19;
    crypto_int64 h3 = f0f3_2+f1f2_2 +f4f9_38+f5f8_38+f6f7_38;
    crypto_int64 h4 = f0f4_2+f1f3_4 +f2f2   +f5f9_76+f6f8_38+f7f7_38;
    crypto_int64 h5 = f0f5_2+f1f4_2 +f2f3_2 +f6f9_38+f7f8_38;
    crypto_int64 h6 = f0f6_2+f1f5_4 +f2f4_2 +f3f3_2 +f7f9_76+f8f8_19;
    crypto_int64 h7 = f0f7_2+f1f6_2 +f2f5_2 +f3f4_2 +f8f9_38;
    crypto_int64 h8 = f0f8_2+f1f7_4 +f2f6_2 +f3f5_4 +f4f4   +f9f9_38;
    crypto_int64 h9 = f0f9_2+f1f8_2 +f2f7_2 +f3f6_2 +f4f5_2;
    crypto_int64 carry0;
    crypto_int64 carry1;
    crypto_int64 carry2;
    crypto_int64 carry3;
    crypto_int64 carry4;
    crypto_int64 carry5;
    crypto_int64 carry6;
    crypto_int64 carry7;
    crypto_int64 carry8;
    crypto_int64 carry9;

    carry0 = (h0 + (crypto_int64) (1<<25)) >> 26; h1 += carry0; h0 -= carry0 << 26;
    carry4 = (h4 + (crypto_int64) (1<<25)) >> 26; h5 += carry4; h4 -= carry4 << 26;

    carry1 = (h1 + (crypto_int64) (1<<24)) >> 25; h2 += carry1; h1 -= carry1 << 25;
    carry5 = (h5 + (crypto_int64) (1<<24)) >> 25; h6 += carry5; h5 -= carry5 << 25;

    carry2 = (h2 + (crypto_int64) (1<<25)) >> 26; h3 += carry2; h2 -= carry2 << 26;
    carry6 = (h6 + (crypto_int64) (1<<25)) >> 26; h7 += carry6; h6 -= carry6 << 26;

    carry3 = (h3 + (crypto_int64) (1<<24)) >> 25; h4 += carry3; h3 -= carry3 << 25;
    carry7 = (h7 + (crypto_int64) (1<<24)) >> 25; h8 += carry7; h7 -= carry7 << 25;

    carry4 = (h4 + (crypto_int64) (1<<25)) >> 26; h5 += carry4; h4 -= carry4 << 26;
    carry8 = (h8 + (crypto_int64) (1<<25)) >> 26; h9 += carry8; h8 -= carry8 << 26;

    carry9 = (h9 + (crypto_int64) (1<<24)) >> 25; h0 += carry9 * 19; h9 -= carry9 << 25;

    carry0 = (h0 + (crypto_int64) (1<<25)) >> 26; h1 += carry0; h0 -= carry0 << 26;

    h[0] = h0;
    h[1] = h1;
    h[2] = h2;
    h[3] = h3;
    h[4] = h4;
    h[5] = h5;
    h[6] = h6;
    h[7] = h7;
    h[8] = h8;
    h[9] = h9;
}

void fe_sq2(fe h,const fe f)
{
    crypto_int32 f0 = f[0];
    crypto_int32 f1 = f[1];
    crypto_int32 f2 = f[2];
    crypto_int32 f3 = f[3];
    crypto_int32 f4 = f[4];
    crypto_int32 f5 = f[5];
    crypto_int32 f6 = f[6];
    crypto_int32 f7 = f[7];
    crypto_int32 f8 = f[8];
    crypto_int32 f9 = f[9];
    crypto_int32 f0_2 = 2 * f0;
    crypto_int32 f1_2 = 2 * f1;
    crypto_int32 f2_2 = 2 * f2;
    crypto_int32 f3_2 = 2 * f3;
    crypto_int32 f4_2 = 2 * f4;
    crypto_int32 f5_2 = 2 * f5;
    crypto_int32 f6_2 = 2 * f6;
    crypto_int32 f7_2 = 2 * f7;
    crypto_int32 f5_38 = 38 * f5; /* 1.959375*2^30 */
    crypto_int32 f6_19 = 19 * f6; /* 1.959375*2^30 */
    crypto_int32 f7_38 = 38 * f7; /* 1.959375*2^30 */
    crypto_int32 f8_19 = 19 * f8; /* 1.959375*2^30 */
    crypto_int32 f9_38 = 38 * f9; /* 1.959375*2^30 */
    crypto_int64 f0f0    = f0   * (crypto_int64) f0;
    crypto_int64 f0f1_2  = f0_2 * (crypto_int64) f1;
    crypto_int64 f0f2_2  = f0_2 * (crypto_int64) f2;
    crypto_int64 f0f3_2  = f0_2 * (crypto_int64) f3;
    crypto_int64 f0f4_2  = f0_2 * (crypto_int64) f4;
    crypto_int64 f0f5_2  = f0_2 * (crypto_int64) f5;
    crypto_int64 f0f6_2  = f0_2 * (crypto_int64) f6;
    crypto_int64 f0f7_2  = f0_2 * (crypto_int64) f7;
    crypto_int64 f0f8_2  = f0_2 * (crypto_int64) f8;
    crypto_int64 f0f9_2  = f0_2 * (crypto_int64) f9;
    crypto_int64 f1f1_2  = f1_2 * (crypto_int64) f1;
    crypto_int64 f1f2_2  = f1_2 * (crypto_int64) f2;
    crypto_int64 f1f3_4  = f1_2 * (crypto_int64) f3_2;
    crypto_int64 f1f4_2  = f1_2 * (crypto_int64) f4;
    crypto_int64 f1f5_4  = f1_2 * (crypto_int64) f5_2;
    crypto_int64 f1f6_2  = f1_2 * (crypto_int64) f6;
    crypto_int64 f1f7_4  = f1_2 * (crypto_int64) f7_2;
    crypto_int64 f1f8_2  = f1_2 * (crypto_int64) f8;
    crypto_int64 f1f9_76 = f1_2 * (crypto_int64) f9_38;
    crypto_int64 f2f2    = f2   * (crypto_int64) f2;
    crypto_int64 f2f3_2  = f2_2 * (crypto_int64) f3;
    crypto_int64 f2f4_2  = f2_2 * (crypto_int64) f4;
    crypto_int64 f2f5_2  = f2_2 * (crypto_int64) f5;
    crypto_int64 f2f6_2  = f2_2 * (crypto_int64) f6;
    crypto_int64 f2f7_2  = f2_2 * (crypto_int64) f7;
    crypto_int64 f2f8_38 = f2_2 * (crypto_int64) f8_19;
    crypto_int64 f2f9_38 = f2   * (crypto_int64) f9_38;
    crypto_int64 f3f3_2  = f3_2 * (crypto_int64) f3;
    crypto_int64 f3f4_2  = f3_2 * (crypto_int64) f4;
    crypto_int64 f3f5_4  = f3_2 * (crypto_int64) f5_2;
    crypto_int64 f3f6_2  = f3_2 * (crypto_int64) f6;
    crypto_int64 f3f7_76 = f3_2 * (crypto_int64) f7_38;
    crypto_int64 f3f8_38 = f3_2 * (crypto_int64) f8_19;
    crypto_int64 f3f9_76 = f3_2 * (crypto_int64) f9_38;
    crypto_int64 f4f4    = f4   * (crypto_int64) f4;
    crypto_int64 f4f5_2  = f4_2 * (crypto_int64) f5;
    crypto_int64 f4f6_38 = f4_2 * (crypto_int64) f6_19;
    crypto_int64 f4f7_38 = f4   * (crypto_int64) f7_38;
    crypto_int64 f4f8_38 = f4_2 * (crypto_int64) f8_19;
    crypto_int64 f4f9_38 = f4   * (crypto_int64) f9_38;
    crypto_int64 f5f5_38 = f5   * (crypto_int64) f5_38;
    crypto_int64 f5f6_38 = f5_2 * (crypto_int64) f6_19;
    crypto_int64 f5f7_76 = f5_2 * (crypto_int64) f7_38;
    crypto_int64 f5f8_38 = f5_2 * (crypto_int64) f8_19;
    crypto_int64 f5f9_76 = f5_2 * (crypto_int64) f9_38;
    crypto_int64 f6f6_19 = f6   * (crypto_int64) f6_19;
    crypto_int64 f6f7_38 = f6   * (crypto_int64) f7_38;
    crypto_int64 f6f8_38 = f6_2 * (crypto_int64) f8_19;
    crypto_int64 f6f9_38 = f6   * (crypto_int64) f9_38;
    crypto_int64 f7f7_38 = f7   * (crypto_int64) f7_38;
    crypto_int64 f7f8_38 = f7_2 * (crypto_int64) f8_19;
    crypto_int64 f7f9_76 = f7_2 * (crypto_int64) f9_38;
    crypto_int64 f8f8_19 = f8   * (crypto_int64) f8_19;
    crypto_int64 f8f9_38 = f8   * (crypto_int64) f9_38;
    crypto_int64 f9f9_38 = f9   * (crypto_int64) f9_38;
    crypto_int64 h0 = f0f0  +f1f9_76+f2f8_38+f3f7_76+f4f6_38+f5f5_38;
    crypto_int64 h1 = f0f1_2+f2f9_38+f3f8_38+f4f7_38+f5f6_38;
    crypto_int64 h2 = f0f2_2+f1f1_2 +f3f9_76+f4f8_38+f5f7_76+f6f6_19;
    crypto_int64 h3 = f0f3_2+f1f2_2 +f4f9_38+f5f8_38+f6f7_38;
    crypto_int64 h4 = f0f4_2+f1f3_4 +f2f2   +f5f9_76+f6f8_38+f7f7_38;
    crypto_int64 h5 = f0f5_2+f1f4_2 +f2f3_2 +f6f9_38+f7f8_38;
    crypto_int64 h6 = f0f6_2+f1f5_4 +f2f4_2 +f3f3_2 +f7f9_76+f8f8_19;
    crypto_int64 h7 = f0f7_2+f1f6_2 +f2f5_2 +f3f4_2 +f8f9_38;
    crypto_int64 h8 = f0f8_2+f1f7_4 +f2f6_2 +f3f5_4 +f4f4   +f9f9_38;
    crypto_int64 h9 = f0f9_2+f1f8_2 +f2f7_2 +f3f6_2 +f4f5_2;
    crypto_int64 carry0;
    crypto_int64 carry1;
    crypto_int64 carry2;
    crypto_int64 carry3;
    crypto_int64 carry4;
    crypto_int64 carry5;
    crypto_int64 carry6;
    crypto_int64 carry7;
    crypto_int64 carry8;
    crypto_int64 carry9;

    h0 += h0;
    h1 += h1;
    h2 += h2;
    h3 += h3;
    h4 += h4;
    h5 += h5;
    h6 += h6;
    h7 += h7;
    h8 += h8;
    h9 += h9;

    carry0 = (h0 + (crypto_int64) (1<<25)) >> 26; h1 += carry0; h0 -= carry0 << 26;
    carry4 = (h4 + (crypto_int64) (1<<25)) >> 26; h5 += carry4; h4 -= carry4 << 26;

    carry1 = (h1 + (crypto_int64) (1<<24)) >> 25; h2 += carry1; h1 -= carry1 << 25;
    carry5 = (h5 + (crypto_int64) (1<<24)) >> 25; h6 += carry5; h5 -= carry5 << 25;

    carry2 = (h2 + (crypto_int64) (1<<25)) >> 26; h3 += carry2; h2 -= carry2 << 26;
    carry6 = (h6 + (crypto_int64) (1<<25)) >> 26; h7 += carry6; h6 -= carry6 << 26;

    carry3 = (h3 + (crypto_int64) (1<<24)) >> 25; h4 += carry3; h3 -= carry3 << 25;
    carry7 = (h7 + (crypto_int64) (1<<24)) >> 25; h8 += carry7; h7 -= carry7 << 25;

    carry4 = (h4 + (crypto_int64) (1<<25)) >> 26; h5 += carry4; h4 -= carry4 << 26;
    carry8 = (h8 + (crypto_int64) (1<<25)) >> 26; h9 += carry8; h8 -= carry8 << 26;

    carry9 = (h9 + (crypto_int64) (1<<24)) >> 25; h0 += carry9 * 19; h9 -= carry9 << 25;

    carry0 = (h0 + (crypto_int64) (1<<25)) >> 26; h1 += carry0; h0 -= carry0 << 26;

    h[0] = h0;
    h[1] = h1;
    h[2] = h2;
    h[3] = h3;
    h[4] = h4;
    h[5] = h5;
    h[6] = h6;
    h[7] = h7;
    h[8] = h8;
    h[9] = h9;
}

void fe_invert(fe out,const fe z)
{
    fe t0;
    fe t1;
    fe t2;
    fe t3;
    int i;


/* qhasm: fe z1 */

/* qhasm: fe z2 */

/* qhasm: fe z8 */

/* qhasm: fe z9 */

/* qhasm: fe z11 */

/* qhasm: fe z22 */

/* qhasm: fe z_5_0 */

/* qhasm: fe z_10_5 */

/* qhasm: fe z_10_0 */

/* qhasm: fe z_20_10 */

/* qhasm: fe z_20_0 */

/* qhasm: fe z_40_20 */

/* qhasm: fe z_40_0 */

/* qhasm: fe z_50_10 */

/* qhasm: fe z_50_0 */

/* qhasm: fe z_100_50 */

/* qhasm: fe z_100_0 */

/* qhasm: fe z_200_100 */

/* qhasm: fe z_200_0 */

/* qhasm: fe z_250_50 */

/* qhasm: fe z_250_0 */

/* qhasm: fe z_255_5 */

/* qhasm: fe z_255_21 */

/* qhasm: enter pow225521 */

/* qhasm: z2 = z1^2^1 */
/* asm 1: fe_sq(>z2=fe#1,<z1=fe#11); for (i = 1;i < 1;++i) fe_sq(>z2=fe#1,>z2=fe#1); */
/* asm 2: fe_sq(>z2=t0,<z1=z); for (i = 1;i < 1;++i) fe_sq(>z2=t0,>z2=t0); */
    fe_sq(t0,z); for (i = 1;i < 1;++i) fe_sq(t0,t0);

/* qhasm: z8 = z2^2^2 */
/* asm 1: fe_sq(>z8=fe#2,<z2=fe#1); for (i = 1;i < 2;++i) fe_sq(>z8=fe#2,>z8=fe#2); */
/* asm 2: fe_sq(>z8=t1,<z2=t0); for (i = 1;i < 2;++i) fe_sq(>z8=t1,>z8=t1); */
    fe_sq(t1,t0); for (i = 1;i < 2;++i) fe_sq(t1,t1);

/* qhasm: z9 = z1*z8 */
/* asm 1: fe_mul(>z9=fe#2,<z1=fe#11,<z8=fe#2); */
/* asm 2: fe_mul(>z9=t1,<z1=z,<z8=t1); */
    fe_mul(t1,z,t1);

/* qhasm: z11 = z2*z9 */
/* asm 1: fe_mul(>z11=fe#1,<z2=fe#1,<z9=fe#2); */
/* asm 2: fe_mul(>z11=t0,<z2=t0,<z9=t1); */
    fe_mul(t0,t0,t1);

/* qhasm: z22 = z11^2^1 */
/* asm 1: fe_sq(>z22=fe#3,<z11=fe#1); for (i = 1;i < 1;++i) fe_sq(>z22=fe#3,>z22=fe#3); */
/* asm 2: fe_sq(>z22=t2,<z11=t0); for (i = 1;i < 1;++i) fe_sq(>z22=t2,>z22=t2); */
    fe_sq(t2,t0); for (i = 1;i < 1;++i) fe_sq(t2,t2);

/* qhasm: z_5_0 = z9*z22 */
/* asm 1: fe_mul(>z_5_0=fe#2,<z9=fe#2,<z22=fe#3); */
/* asm 2: fe_mul(>z_5_0=t1,<z9=t1,<z22=t2); */
    fe_mul(t1,t1,t2);

/* qhasm: z_10_5 = z_5_0^2^5 */
/* asm 1: fe_sq(>z_10_5=fe#3,<z_5_0=fe#2); for (i = 1;i < 5;++i) fe_sq(>z_10_5=fe#3,>z_10_5=fe#3); */
/* asm 2: fe_sq(>z_10_5=t2,<z_5_0=t1); for (i = 1;i < 5;++i) fe_sq(>z_10_5=t2,>z_10_5=t2); */
    fe_sq(t2,t1); for (i = 1;i < 5;++i) fe_sq(t2,t2);

/* qhasm: z_10_0 = z_10_5*z_5_0 */
/* asm 1: fe_mul(>z_10_0=fe#2,<z_10_5=fe#3,<z_5_0=fe#2); */
/* asm 2: fe_mul(>z_10_0=t1,<z_10_5=t2,<z_5_0=t1); */
    fe_mul(t1,t2,t1);

/* qhasm: z_20_10 = z_10_0^2^10 */
/* asm 1: fe_sq(>z_20_10=fe#3,<z_10_0=fe#2); for (i = 1;i < 10;++i) fe_sq(>z_20_10=fe#3,>z_20_10=fe#3); */
/* asm 2: fe_sq(>z_20_10=t2,<z_10_0=t1); for (i = 1;i < 10;++i) fe_sq(>z_20_10=t2,>z_20_10=t2); */
    fe_sq(t2,t1); for (i = 1;i < 10;++i) fe_sq(t2,t2);

/* qhasm: z_20_0 = z_20_10*z_10_0 */
/* asm 1: fe_mul(>z_20_0=fe#3,<z_20_10=fe#3,<z_10_0=fe#2); */
/* asm 2: fe_mul(>z_20_0=t2,<z_20_10=t2,<z_10_0=t1); */
    fe_mul(t2,t2,t1);

/* qhasm: z_40_20 = z_20_0^2^20 */
/* asm 1: fe_sq(>z_40_20=fe#4,<z_20_0=fe#3); for (i = 1;i < 20;++i) fe_sq(>z_40_20=fe#4,>z_40_20=fe#4); */
/* asm 2: fe_sq(>z_40_20=t3,<z_20_0=t2); for (i = 1;i < 20;++i) fe_sq(>z_40_20=t3,>z_40_20=t3); */
    fe_sq(t3,t2); for (i = 1;i < 20;++i) fe_sq(t3,t3);

/* qhasm: z_40_0 = z_40_20*z_20_0 */
/* asm 1: fe_mul(>z_40_0=fe#3,<z_40_20=fe#4,<z_20_0=fe#3); */
/* asm 2: fe_mul(>z_40_0=t2,<z_40_20=t3,<z_20_0=t2); */
    fe_mul(t2,t3,t2);

/* qhasm: z_50_10 = z_40_0^2^10 */
/* asm 1: fe_sq(>z_50_10=fe#3,<z_40_0=fe#3); for (i = 1;i < 10;++i) fe_sq(>z_50_10=fe#3,>z_50_10=fe#3); */
/* asm 2: fe_sq(>z_50_10=t2,<z_40_0=t2); for (i = 1;i < 10;++i) fe_sq(>z_50_10=t2,>z_50_10=t2); */
    fe_sq(t2,t2); for (i = 1;i < 10;++i) fe_sq(t2,t2);

/* qhasm: z_50_0 = z_50_10*z_10_0 */
/* asm 1: fe_mul(>z_50_0=fe#2,<z_50_10=fe#3,<z_10_0=fe#2); */
/* asm 2: fe_mul(>z_50_0=t1,<z_50_10=t2,<z_10_0=t1); */
    fe_mul(t1,t2,t1);

/* qhasm: z_100_50 = z_50_0^2^50 */
/* asm 1: fe_sq(>z_100_50=fe#3,<z_50_0=fe#2); for (i = 1;i < 50;++i) fe_sq(>z_100_50=fe#3,>z_100_50=fe#3); */
/* asm 2: fe_sq(>z_100_50=t2,<z_50_0=t1); for (i = 1;i < 50;++i) fe_sq(>z_100_50=t2,>z_100_50=t2); */
    fe_sq(t2,t1); for (i = 1;i < 50;++i) fe_sq(t2,t2);

/* qhasm: z_100_0 = z_100_50*z_50_0 */
/* asm 1: fe_mul(>z_100_0=fe#3,<z_100_50=fe#3,<z_50_0=fe#2); */
/* asm 2: fe_mul(>z_100_0=t2,<z_100_50=t2,<z_50_0=t1); */
    fe_mul(t2,t2,t1);

/* qhasm: z_200_100 = z_100_0^2^100 */
/* asm 1: fe_sq(>z_200_100=fe#4,<z_100_0=fe#3); for (i = 1;i < 100;++i) fe_sq(>z_200_100=fe#4,>z_200_100=fe#4); */
/* asm 2: fe_sq(>z_200_100=t3,<z_100_0=t2); for (i = 1;i < 100;++i) fe_sq(>z_200_100=t3,>z_200_100=t3); */
    fe_sq(t3,t2); for (i = 1;i < 100;++i) fe_sq(t3,t3);

/* qhasm: z_200_0 = z_200_100*z_100_0 */
/* asm 1: fe_mul(>z_200_0=fe#3,<z_200_100=fe#4,<z_100_0=fe#3); */
/* asm 2: fe_mul(>z_200_0=t2,<z_200_100=t3,<z_100_0=t2); */
    fe_mul(t2,t3,t2);

/* qhasm: z_250_50 = z_200_0^2^50 */
/* asm 1: fe_sq(>z_250_50=fe#3,<z_200_0=fe#3); for (i = 1;i < 50;++i) fe_sq(>z_250_50=fe#3,>z_250_50=fe#3); */
/* asm 2: fe_sq(>z_250_50=t2,<z_200_0=t2); for (i = 1;i < 50;++i) fe_sq(>z_250_50=t2,>z_250_50=t2); */
    fe_sq(t2,t2); for (i = 1;i < 50;++i) fe_sq(t2,t2);

/* qhasm: z_250_0 = z_250_50*z_50_0 */
/* asm 1: fe_mul(>z_250_0=fe#2,<z_250_50=fe#3,<z_50_0=fe#2); */
/* asm 2: fe_mul(>z_250_0=t1,<z_250_50=t2,<z_50_0=t1); */
    fe_mul(t1,t2,t1);

/* qhasm: z_255_5 = z_250_0^2^5 */
/* asm 1: fe_sq(>z_255_5=fe#2,<z_250_0=fe#2); for (i = 1;i < 5;++i) fe_sq(>z_255_5=fe#2,>z_255_5=fe#2); */
/* asm 2: fe_sq(>z_255_5=t1,<z_250_0=t1); for (i = 1;i < 5;++i) fe_sq(>z_255_5=t1,>z_255_5=t1); */
    fe_sq(t1,t1); for (i = 1;i < 5;++i) fe_sq(t1,t1);

/* qhasm: z_255_21 = z_255_5*z11 */
/* asm 1: fe_mul(>z_255_21=fe#12,<z_255_5=fe#2,<z11=fe#1); */
/* asm 2: fe_mul(>z_255_21=out,<z_255_5=t1,<z11=t0); */
    fe_mul(out,t1,t0);

/* qhasm: return */

    return;
}

int fe_isnegative(const fe f)
{
    unsigned char s[32];
    fe_tobytes(s,f);
    return s[0] & 1;
}

static int fe_isnonzero(const fe f) {
    unsigned char s[32];
    fe_tobytes(s, f);
    return (((int) (s[0] | s[1] | s[2] | s[3] | s[4] | s[5] | s[6] | s[7] | s[8] |
                    s[9] | s[10] | s[11] | s[12] | s[13] | s[14] | s[15] | s[16] | s[17] |
                    s[18] | s[19] | s[20] | s[21] | s[22] | s[23] | s[24] | s[25] | s[26] |
                    s[27] | s[28] | s[29] | s[30] | s[31]) - 1) >> 8) + 1;
}

void fe_tobytes(unsigned char *s,const fe h)
{
    crypto_int32 h0 = h[0];
    crypto_int32 h1 = h[1];
    crypto_int32 h2 = h[2];
    crypto_int32 h3 = h[3];
    crypto_int32 h4 = h[4];
    crypto_int32 h5 = h[5];
    crypto_int32 h6 = h[6];
    crypto_int32 h7 = h[7];
    crypto_int32 h8 = h[8];
    crypto_int32 h9 = h[9];
    crypto_int32 q;
    crypto_int32 carry0;
    crypto_int32 carry1;
    crypto_int32 carry2;
    crypto_int32 carry3;
    crypto_int32 carry4;
    crypto_int32 carry5;
    crypto_int32 carry6;
    crypto_int32 carry7;
    crypto_int32 carry8;
    crypto_int32 carry9;

    q = (19 * h9 + (((crypto_int32) 1) << 24)) >> 25;
    q = (h0 + q) >> 26;
    q = (h1 + q) >> 25;
    q = (h2 + q) >> 26;
    q = (h3 + q) >> 25;
    q = (h4 + q) >> 26;
    q = (h5 + q) >> 25;
    q = (h6 + q) >> 26;
    q = (h7 + q) >> 25;
    q = (h8 + q) >> 26;
    q = (h9 + q) >> 25;

    /* Goal: Output h-(2^255-19)q, which is between 0 and 2^255-20. */
    h0 += 19 * q;
    /* Goal: Output h-2^255 q, which is between 0 and 2^255-20. */

    carry0 = h0 >> 26; h1 += carry0; h0 -= carry0 << 26;
    carry1 = h1 >> 25; h2 += carry1; h1 -= carry1 << 25;
    carry2 = h2 >> 26; h3 += carry2; h2 -= carry2 << 26;
    carry3 = h3 >> 25; h4 += carry3; h3 -= carry3 << 25;
    carry4 = h4 >> 26; h5 += carry4; h4 -= carry4 << 26;
    carry5 = h5 >> 25; h6 += carry5; h5 -= carry5 << 25;
    carry6 = h6 >> 26; h7 += carry6; h6 -= carry6 << 26;
    carry7 = h7 >> 25; h8 += carry7; h7 -= carry7 << 25;
    carry8 = h8 >> 26; h9 += carry8; h8 -= carry8 << 26;
    carry9 = h9 >> 25;               h9 -= carry9 << 25;
    /* h10 = carry9 */

    /*
    Goal: Output h0+...+2^255 h10-2^255 q, which is between 0 and 2^255-20.
    Have h0+...+2^230 h9 between 0 and 2^255-1;
    evidently 2^255 h10-2^255 q = 0.
    Goal: Output h0+...+2^230 h9.
    */

    s[0] = h0 >> 0;
    s[1] = h0 >> 8;
    s[2] = h0 >> 16;
    s[3] = (h0 >> 24) | (h1 << 2);
    s[4] = h1 >> 6;
    s[5] = h1 >> 14;
    s[6] = (h1 >> 22) | (h2 << 3);
    s[7] = h2 >> 5;
    s[8] = h2 >> 13;
    s[9] = (h2 >> 21) | (h3 << 5);
    s[10] = h3 >> 3;
    s[11] = h3 >> 11;
    s[12] = (h3 >> 19) | (h4 << 6);
    s[13] = h4 >> 2;
    s[14] = h4 >> 10;
    s[15] = h4 >> 18;
    s[16] = h5 >> 0;
    s[17] = h5 >> 8;
    s[18] = h5 >> 16;
    s[19] = (h5 >> 24) | (h6 << 1);
    s[20] = h6 >> 7;
    s[21] = h6 >> 15;
    s[22] = (h6 >> 23) | (h7 << 3);
    s[23] = h7 >> 5;
    s[24] = h7 >> 13;
    s[25] = (h7 >> 21) | (h8 << 4);
    s[26] = h8 >> 4;
    s[27] = h8 >> 12;
    s[28] = (h8 >> 20) | (h9 << 6);
    s[29] = h9 >> 2;
    s[30] = h9 >> 10;
    s[31] = h9 >> 18;
}

static void fe_divpowm1(fe r, const fe u, const fe v) {
    fe v3, uv7, t0, t1, t2;
    int i;

    fe_sq(v3, v);
    fe_mul(v3, v3, v); /* v3 = v^3 */
    fe_sq(uv7, v3);
    fe_mul(uv7, uv7, v);
    fe_mul(uv7, uv7, u); /* uv7 = uv^7 */

    /*fe_pow22523(uv7, uv7);*/

    /* From fe_pow22523.c */

    fe_sq(t0, uv7);
    fe_sq(t1, t0);
    fe_sq(t1, t1);
    fe_mul(t1, uv7, t1);
    fe_mul(t0, t0, t1);
    fe_sq(t0, t0);
    fe_mul(t0, t1, t0);
    fe_sq(t1, t0);
    for (i = 0; i < 4; ++i) {
        fe_sq(t1, t1);
    }
    fe_mul(t0, t1, t0);
    fe_sq(t1, t0);
    for (i = 0; i < 9; ++i) {
        fe_sq(t1, t1);
    }
    fe_mul(t1, t1, t0);
    fe_sq(t2, t1);
    for (i = 0; i < 19; ++i) {
        fe_sq(t2, t2);
    }
    fe_mul(t1, t2, t1);
    for (i = 0; i < 10; ++i) {
        fe_sq(t1, t1);
    }
    fe_mul(t0, t1, t0);
    fe_sq(t1, t0);
    for (i = 0; i < 49; ++i) {
        fe_sq(t1, t1);
    }
    fe_mul(t1, t1, t0);
    fe_sq(t2, t1);
    for (i = 0; i < 99; ++i) {
        fe_sq(t2, t2);
    }
    fe_mul(t1, t2, t1);
    for (i = 0; i < 50; ++i) {
        fe_sq(t1, t1);
    }
    fe_mul(t0, t1, t0);
    fe_sq(t0, t0);
    fe_sq(t0, t0);
    fe_mul(t0, t0, uv7);

    /* End fe_pow22523.c */
    /* t0 = (uv^7)^((q-5)/8) */
    fe_mul(t0, t0, v3);
    fe_mul(r, t0, u); /* u^(m+1)v^(-(m+1)) */
}

void ge_add(ge_p1p1 *r, const ge_p3 *p, const ge_cached *q) {
    fe t0;
    fe_add(r->X, p->Y, p->X);
    fe_sub(r->Y, p->Y, p->X);
    fe_mul(r->Z, r->X, q->YplusX);
    fe_mul(r->Y, r->Y, q->YminusX);
    fe_mul(r->T, q->T2d, p->T);
    fe_mul(r->X, p->Z, q->Z);
    fe_add(t0, r->X, r->X);
    fe_sub(r->X, r->Z, r->Y);
    fe_add(r->Y, r->Z, r->Y);
    fe_add(r->Z, t0, r->T);
    fe_sub(r->T, t0, r->T);
}

void ge_madd(ge_p1p1 *r,const ge_p3 *p,const ge_precomp *q)
{
    fe t0;

/* qhasm: enter ge_madd */

/* qhasm: fe X1 */

/* qhasm: fe Y1 */

/* qhasm: fe Z1 */

/* qhasm: fe T1 */

/* qhasm: fe ypx2 */

/* qhasm: fe ymx2 */

/* qhasm: fe xy2d2 */

/* qhasm: fe X3 */

/* qhasm: fe Y3 */

/* qhasm: fe Z3 */

/* qhasm: fe T3 */

/* qhasm: fe YpX1 */

/* qhasm: fe YmX1 */

/* qhasm: fe A */

/* qhasm: fe B */

/* qhasm: fe C */

/* qhasm: fe D */

/* qhasm: YpX1 = Y1+X1 */
/* asm 1: fe_add(>YpX1=fe#1,<Y1=fe#12,<X1=fe#11); */
/* asm 2: fe_add(>YpX1=r->X,<Y1=p->Y,<X1=p->X); */
    fe_add(r->X,p->Y,p->X);

/* qhasm: YmX1 = Y1-X1 */
/* asm 1: fe_sub(>YmX1=fe#2,<Y1=fe#12,<X1=fe#11); */
/* asm 2: fe_sub(>YmX1=r->Y,<Y1=p->Y,<X1=p->X); */
    fe_sub(r->Y,p->Y,p->X);

/* qhasm: A = YpX1*ypx2 */
/* asm 1: fe_mul(>A=fe#3,<YpX1=fe#1,<ypx2=fe#15); */
/* asm 2: fe_mul(>A=r->Z,<YpX1=r->X,<ypx2=q->yplusx); */
    fe_mul(r->Z,r->X,q->yplusx);

/* qhasm: B = YmX1*ymx2 */
/* asm 1: fe_mul(>B=fe#2,<YmX1=fe#2,<ymx2=fe#16); */
/* asm 2: fe_mul(>B=r->Y,<YmX1=r->Y,<ymx2=q->yminusx); */
    fe_mul(r->Y,r->Y,q->yminusx);

/* qhasm: C = xy2d2*T1 */
/* asm 1: fe_mul(>C=fe#4,<xy2d2=fe#17,<T1=fe#14); */
/* asm 2: fe_mul(>C=r->T,<xy2d2=q->xy2d,<T1=p->T); */
    fe_mul(r->T,q->xy2d,p->T);

/* qhasm: D = 2*Z1 */
/* asm 1: fe_add(>D=fe#5,<Z1=fe#13,<Z1=fe#13); */
/* asm 2: fe_add(>D=t0,<Z1=p->Z,<Z1=p->Z); */
    fe_add(t0,p->Z,p->Z);

/* qhasm: X3 = A-B */
/* asm 1: fe_sub(>X3=fe#1,<A=fe#3,<B=fe#2); */
/* asm 2: fe_sub(>X3=r->X,<A=r->Z,<B=r->Y); */
    fe_sub(r->X,r->Z,r->Y);

/* qhasm: Y3 = A+B */
/* asm 1: fe_add(>Y3=fe#2,<A=fe#3,<B=fe#2); */
/* asm 2: fe_add(>Y3=r->Y,<A=r->Z,<B=r->Y); */
    fe_add(r->Y,r->Z,r->Y);

/* qhasm: Z3 = D+C */
/* asm 1: fe_add(>Z3=fe#3,<D=fe#5,<C=fe#4); */
/* asm 2: fe_add(>Z3=r->Z,<D=t0,<C=r->T); */
    fe_add(r->Z,t0,r->T);

/* qhasm: T3 = D-C */
/* asm 1: fe_sub(>T3=fe#4,<D=fe#5,<C=fe#4); */
/* asm 2: fe_sub(>T3=r->T,<D=t0,<C=r->T); */
    fe_sub(r->T,t0,r->T);

/* qhasm: return */
}

void ge_precomp_0(ge_precomp *h)
{
    fe_1(h->yplusx);
    fe_1(h->yminusx);
    fe_0(h->xy2d);
}

static ge_precomp base[32][8] = {
#include "Monero/crypto/base.h"
} ;

static void ge_p2_0(ge_p2 *h) {
    fe_0(h->X);
    fe_1(h->Y);
    fe_1(h->Z);
}

void ge_p2_dbl(ge_p1p1 *r, const ge_p2 *p) {
    fe t0;
    fe_sq(r->X, p->X);
    fe_sq(r->Z, p->Y);
    fe_sq2(r->T, p->Z);
    fe_add(r->Y, p->X, p->Y);
    fe_sq(t0, r->Y);
    fe_add(r->Y, r->Z, r->X);
    fe_sub(r->Z, r->Z, r->X);
    fe_sub(r->X, t0, r->Y);
    fe_sub(r->T, r->T, r->Z);
}

void ge_p3_dbl(ge_p1p1 *r,const ge_p3 *p)
{
    ge_p2 q;
    ge_p3_to_p2(&q,p);
    ge_p2_dbl(r,&q);
}

void ge_p3_0(ge_p3 *h)
{
    fe_0(h->X);
    fe_1(h->Y);
    fe_1(h->Z);
    fe_0(h->T);
}

extern void ge_p3_to_p2(ge_p2 *r,const ge_p3 *p)
{
    fe_copy(r->X,p->X);
    fe_copy(r->Y,p->Y);
    fe_copy(r->Z,p->Z);
}

void ge_p3_tobytes(unsigned char *s,const ge_p3 *h)
{
    fe recip;
    fe x;
    fe y;

    fe_invert(recip,h->Z);
    fe_mul(x,h->X,recip);
    fe_mul(y,h->Y,recip);
    fe_tobytes(s,y);
    s[31] ^= fe_isnegative(x) << 7;
}

void ge_p3_to_cached(ge_cached *r, const ge_p3 *p) {
    fe_add(r->YplusX, p->Y, p->X);
    fe_sub(r->YminusX, p->Y, p->X);
    fe_copy(r->Z, p->Z);
    fe_mul(r->T2d, p->T, fe_d2);
}

static void ge_cached_0(ge_cached *r) {
    fe_1(r->YplusX);
    fe_1(r->YminusX);
    fe_1(r->Z);
    fe_0(r->T2d);
}

static void ge_cached_cmov(ge_cached *t, const ge_cached *u, unsigned char b) {
    fe_cmov(t->YplusX, u->YplusX, b);
    fe_cmov(t->YminusX, u->YminusX, b);
    fe_cmov(t->Z, u->Z, b);
    fe_cmov(t->T2d, u->T2d, b);
}

void ge_p1p1_to_p2(ge_p2 *r, const ge_p1p1 *p) {
    fe_mul(r->X, p->X, p->T);
    fe_mul(r->Y, p->Y, p->Z);
    fe_mul(r->Z, p->Z, p->T);
}

void ge_p1p1_to_p3(ge_p3 *r, const ge_p1p1 *p) {
    fe_mul(r->X, p->X, p->T);
    fe_mul(r->Y, p->Y, p->Z);
    fe_mul(r->Z, p->Z, p->T);
    fe_mul(r->T, p->X, p->Y);
}

void ge_mul8(ge_p1p1 *r, const ge_p2 *t) {
    ge_p2 u;
    ge_p2_dbl(r, t);
    ge_p1p1_to_p2(&u, r);
    ge_p2_dbl(r, &u);
    ge_p1p1_to_p2(&u, r);
    ge_p2_dbl(r, &u);
}

int ge_frombytes_vartime(ge_p3 *h, const unsigned char *s) {
    fe u;
    fe v;
    fe vxx;
    fe check;

    /* From fe_frombytes.c */

    int64_t h0 = load_4(s);
    int64_t h1 = load_3(s + 4) << 6;
    int64_t h2 = load_3(s + 7) << 5;
    int64_t h3 = load_3(s + 10) << 3;
    int64_t h4 = load_3(s + 13) << 2;
    int64_t h5 = load_4(s + 16);
    int64_t h6 = load_3(s + 20) << 7;
    int64_t h7 = load_3(s + 23) << 5;
    int64_t h8 = load_3(s + 26) << 4;
    int64_t h9 = (load_3(s + 29) & 8388607) << 2;
    int64_t carry0;
    int64_t carry1;
    int64_t carry2;
    int64_t carry3;
    int64_t carry4;
    int64_t carry5;
    int64_t carry6;
    int64_t carry7;
    int64_t carry8;
    int64_t carry9;

    /* Validate the number to be canonical */
    if (h9 == 33554428 && h8 == 268435440 && h7 == 536870880 && h6 == 2147483520 &&
        h5 == 4294967295 && h4 == 67108860 && h3 == 134217720 && h2 == 536870880 &&
        h1 == 1073741760 && h0 >= 4294967277) {
        return -1;
    }

    carry9 = (h9 + (int64_t) (1<<24)) >> 25; h0 += carry9 * 19; h9 -= carry9 << 25;
    carry1 = (h1 + (int64_t) (1<<24)) >> 25; h2 += carry1; h1 -= carry1 << 25;
    carry3 = (h3 + (int64_t) (1<<24)) >> 25; h4 += carry3; h3 -= carry3 << 25;
    carry5 = (h5 + (int64_t) (1<<24)) >> 25; h6 += carry5; h5 -= carry5 << 25;
    carry7 = (h7 + (int64_t) (1<<24)) >> 25; h8 += carry7; h7 -= carry7 << 25;

    carry0 = (h0 + (int64_t) (1<<25)) >> 26; h1 += carry0; h0 -= carry0 << 26;
    carry2 = (h2 + (int64_t) (1<<25)) >> 26; h3 += carry2; h2 -= carry2 << 26;
    carry4 = (h4 + (int64_t) (1<<25)) >> 26; h5 += carry4; h4 -= carry4 << 26;
    carry6 = (h6 + (int64_t) (1<<25)) >> 26; h7 += carry6; h6 -= carry6 << 26;
    carry8 = (h8 + (int64_t) (1<<25)) >> 26; h9 += carry8; h8 -= carry8 << 26;

    h->Y[0] = h0;
    h->Y[1] = h1;
    h->Y[2] = h2;
    h->Y[3] = h3;
    h->Y[4] = h4;
    h->Y[5] = h5;
    h->Y[6] = h6;
    h->Y[7] = h7;
    h->Y[8] = h8;
    h->Y[9] = h9;

    /* End fe_frombytes.c */

    fe_1(h->Z);
    fe_sq(u, h->Y);
    fe_mul(v, u, fe_d);
    fe_sub(u, u, h->Z);       /* u = y^2-1 */
    fe_add(v, v, h->Z);       /* v = dy^2+1 */

    fe_divpowm1(h->X, u, v); /* x = uv^3(uv^7)^((q-5)/8) */

    fe_sq(vxx, h->X);
    fe_mul(vxx, vxx, v);
    fe_sub(check, vxx, u);    /* vx^2-u */
    if (fe_isnonzero(check)) {
        fe_add(check, vxx, u);  /* vx^2+u */
        if (fe_isnonzero(check)) {
            return -1;
        }
        fe_mul(h->X, h->X, fe_sqrtm1);
    }

    if (fe_isnegative(h->X) != (s[31] >> 7)) {
        /* If x = 0, the sign must be positive */
        if (!fe_isnonzero(h->X)) {
            return -1;
        }
        fe_neg(h->X, h->X);
    }

    fe_mul(h->T, h->X, h->Y);
    return 0;
}

void ge_tobytes(unsigned char *s, const ge_p2 *h) {
    fe recip;
    fe x;
    fe y;

    fe_invert(recip, h->Z);
    fe_mul(x, h->X, recip);
    fe_mul(y, h->Y, recip);
    fe_tobytes(s, y);
    s[31] ^= fe_isnegative(x) << 7;
}

static void select(ge_precomp *t,int pos,signed char b)
{
    ge_precomp minust;
    unsigned char bnegative = negative(b);
    unsigned char babs = b - (((-bnegative) & b) << 1);

    ge_precomp_0(t);
    cmov(t,&base[pos][0],equal(babs,1));
    cmov(t,&base[pos][1],equal(babs,2));
    cmov(t,&base[pos][2],equal(babs,3));
    cmov(t,&base[pos][3],equal(babs,4));
    cmov(t,&base[pos][4],equal(babs,5));
    cmov(t,&base[pos][5],equal(babs,6));
    cmov(t,&base[pos][6],equal(babs,7));
    cmov(t,&base[pos][7],equal(babs,8));
    fe_copy(minust.yplusx,t->yminusx);
    fe_copy(minust.yminusx,t->yplusx);
    fe_neg(minust.xy2d,t->xy2d);
    cmov(t,&minust,bnegative);
}

static unsigned char equal(signed char b,signed char c)
{
    unsigned char ub = b;
    unsigned char uc = c;
    unsigned char x = ub ^ uc; /* 0: yes; 1..255: no */
    crypto_uint32 y = x; /* 0: yes; 1..255: no */
    y -= 1; /* 4294967295: yes; 0..254: no */
    y >>= 31; /* 1: yes; 0: no */
    return y;
}

static unsigned char negative(signed char b)
{
    unsigned long long x = b; /* 18446744073709551361..18446744073709551615: yes; 0..255: no */
    x >>= 63; /* 1: yes; 0: no */
    return x;
}

static void cmov(ge_precomp *t,ge_precomp *u,unsigned char b)
{
    fe_cmov(t->yplusx,u->yplusx,b);
    fe_cmov(t->yminusx,u->yminusx,b);
    fe_cmov(t->xy2d,u->xy2d,b);
}

const fe fe_sqrtm1 = {-32595792, -7943725, 9377950, 3500415, 12389472, -272473, -25146209, -2005654, 326686, 11406482};

const fe fe_d = {-10913610, 13857413, -15372611, 6949391, 114729, -8787816, -6275908, -3247719, -18696448, -12055116};

const fe fe_d2 = {-21827239, -5839606, -30745221, 13898782, 229458, 15978800, -12551817, -6495438, 29715968, 9444199};