#ifndef __ED25519_REF10_H__
#define __ED25519_REF10_H__

#include <stddef.h>
#include <stdint.h>

#include "ed25519_ref10_fe_25_5.h"
#include "fe_25_5/fe.h"

#ifdef __cplusplus
extern "C" {
#endif

void fe25519_invert(fe25519 out, const fe25519 z);

/*
 ge means group element.

 Here the group is the set of pairs (x,y) of field elements
 satisfying -x^2 + y^2 = 1 + d x^2y^2
 where d = -121665/121666.

 Representations:
 ge25519_p2 (projective): (X:Y:Z) satisfying x=X/Z, y=Y/Z
 ge25519_p3 (extended): (X:Y:Z:T) satisfying x=X/Z, y=Y/Z, XY=ZT
 ge25519_p1p1_1 (completed): ((X:Z),(Y:T)) satisfying x=X/Z, y=Y/T
 ge25519_precomp (Duif): (y+x,y-x,2dxy)
 */

typedef struct {
    fe25519 X;
    fe25519 Y;
    fe25519 Z;
} ge25519_p2;

typedef struct {
    fe25519 X;
    fe25519 Y;
    fe25519 Z;
    fe25519 T;
} ge25519_p3;

typedef struct {
    fe25519 X;
    fe25519 Y;
    fe25519 Z;
    fe25519 T;
} ge25519_p1p1_1;

typedef struct {
    fe25519 yplusx;
    fe25519 yminusx;
    fe25519 xy2d;
} ge25519_precomp;

typedef struct {
    fe25519 YplusX;
    fe25519 YminusX;
    fe25519 Z;
    fe25519 T2d;
} ge25519_cached;

void ge25519_tobytes(unsigned char *s, const ge25519_p2 *h);

void ge25519_p3_tobytes(unsigned char *s, const ge25519_p3 *h);

int ge25519_frombytes_negate_vartime(ge25519_p3 *h, const unsigned char *s);

void ge25519_p3_to_cached(ge25519_cached *r, const ge25519_p3 *p);

void ge25519_p1p1_to_p3(ge25519_p3 *r, const ge25519_p1p1_1 *p);

void ge25519_add2(ge25519_p1p1_1 *r, const ge25519_p3 *p, const ge25519_cached *q);

void ge25519_sub(ge25519_p1p1_1 *r, const ge25519_p3 *p, const ge25519_cached *q);

int ge25519_is_on_main_subgroup(const ge25519_p3 *p);

int ge25519_has_small_order(const unsigned char s[32]);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
