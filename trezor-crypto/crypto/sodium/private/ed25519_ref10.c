#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <TrezorCrypto/sodium/private/ed25519_ref10.h>

/*
 * Field arithmetic:
 * Use 5*51 bit limbs on 64-bit systems with support for 128 bit arithmetic,
 * and 10*25.5 bit limbs elsewhere.
 *
 * Functions used elsewhere that are candidates for inlining are defined
 * via "private/curve25519_ref10.h".
 */

#include <TrezorCrypto/sodium/private/fe_25_5/constants.h>
#include <TrezorCrypto/sodium/private/fe_25_5/fe.h>

void fe25519_invert(fe25519 out, const fe25519 z) {
    fe25519 t0;
    fe25519 t1;
    fe25519 t2;
    fe25519 t3;
    int i;

    fe25519_sq(t0, z);
    fe25519_sq(t1, t0);
    fe25519_sq(t1, t1);
    fe25519_mul(t1, z, t1);
    fe25519_mul(t0, t0, t1);
    fe25519_sq(t2, t0);
    fe25519_mul(t1, t1, t2);
    fe25519_sq(t2, t1);
    for (i = 1; i < 5; ++i) {
        fe25519_sq(t2, t2);
    }
    fe25519_mul(t1, t2, t1);
    fe25519_sq(t2, t1);
    for (i = 1; i < 10; ++i) {
        fe25519_sq(t2, t2);
    }
    fe25519_mul(t2, t2, t1);
    fe25519_sq(t3, t2);
    for (i = 1; i < 20; ++i) {
        fe25519_sq(t3, t3);
    }
    fe25519_mul(t2, t3, t2);
    fe25519_sq(t2, t2);
    for (i = 1; i < 10; ++i) {
        fe25519_sq(t2, t2);
    }
    fe25519_mul(t1, t2, t1);
    fe25519_sq(t2, t1);
    for (i = 1; i < 50; ++i) {
        fe25519_sq(t2, t2);
    }
    fe25519_mul(t2, t2, t1);
    fe25519_sq(t3, t2);
    for (i = 1; i < 100; ++i) {
        fe25519_sq(t3, t3);
    }
    fe25519_mul(t2, t3, t2);
    fe25519_sq(t2, t2);
    for (i = 1; i < 50; ++i) {
        fe25519_sq(t2, t2);
    }
    fe25519_mul(t1, t2, t1);
    fe25519_sq(t1, t1);
    for (i = 1; i < 5; ++i) {
        fe25519_sq(t1, t1);
    }
    fe25519_mul(out, t1, t0);
}

void fe25519_pow22523(fe25519 out, const fe25519 z) {
    fe25519 t0;
    fe25519 t1;
    fe25519 t2;
    int i;

    fe25519_sq(t0, z);
    fe25519_sq(t1, t0);
    fe25519_sq(t1, t1);
    fe25519_mul(t1, z, t1);
    fe25519_mul(t0, t0, t1);
    fe25519_sq(t0, t0);
    fe25519_mul(t0, t1, t0);
    fe25519_sq(t1, t0);
    for (i = 1; i < 5; ++i) {
        fe25519_sq(t1, t1);
    }
    fe25519_mul(t0, t1, t0);
    fe25519_sq(t1, t0);
    for (i = 1; i < 10; ++i) {
        fe25519_sq(t1, t1);
    }
    fe25519_mul(t1, t1, t0);
    fe25519_sq(t2, t1);
    for (i = 1; i < 20; ++i) {
        fe25519_sq(t2, t2);
    }
    fe25519_mul(t1, t2, t1);
    fe25519_sq(t1, t1);
    for (i = 1; i < 10; ++i) {
        fe25519_sq(t1, t1);
    }
    fe25519_mul(t0, t1, t0);
    fe25519_sq(t1, t0);
    for (i = 1; i < 50; ++i) {
        fe25519_sq(t1, t1);
    }
    fe25519_mul(t1, t1, t0);
    fe25519_sq(t2, t1);
    for (i = 1; i < 100; ++i) {
        fe25519_sq(t2, t2);
    }
    fe25519_mul(t1, t2, t1);
    fe25519_sq(t1, t1);
    for (i = 1; i < 50; ++i) {
        fe25519_sq(t1, t1);
    }
    fe25519_mul(t0, t1, t0);
    fe25519_sq(t0, t0);
    fe25519_sq(t0, t0);
    fe25519_mul(out, t0, z);
}

