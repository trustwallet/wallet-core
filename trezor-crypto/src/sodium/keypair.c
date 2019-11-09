#include <string.h>

#include <TrezorCrypto/sodium/keypair.h>

/* Perform a fixed-base multiplication of the Edwards base point,
   (which is efficient due to precalculated tables), then convert
   to the Curve25519 montgomery-format public key.  In particular,
   convert Curve25519's "montgomery" x-coordinate into an Ed25519
   "edwards" y-coordinate:

   mont_x = (ed_y + 1) / (1 - ed_y)

   with projective coordinates:

   mont_x = (ed_y + ed_z) / (ed_z - ed_y)

   NOTE: ed_y=1 is converted to mont_x=0 since fe_invert is mod-exp
*/
int ed25519_pk_to_curve25519(unsigned char *curve25519_pk, const unsigned char *ed25519_pk) {
    ge25519_p3 A;
    fe25519 x;
    fe25519 one_minus_y;

    if (ge25519_has_small_order(ed25519_pk) != 0 ||
        ge25519_frombytes_negate_vartime(&A, ed25519_pk) != 0 ||
        ge25519_is_on_main_subgroup(&A) == 0) {
        return -1;
    }
    fe25519_1(one_minus_y);
    fe25519_sub(one_minus_y, one_minus_y, A.Y);
    fe25519_invert(one_minus_y, one_minus_y);
    fe25519_1(x);
    fe25519_add(x, x, A.Y);
    fe25519_mul(x, x, one_minus_y);
    fe25519_tobytes(curve25519_pk, x);

    return 0;
}

/* Convert the Curve25519 public key into an Ed25519 public key.  In
particular, convert Curve25519's "montgomery" x-coordinate into an
Ed25519 "edwards" y-coordinate:

ed_y = (mont_x - 1) / (mont_x + 1)

NOTE: mont_x=-1 is converted to ed_y=0 since fe_invert is mod-exp
*/
int curve25519_pk_to_ed25519(unsigned char *ed25519_pk, const unsigned char *curve25519_pk) {
    fe25519 x;
    fe25519 tmp;
    fe25519 one_minus_y;

    fe25519_frombytes(x, curve25519_pk);
    fe25519_1(one_minus_y);
    fe25519_add(one_minus_y, x, one_minus_y);
    fe25519_invert(one_minus_y, one_minus_y);
    fe25519_1(tmp);
    fe25519_sub(x, x, tmp);
    fe25519_mul(x, x, one_minus_y);
    fe25519_tobytes(ed25519_pk, x);

    return 0;
}