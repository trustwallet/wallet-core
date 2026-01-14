// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_crypto::crypto_pbkdf2::{error::Error, pbkdf2_hmac_512, pbkdf2_hmac_sha256};
use tw_encoding::{base64, base64::STANDARD, hex};

#[test]
fn test_pbkdf2_hmac_sha256() {
    let password = hex::decode("70617373776f7264").unwrap();
    let salt = hex::decode("73616C74123456781123123412345678").unwrap();

    let data = pbkdf2_hmac_sha256(&password, &salt, 1, 20).unwrap();
    assert_eq!(
        hex::encode(&data, false),
        "bfa8ae076a2216e7ab8e1bb3e5cfcd20b02492fe"
    );

    let data = pbkdf2_hmac_sha256(&password, &salt, 4096, 20).unwrap();
    assert_eq!(
        hex::encode(&data, false),
        "cb8cbfe44b0cd1ebf207bbc761a1fbc1fc13ac54"
    );

    let salt2 = base64::decode("kNHS+Mx//slRsmLF9396HQ==", STANDARD).unwrap();

    let data = pbkdf2_hmac_sha256(&password, &salt2, 100, 32).unwrap();
    assert_eq!(
        hex::encode(&data, false),
        "9cf33ebd3542c691fac6f61609a8d13355a0adf4d15eed77cc9d13f792b77c3a"
    );
}

#[test]
fn test_pbkdf2_hmac_exceeds_max_output_size() {
    let password = hex::decode("70617373776f7264").unwrap();
    let salt = hex::decode("73616C74123456781123123412345678").unwrap();

    let data = pbkdf2_hmac_sha256(&password, &salt, 1, 1025);
    assert_eq!(data.err(), Some(Error::InvalidOutputSize));

    let data = pbkdf2_hmac_512(&password, &salt, 1, 1025);
    assert_eq!(data.err(), Some(Error::InvalidOutputSize));
}

#[test]
fn test_pbkdf2_hmac_exceeds_max_iterations() {
    let password = hex::decode("70617373776f7264").unwrap();
    let salt = hex::decode("73616C74").unwrap();

    let data = pbkdf2_hmac_sha256(&password, &salt, 1000001, 20);
    assert_eq!(data.err(), Some(Error::InvalidOutputSize));

    let data = pbkdf2_hmac_512(&password, &salt, 1000001, 20);
    assert_eq!(data.err(), Some(Error::InvalidOutputSize));
}

#[test]
fn test_pbkdf2_hmac_invalid_salt_size() {
    let password = hex::decode("70617373776f7264").unwrap();
    let salt = hex::decode("73616C74").unwrap();

    let data = pbkdf2_hmac_sha256(&password, &salt, 1, 20);
    assert_eq!(data.err(), Some(Error::InvalidSaltSize));

    let data = pbkdf2_hmac_512(&password, &salt, 1, 20);
    assert_eq!(data.err(), Some(Error::InvalidSaltSize));
}