/*
 r = p + q
 */

void ge25519_add2(ge25519_p1p1_1 *r, const ge25519_p3 *p, const ge25519_cached *q) {
    fe25519 t0;

    fe25519_add(r->X, p->Y, p->X);
    fe25519_sub(r->Y, p->Y, p->X);
    fe25519_mul(r->Z, r->X, q->YplusX);
    fe25519_mul(r->Y, r->Y, q->YminusX);
    fe25519_mul(r->T, q->T2d, p->T);
    fe25519_mul(r->X, p->Z, q->Z);
    fe25519_add(t0, r->X, r->X);
    fe25519_sub(r->X, r->Z, r->Y);
    fe25519_add(r->Y, r->Z, r->Y);
    fe25519_add(r->Z, t0, r->T);
    fe25519_sub(r->T, t0, r->T);
}

int ge25519_frombytes_negate_vartime(ge25519_p3 *h, const unsigned char *s) {
    fe25519 u;
    fe25519 v;
    fe25519 v3;
    fe25519 vxx;
    fe25519 m_root_check, p_root_check;

    fe25519_frombytes(h->Y, s);
    fe25519_1(h->Z);
    fe25519_sq(u, h->Y);
    fe25519_mul(v, u, d);
    fe25519_sub(u, u, h->Z); /* u = y^2-1 */
    fe25519_add(v, v, h->Z); /* v = dy^2+1 */

    fe25519_sq(v3, v);
    fe25519_mul(v3, v3, v); /* v3 = v^3 */
    fe25519_sq(h->X, v3);
    fe25519_mul(h->X, h->X, v);
    fe25519_mul(h->X, h->X, u); /* x = uv^7 */

    fe25519_pow22523(h->X, h->X); /* x = (uv^7)^((q-5)/8) */
    fe25519_mul(h->X, h->X, v3);
    fe25519_mul(h->X, h->X, u); /* x = uv^3(uv^7)^((q-5)/8) */

    fe25519_sq(vxx, h->X);
    fe25519_mul(vxx, vxx, v);
    fe25519_sub(m_root_check, vxx, u); /* vx^2-u */
    if (fe25519_iszero(m_root_check) == 0) {
        fe25519_add(p_root_check, vxx, u); /* vx^2+u */
        if (fe25519_iszero(p_root_check) == 0) {
            return -1;
        }
        fe25519_mul(h->X, h->X, sqrtm1);
    }

    if (fe25519_isnegative(h->X) == (s[31] >> 7)) {
        fe25519_neg(h->X, h->X);
    }
    fe25519_mul(h->T, h->X, h->Y);

    return 0;
}

/*
 r = p
 */

void ge25519_p1p1_to_p3(ge25519_p3 *r, const ge25519_p1p1_1 *p) {
    fe25519_mul(r->X, p->X, p->T);
    fe25519_mul(r->Y, p->Y, p->Z);
    fe25519_mul(r->Z, p->Z, p->T);
    fe25519_mul(r->T, p->X, p->Y);
}

/*
 r = 2 * p
 */

void ge25519_p2_dbl(ge25519_p1p1_1 *r, const ge25519_p2 *p) {
    fe25519 t0;

    fe25519_sq(r->X, p->X);
    fe25519_sq(r->Z, p->Y);
    fe25519_sq2(r->T, p->Z);
    fe25519_add(r->Y, p->X, p->Y);
    fe25519_sq(t0, r->Y);
    fe25519_add(r->Y, r->Z, r->X);
    fe25519_sub(r->Z, r->Z, r->X);
    fe25519_sub(r->X, t0, r->Y);
    fe25519_sub(r->T, r->T, r->Z);
}

