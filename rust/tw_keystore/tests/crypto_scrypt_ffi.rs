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

    let salt =
        hex::decode("80132842c6cde8f9d04582932ef92c3cad3ba6b41e1296ef681692372886db86").unwrap();
    let salt = CByteArray::from(salt);

    let res = unsafe {
        crypto_scrypt(
            password.data(),
            password.size(),
            salt.data(),
            salt.size(),
            1 << 12,
            8,
            6,
            32,
        )
    }
    .unwrap();
    let data = unsafe { res.into_vec() };
    assert_eq!(
        hex::encode(&data, false),
        "1217705511f43b7d2faea767a156a9946c579b3436ba27252a73278a7162cedc"
    );
}
