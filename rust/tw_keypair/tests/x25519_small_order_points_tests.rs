// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_encoding::hex;
use tw_keypair::nacl_crypto_box::public_key::PublicKey;

#[test]
fn test_x25519_small_order_points() {
    let small_order_points = [
        // The four canonical small-order Montgomery u-coordinates derived from
        // the 8 Edwards torsion points (each ±Edwards pair shares one u-coordinate):
        "0000000000000000000000000000000000000000000000000000000000000000", // order 1 – identity (u = 0)
        "0100000000000000000000000000000000000000000000000000000000000000", // order 2
        "5f9c95bca3508c24b1d0b1559c83ef5b04445cc4581c8e86d8224eddd09f1157", // order 4
        "e0eb7a7c3b41b8ae1656e3faf19fc46ada098deb9c32b1fd866205165f49b800", // order 8
        // Non-canonical encoding of the identity: u = p ≡ 0 (mod p).
        // In little-endian, the field prime p = 2^255-19 is encoded as edfff...7f.
        // curve25519_dalek correctly identifies this as small-order via is_small_order().
        "edffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff7f", // u = p ≡ 0 (mod p), non-canonical identity
    ];

    for point_hex in small_order_points {
        let point = hex::decode(point_hex).expect("Invalid hex in test data");
        let result = PublicKey::try_from(point.as_slice());
        assert!(
            result.is_err(),
            "Expected error for small order point: {point_hex}"
        );
    }
}

#[test]
fn test_x25519_invalid_encoded_points() {
    // These u-coordinates have no valid Edwards birational image (to_edwards() returns None
    // for both sign bits). They are NOT small-order points; they are simply invalid/
    // non-canonical u-coordinates that do not correspond to any point on the Edwards curve.
    let invalid_encoded_points = [
        "0000000000000000000000000000000000000000000000000000000000000080", // high bit set (non-canonical)
        "0100000000000000000000000000000000000000000000000000000000000080", // high bit set (non-canonical)
        "edffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff", // high bit set (non-canonical)
        // u = 2^255 - 64: not a quadratic residue mod p, so (u³+Au²+u) has no square root –
        // this u-coordinate does not lie on the Montgomery curve and has no Edwards image.
        "c0ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff7f",
    ];

    for point_hex in invalid_encoded_points {
        let point = hex::decode(point_hex).expect("Invalid hex in test data");
        let result = PublicKey::try_from(point.as_slice());
        assert!(
            result.is_err(),
            "Expected error for invalid encoded point: {point_hex}"
        );
    }
}