void ge25519_p3_0(ge25519_p3 *h) {
    fe25519_0(h->X);
    fe25519_1(h->Y);
    fe25519_1(h->Z);
    fe25519_0(h->T);
}

void ge25519_cached_0(ge25519_cached *h) {
    fe25519_1(h->YplusX);
    fe25519_1(h->YminusX);
    fe25519_1(h->Z);
    fe25519_0(h->T2d);
}

/*
 r = p
 */

void ge25519_p3_to_cached(ge25519_cached *r, const ge25519_p3 *p) {
    fe25519_add(r->YplusX, p->Y, p->X);
    fe25519_sub(r->YminusX, p->Y, p->X);
    fe25519_copy(r->Z, p->Z);
    fe25519_mul(r->T2d, p->T, d2);
}

/*
 r = p
 */

void ge25519_p3_to_p2(ge25519_p2 *r, const ge25519_p3 *p) {
    fe25519_copy(r->X, p->X);
    fe25519_copy(r->Y, p->Y);
    fe25519_copy(r->Z, p->Z);
}

void ge25519_p3_tobytes(unsigned char *s, const ge25519_p3 *h) {
    fe25519 recip;
    fe25519 x;
    fe25519 y;

    fe25519_invert(recip, h->Z);
    fe25519_mul(x, h->X, recip);
    fe25519_mul(y, h->Y, recip);
    fe25519_tobytes(s, y);
    s[31] ^= fe25519_isnegative(x) << 7;
}

/*
 r = 2 * p
 */

void ge25519_p3_dbl(ge25519_p1p1_1 *r, const ge25519_p3 *p) {
    ge25519_p2 q;
    ge25519_p3_to_p2(&q, p);
    ge25519_p2_dbl(r, &q);
}

void ge25519_precomp_0(ge25519_precomp *h) {
    fe25519_1(h->yplusx);
    fe25519_1(h->yminusx);
    fe25519_0(h->xy2d);
}

unsigned char equal(signed char b, signed char c) {
    unsigned char ub = b;
    unsigned char uc = c;
    unsigned char x = ub ^ uc; /* 0: yes; 1..255: no */
    uint32_t y = x;            /* 0: yes; 1..255: no */

    y -= 1;   /* 4294967295: yes; 0..254: no */
    y >>= 31; /* 1: yes; 0: no */

    return y;
}

unsigned char negative(signed char b) {
    /* 18446744073709551361..18446744073709551615: yes; 0..255: no */
    uint64_t x = b;

    x >>= 63; /* 1: yes; 0: no */

    return x;
}

void ge25519_cmov(ge25519_precomp *t, const ge25519_precomp *u, unsigned char b) {
    fe25519_cmov(t->yplusx, u->yplusx, b);
    fe25519_cmov(t->yminusx, u->yminusx, b);
    fe25519_cmov(t->xy2d, u->xy2d, b);
}

void ge25519_cmov_cached(ge25519_cached *t, const ge25519_cached *u, unsigned char b) {
    fe25519_cmov(t->YplusX, u->YplusX, b);
    fe25519_cmov(t->YminusX, u->YminusX, b);
    fe25519_cmov(t->Z, u->Z, b);
    fe25519_cmov(t->T2d, u->T2d, b);
}

/*
 r = p - q
 */

void ge25519_sub(ge25519_p1p1_1 *r, const ge25519_p3 *p, const ge25519_cached *q) {
    fe25519 t0;

    fe25519_add(r->X, p->Y, p->X);
    fe25519_sub(r->Y, p->Y, p->X);
    fe25519_mul(r->Z, r->X, q->YminusX);
    fe25519_mul(r->Y, r->Y, q->YplusX);
    fe25519_mul(r->T, q->T2d, p->T);
    fe25519_mul(r->X, p->Z, q->Z);
    fe25519_add(t0, r->X, r->X);
    fe25519_sub(r->X, r->Z, r->Y);
    fe25519_add(r->Y, r->Z, r->Y);
    fe25519_sub(r->Z, t0, r->T);
    fe25519_add(r->T, t0, r->T);
}

