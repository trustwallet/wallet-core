// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Monero/Address.h"

using namespace TW::Monero;


typedef int crypto_int32;
typedef unsigned int crypto_uint32;
typedef int64_t crypto_int64;

typedef crypto_int32 fe[10];

typedef int32_t fe[10];

typedef struct {
    fe X;
    fe Y;
    fe Z;
} ge_p2;

typedef struct {
    fe X;
    fe Y;
    fe Z;
    fe T;
} ge_p3;

typedef struct {
    fe X;
    fe Y;
    fe Z;
    fe T;
} ge_p1p1;

typedef struct {
    fe yplusx;
    fe yminusx;
    fe xy2d;
} ge_precomp;

typedef struct {
    fe YplusX;
    fe YminusX;
    fe Z;
    fe T2d;
} ge_cached;

void sc_reduce32(unsigned char *);
int sc_check(const unsigned char *);

uint64_t load_3(const unsigned char *in);
uint64_t load_4(const unsigned char *in);

extern const ge_precomp ge_base[32][8];
void ge_scalarmult_base(ge_p3 *, const unsigned char *);
void ge_scalarmult(ge_p2 *, const unsigned char *, const ge_p3 *);


extern void fe_0(fe);
extern void fe_1(fe);
extern void fe_add(fe,const fe,const fe);
extern void fe_sub(fe,const fe,const fe);
extern void fe_mul(fe,const fe,const fe);
extern void fe_cmov(fe,const fe,unsigned int);
extern void fe_copy(fe,const fe);
extern void fe_neg(fe,const fe);
extern void fe_sq(fe,const fe);
extern void fe_sq2(fe,const fe);
extern void fe_invert(fe,const fe);
extern void fe_tobytes(unsigned char *,const fe);
static void fe_divpowm1(fe r, const fe u, const fe v);
extern int fe_isnegative(const fe);
static int fe_isnonzero(const fe);
extern const fe fe_d;
extern const fe fe_d2;
extern const fe fe_sqrtm1;

void ge_add(ge_p1p1 *, const ge_p3 *, const ge_cached *);
extern void ge_madd(ge_p1p1 *,const ge_p3 *,const ge_precomp *);
extern void ge_precomp_0(ge_precomp *);
extern void ge_p3_0(ge_p3 *);
void ge_mul8(ge_p1p1 *, const ge_p2 *);
void ge_p2_dbl(ge_p1p1 *, const ge_p2 *);
extern void ge_p3_dbl(ge_p1p1 *,const ge_p3 *);
extern void ge_p3_to_p2(ge_p2 *,const ge_p3 *);
extern void ge_p3_tobytes(unsigned char *,const ge_p3 *);
void ge_p1p1_to_p2(ge_p2 *, const ge_p1p1 *);
void ge_p1p1_to_p3(ge_p3 *, const ge_p1p1 *);
int ge_frombytes_vartime(ge_p3 *, const unsigned char *);
void ge_tobytes(unsigned char *, const ge_p2 *);
void ge_p3_to_cached(ge_cached *, const ge_p3 *);
static void ge_cached_0(ge_cached *r);
static void ge_cached_cmov(ge_cached *t, const ge_cached *u, unsigned char b);
static void ge_p2_0(ge_p2 *h);



static void select(ge_precomp *t,int pos,signed char b);

static unsigned char equal(signed char b,signed char c);

static unsigned char negative(signed char b);

static void cmov(ge_precomp *t,ge_precomp *u,unsigned char b);