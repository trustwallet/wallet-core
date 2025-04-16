// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_encoding::hex;
use tw_crypto::crypto_scrypt::params::Params;
use tw_crypto::crypto_scrypt::scrypt;

#[test]
fn test_scrypt() {
    let password = hex::decode("70617373776f7264").unwrap();
    let salt =
        hex::decode("80132842c6cde8f9d04582932ef92c3cad3ba6b41e1296ef681692372886db86").unwrap();

    let params = Params {
        n: 1 << 12,
        r: 8,
        p: 6,
        desired_len: 32,
    };
    let res = scrypt(&password, &salt, &params).unwrap();
    assert_eq!(
        hex::encode(&res, false),
        "1217705511f43b7d2faea767a156a9946c579b3436ba27252a73278a7162cedc"
    );
}

#[test]
fn test_scrypt_invalid_n() {
    let password = hex::decode("70617373776f7264").unwrap();
    let salt =
        hex::decode("80132842c6cde8f9d04582932ef92c3cad3ba6b41e1296ef681692372886db86").unwrap();

    let params = Params {
        // Must be a power of 2.
        n: (1 << 12) + 1,
        r: 8,
        p: 6,
        desired_len: 32,
    };
    scrypt(&password, &salt, &params).unwrap_err();
}