void ge25519_tobytes(unsigned char *s, const ge25519_p2 *h) {
    fe25519 recip;
    fe25519 x;
    fe25519 y;

    fe25519_invert(recip, h->Z);
    fe25519_mul(x, h->X, recip);
    fe25519_mul(y, h->Y, recip);
    fe25519_tobytes(s, y);
    s[31] ^= fe25519_isnegative(x) << 7;
}

/* multiply by the order of the main subgroup l = 2^252+27742317777372353535851937790883648493 */
void ge25519_mul_l(ge25519_p3 *r, const ge25519_p3 *A) {
    const signed char aslide[253] = {
        13, 0,   0, 0, 0, -1, 0,   0, 0,  0,  -11, 0, 0, 0,  0,  0,  0, -5, 0, 0,  0,  0, 0,
        0,  -3,  0, 0, 0, 0,  -13, 0, 0,  0,  0,   7, 0, 0,  0,  0,  0, 3,  0, 0,  0,  0, -13,
        0,  0,   0, 0, 5, 0,  0,   0, 0,  0,  0,   0, 0, 11, 0,  0,  0, 0,  0, 11, 0,  0, 0,
        0,  -13, 0, 0, 0, 0,  0,   0, -3, 0,  0,   0, 0, 0,  -1, 0,  0, 0,  0, 3,  0,  0, 0,
        0,  -11, 0, 0, 0, 0,  0,   0, 0,  15, 0,   0, 0, 0,  0,  -1, 0, 0,  0, 0,  -1, 0, 0,
        0,  0,   7, 0, 0, 0,  0,   5, 0,  0,  0,   0, 0, 0,  0,  0,  0, 0,  0, 0,  0,  0, 0,
        0,  0,   0, 0, 0, 0,  0,   0, 0,  0,  0,   0, 0, 0,  0,  0,  0, 0,  0, 0,  0,  0, 0,
        0,  0,   0, 0, 0, 0,  0,   0, 0,  0,  0,   0, 0, 0,  0,  0,  0, 0,  0, 0,  0,  0, 0,
        0,  0,   0, 0, 0, 0,  0,   0, 0,  0,  0,   0, 0, 0,  0,  0,  0, 0,  0, 0,  0,  0, 0,
        0,  0,   0, 0, 0, 0,  0,   0, 0,  0,  0,   0, 0, 0,  0,  0,  0, 0,  0, 0,  0,  0, 0,
        0,  0,   0, 0, 0, 0,  0,   0, 0,  0,  0,   0, 0, 0,  0,  0,  0, 0,  0, 0,  0,  0, 1};
    ge25519_cached Ai[8];
    ge25519_p1p1_1 t;
    ge25519_p3 u;
    ge25519_p3 A2;
    int i;

    ge25519_p3_to_cached(&Ai[0], A);
    ge25519_p3_dbl(&t, A);
    ge25519_p1p1_to_p3(&A2, &t);
    ge25519_add2(&t, &A2, &Ai[0]);
    ge25519_p1p1_to_p3(&u, &t);
    ge25519_p3_to_cached(&Ai[1], &u);
    ge25519_add2(&t, &A2, &Ai[1]);
    ge25519_p1p1_to_p3(&u, &t);
    ge25519_p3_to_cached(&Ai[2], &u);
    ge25519_add2(&t, &A2, &Ai[2]);
    ge25519_p1p1_to_p3(&u, &t);
    ge25519_p3_to_cached(&Ai[3], &u);
    ge25519_add2(&t, &A2, &Ai[3]);
    ge25519_p1p1_to_p3(&u, &t);
    ge25519_p3_to_cached(&Ai[4], &u);
    ge25519_add2(&t, &A2, &Ai[4]);
    ge25519_p1p1_to_p3(&u, &t);
    ge25519_p3_to_cached(&Ai[5], &u);
    ge25519_add2(&t, &A2, &Ai[5]);
    ge25519_p1p1_to_p3(&u, &t);
    ge25519_p3_to_cached(&Ai[6], &u);
    ge25519_add2(&t, &A2, &Ai[6]);
    ge25519_p1p1_to_p3(&u, &t);
    ge25519_p3_to_cached(&Ai[7], &u);

    ge25519_p3_0(r);

    for (i = 252; i >= 0; --i) {
        ge25519_p3_dbl(&t, r);

        if (aslide[i] > 0) {
            ge25519_p1p1_to_p3(&u, &t);
            ge25519_add2(&t, &u, &Ai[aslide[i] / 2]);
        } else if (aslide[i] < 0) {
            ge25519_p1p1_to_p3(&u, &t);
            ge25519_sub(&t, &u, &Ai[(-aslide[i]) / 2]);
        }

        ge25519_p1p1_to_p3(r, &t);
    }
}

