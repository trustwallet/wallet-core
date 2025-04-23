// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_crypto::ffi::crypto_aes_ctr::*;
use tw_encoding::hex;
use tw_memory::ffi::tw_data::TWData;
use tw_memory::ffi::RawPtrTrait;
use tw_memory::test_utils::tw_data_helper::TWDataHelper;

#[test]
fn test_crypto_aes_ctr_128_ffi() {
    let data_hex = "6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e5130c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710";
    let data = hex::decode(data_hex).unwrap();
    let data = TWDataHelper::create(data);
    let iv = hex::decode("f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff").unwrap();
    let iv = TWDataHelper::create(iv);
    let key = hex::decode("2b7e151628aed2a6abf7158809cf4f3c").unwrap();
    let key = TWDataHelper::create(key);

    let encrypted = unsafe { crypto_aes_ctr_encrypt_128(data.ptr(), iv.ptr(), key.ptr()) };
    let encrypted = unsafe { TWData::from_ptr_as_mut(encrypted).unwrap() };

    let decrypted = unsafe { crypto_aes_ctr_decrypt_128(encrypted, iv.ptr(), key.ptr()) };
    let decrypted = unsafe { TWData::from_ptr_as_mut(decrypted).unwrap() };

    let decrypted_data = decrypted.to_vec();
    assert_eq!(hex::encode(&decrypted_data, false), data_hex);
}

#[test]
fn test_crypto_aes_ctr_192_ffi() {
    let data_hex = "6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e5130c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710";
    let data = hex::decode(data_hex).unwrap();
    let data = TWDataHelper::create(data);
    let iv = hex::decode("f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff").unwrap();
    let iv = TWDataHelper::create(iv);
    let key = hex::decode("8e73b0f7da0e6452c810f32b809079e562f8ead2522c6b7b").unwrap();
    let key = TWDataHelper::create(key);

    let encrypted = unsafe { crypto_aes_ctr_encrypt_192(data.ptr(), iv.ptr(), key.ptr()) };
    let encrypted = unsafe { TWData::from_ptr_as_mut(encrypted).unwrap() };

    let decrypted = unsafe { crypto_aes_ctr_decrypt_192(encrypted, iv.ptr(), key.ptr()) };
    let decrypted = unsafe { TWData::from_ptr_as_mut(decrypted).unwrap() };

    let decrypted_data = decrypted.to_vec();
    assert_eq!(hex::encode(&decrypted_data, false), data_hex);
}

#[test]
fn test_crypto_aes_ctr_256_ffi() {
    let data_hex = "6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e5130c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710";
    let data = hex::decode(data_hex).unwrap();
    let data = TWDataHelper::create(data);
    let iv = hex::decode("f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff").unwrap();
    let iv = TWDataHelper::create(iv);
    let key =
        hex::decode("603deb1015ca71be2b73aef0857d77811f352c073b6108d72d9810a30914dff4").unwrap();
    let key = TWDataHelper::create(key);

    let encrypted: *mut TWData =
        unsafe { crypto_aes_ctr_encrypt_256(data.ptr(), iv.ptr(), key.ptr()) };
    let encrypted = unsafe { TWData::from_ptr_as_mut(encrypted).unwrap() };

    let decrypted = unsafe { crypto_aes_ctr_decrypt_256(encrypted, iv.ptr(), key.ptr()) };
    let decrypted = unsafe { TWData::from_ptr_as_mut(decrypted).unwrap() };

    let decrypted_data = decrypted.to_vec();
    assert_eq!(hex::encode(&decrypted_data, false), data_hex);
}
