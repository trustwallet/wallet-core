// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_encoding::hex;
use tw_keystore::ffi::crypto_scrypt::crypto_scrypt;
use tw_memory::ffi::c_byte_array::CByteArray;

#[test]
fn test_crypto_scrypt_ffi_null_salt() {
    let password = hex::decode("70617373776f7264").unwrap();
    let password = CByteArray::from(password);

    let salt = CByteArray::null();

    let res = unsafe {
        crypto_scrypt(
            password.data(),
            password.size(),
            salt.data(),
            salt.size(),
            16384,
            8,
            4,
            32,
        )
    }
    .unwrap();
    let data = unsafe { res.into_vec() };
    assert_eq!(
        hex::encode(&data, false),
        "004f57df809101216a343d6215879a9a7f1d7e2c04ef2845b4494cf5f10181a1"
    );
}
