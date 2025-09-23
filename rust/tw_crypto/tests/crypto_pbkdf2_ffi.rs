// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_crypto::ffi::crypto_pbkdf2::{tw_pbkdf2_hmac_sha256, tw_pbkdf2_hmac_sha512};
use tw_encoding::{base64, base64::STANDARD, hex};
use tw_memory::test_utils::tw_data_helper::TWDataHelper;

#[test]
fn test_pbkdf2_hmac_sha256_ffi() {
    let password = hex::decode("70617373776f7264").unwrap();
    let password = TWDataHelper::create(password);

    let salt = hex::decode("73616C74123456781123123412345678").unwrap();
    let salt = TWDataHelper::create(salt);

    let res =
        TWDataHelper::wrap(unsafe { tw_pbkdf2_hmac_sha256(password.ptr(), salt.ptr(), 1, 20) });
    assert_eq!(
        hex::encode(res.to_vec().unwrap(), false),
        "bfa8ae076a2216e7ab8e1bb3e5cfcd20b02492fe"
    );

    let res =
        TWDataHelper::wrap(unsafe { tw_pbkdf2_hmac_sha256(password.ptr(), salt.ptr(), 4096, 20) });
    assert_eq!(
        hex::encode(res.to_vec().unwrap(), false),
        "cb8cbfe44b0cd1ebf207bbc761a1fbc1fc13ac54"
    );

    let salt2 = base64::decode("kNHS+Mx//slRsmLF9396HQ==", STANDARD).unwrap();
    let salt2 = TWDataHelper::create(salt2);

    let res =
        TWDataHelper::wrap(unsafe { tw_pbkdf2_hmac_sha256(password.ptr(), salt2.ptr(), 100, 32) });
    assert_eq!(
        hex::encode(res.to_vec().unwrap(), false),
        "9cf33ebd3542c691fac6f61609a8d13355a0adf4d15eed77cc9d13f792b77c3a"
    );
}

#[test]
fn test_pbkdf2_hmac_sha512_ffi() {
    let password = hex::decode("70617373776f7264").unwrap();
    let password = TWDataHelper::create(password);

    let salt = hex::decode("73616C74123456781123123412345678").unwrap();
    let salt = TWDataHelper::create(salt);

    let res =
        TWDataHelper::wrap(unsafe { tw_pbkdf2_hmac_sha512(password.ptr(), salt.ptr(), 1, 20) });
    assert_eq!(
        hex::encode(res.to_vec().unwrap(), false),
        "d696f604810c117036005c34860ea030a53f1c50"
    );
}
