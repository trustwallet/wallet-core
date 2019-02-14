#ifdef __cplusplus
extern "C" {
#endif

extern const ge25519 ALIGN(16) ge25519_basepoint;

/*
	d
*/

extern const bignum25519 ALIGN(16) ge25519_ecd;

extern const bignum25519 ALIGN(16) ge25519_ec2d;

/*
	sqrt(-1)
*/

extern const bignum25519 ALIGN(16) ge25519_sqrtneg1;

extern const ge25519_niels ALIGN(16) ge25519_niels_sliding_multiples[32];

#ifdef __cplusplus
} /* extern "C" */
#endif
