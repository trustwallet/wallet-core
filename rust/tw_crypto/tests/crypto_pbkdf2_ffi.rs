// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_crypto::ffi::crypto_pbkdf2::crypto_pbkdf2;
use tw_encoding::{base64, base64::STANDARD, hex};
use tw_memory::ffi::c_byte_array::CByteArray;

#[test]
fn test_crypto_pbkdf2_ffi() {
    let password = hex::decode("70617373776f7264").unwrap();
    let password = CByteArray::from(password);

    let salt = hex::decode("73616C74").unwrap();
    let salt = CByteArray::from(salt);

    let res = unsafe {
        crypto_pbkdf2(
            password.data(),
            password.size(),
            salt.data(),
            salt.size(),
            1,
            20,
        )
    };
    let data = unsafe { res.into_vec() };
    assert_eq!(
        hex::encode(&data, false),
        "120fb6cffcf8b32c43e7225256c4f837a86548c9"
    );

    let res = unsafe {
        crypto_pbkdf2(
            password.data(),
            password.size(),
            salt.data(),
            salt.size(),
            4096,
            20,
        )
    };
    let data = unsafe { res.into_vec() };
    assert_eq!(
        hex::encode(&data, false),
        "c5e478d59288c841aa530db6845c4c8d962893a0"
    );

    let salt2 = base64::decode("kNHS+Mx//slRsmLF9396HQ==", STANDARD).unwrap();
    let salt2 = CByteArray::from(salt2);

    let res = unsafe {
        crypto_pbkdf2(
            password.data(),
            password.size(),
            salt2.data(),
            salt2.size(),
            100,
            32,
        )
    };
    let data = unsafe { res.into_vec() };
    assert_eq!(
        hex::encode(&data, false),
        "9cf33ebd3542c691fac6f61609a8d13355a0adf4d15eed77cc9d13f792b77c3a"
    );
}