int ge25519_is_on_main_subgroup(const ge25519_p3 *p) {
    ge25519_p3 pl;

    ge25519_mul_l(&pl, p);

    return fe25519_iszero(pl.X);
}

#ifndef CRYPTO_ALIGN
#if defined(__INTEL_COMPILER) || defined(_MSC_VER)
#define CRYPTO_ALIGN(x) __declspec(align(x))
#else
#define CRYPTO_ALIGN(x) __attribute__((aligned(x)))
#endif
#endif

#define COMPILER_ASSERT(X) (void)sizeof(char[(X) ? 1 : -1])

int ge25519_has_small_order(const unsigned char s[32]) {
    CRYPTO_ALIGN(16)
    const unsigned char blacklist[][32] = {
        /* 0 (order 4) */
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        /* 1 (order 1) */
        {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        /* 2707385501144840649318225287225658788936804267575313519463743609750303402022
           (order 8) */
        {0x26, 0xe8, 0x95, 0x8f, 0xc2, 0xb2, 0x27, 0xb0, 0x45, 0xc3, 0xf4,
         0x89, 0xf2, 0xef, 0x98, 0xf0, 0xd5, 0xdf, 0xac, 0x05, 0xd3, 0xc6,
         0x33, 0x39, 0xb1, 0x38, 0x02, 0x88, 0x6d, 0x53, 0xfc, 0x05},
        /* 55188659117513257062467267217118295137698188065244968500265048394206261417927
           (order 8) */
        {0xc7, 0x17, 0x6a, 0x70, 0x3d, 0x4d, 0xd8, 0x4f, 0xba, 0x3c, 0x0b,
         0x76, 0x0d, 0x10, 0x67, 0x0f, 0x2a, 0x20, 0x53, 0xfa, 0x2c, 0x39,
         0xcc, 0xc6, 0x4e, 0xc7, 0xfd, 0x77, 0x92, 0xac, 0x03, 0x7a},
        /* p-1 (order 2) */
        {0xec, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
         0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
         0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f},
        /* p (=0, order 4) */
        {0xed, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
         0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
         0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f},
        /* p+1 (=1, order 1) */
        {0xee, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
         0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
         0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f}};
    unsigned char c[7] = {0};
    unsigned int k;
    size_t i, j;

    COMPILER_ASSERT(7 == sizeof blacklist / sizeof blacklist[0]);
    for (j = 0; j < 31; j++) {
        for (i = 0; i < sizeof blacklist / sizeof blacklist[0]; i++) {
            c[i] |= s[j] ^ blacklist[i][j];
        }
    }
    for (i = 0; i < sizeof blacklist / sizeof blacklist[0]; i++) {
        c[i] |= (s[j] & 0x7f) ^ blacklist[i][j];
    }
    k = 0;
    for (i = 0; i < sizeof blacklist / sizeof blacklist[0]; i++) {
        k |= (c[i] - 1);
    }
    return (int)((k >> 8) & 1);
}