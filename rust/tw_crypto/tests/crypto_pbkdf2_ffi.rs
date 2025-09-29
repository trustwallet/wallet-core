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

    let salt = hex::decode("73616C74").unwrap();
    let salt = TWDataHelper::create(salt);

    let res =
        TWDataHelper::wrap(unsafe { tw_pbkdf2_hmac_sha256(password.ptr(), salt.ptr(), 1, 20) });
    assert_eq!(
        hex::encode(res.to_vec().unwrap(), false),
        "120fb6cffcf8b32c43e7225256c4f837a86548c9"
    );

    let res =
        TWDataHelper::wrap(unsafe { tw_pbkdf2_hmac_sha256(password.ptr(), salt.ptr(), 4096, 20) });
    assert_eq!(
        hex::encode(res.to_vec().unwrap(), false),
        "c5e478d59288c841aa530db6845c4c8d962893a0"
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

    let salt = hex::decode("73616C74").unwrap();
    let salt = TWDataHelper::create(salt);

    let res =
        TWDataHelper::wrap(unsafe { tw_pbkdf2_hmac_sha512(password.ptr(), salt.ptr(), 1, 20) });
    assert_eq!(
        hex::encode(res.to_vec().unwrap(), false),
        "867f70cf1ade02cff3752599a3a53dc4af34c7a6"
    );
}
