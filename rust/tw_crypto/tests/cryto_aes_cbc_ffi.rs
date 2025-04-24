// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_crypto::ffi::crypto_aes_cbc::*;
use tw_encoding::hex;
use tw_memory::ffi::tw_data::TWData;
use tw_memory::ffi::RawPtrTrait;
use tw_memory::test_utils::tw_data_helper::TWDataHelper;

#[test]
fn test_crypto_aes_cbc_encrypt_zero_padding() {
    let key =
        hex::decode("603deb1015ca71be2b73aef0857d77811f352c073b6108d72d9810a30914dff4").unwrap();
    let key = TWDataHelper::create(key);

    let iv = hex::decode("000102030405060708090A0B0C0D0E0F").unwrap();
    let iv = TWDataHelper::create(iv);

    let data = hex::decode("6bc1bee22e409f96e93d7e117393172a").unwrap();
    let data = TWDataHelper::create(data);

    let encrypt_result = unsafe { crypto_aes_cbc_encrypt(data.ptr(), iv.ptr(), key.ptr(), 0) };
    let encrypt_result = unsafe { TWData::from_ptr_as_mut(encrypt_result).unwrap() };

    assert_eq!(
        hex::encode(&encrypt_result.to_vec(), false),
        "f58c4c04d6e5f1ba779eabfb5f7bfbd6"
    );
}

#[test]
fn test_crypto_aes_cbc_decrypt_zero_padding() {
    let key =
        hex::decode("603deb1015ca71be2b73aef0857d77811f352c073b6108d72d9810a30914dff4").unwrap();
    let key = TWDataHelper::create(key);

    let iv = hex::decode("000102030405060708090A0B0C0D0E0F").unwrap();
    let iv = TWDataHelper::create(iv);

    let cipher = hex::decode("f58c4c04d6e5f1ba779eabfb5f7bfbd6").unwrap();
    let cipher = TWDataHelper::create(cipher);

    let decrypt_result = unsafe { crypto_aes_cbc_decrypt(cipher.ptr(), iv.ptr(), key.ptr(), 0) };
    let decrypt_result = unsafe { TWData::from_ptr_as_mut(decrypt_result).unwrap() };

    assert_eq!(
        hex::encode(&decrypt_result.to_vec(), false),
        "6bc1bee22e409f96e93d7e117393172a"
    );
}

#[test]
fn test_crypto_aes_cbc_encrypt_pkcs7_padding() {
    let key2 =
        hex::decode("bbc82a01ebdb14698faee4a9e5038de72c995a9f6bcdb21903d62408b0c5ca96").unwrap();
    let key2 = TWDataHelper::create(key2);

    let iv = hex::decode("37f8687086d31852979e228f4a97925b").unwrap();
    let iv = TWDataHelper::create(iv);

    let data = hex::decode("7b226a736f6e727063223a22322e30222c226964223a313535343334333833343735323434362c226572726f72223a7b22636f6465223a2d33323030302c226d657373616765223a2253657373696f6e2052656a6563746564227d7d").unwrap();
    let data = TWDataHelper::create(data);

    let encrypt_result = unsafe { crypto_aes_cbc_encrypt(data.ptr(), iv.ptr(), key2.ptr(), 1) };
    let encrypt_result = unsafe { TWData::from_ptr_as_mut(encrypt_result).unwrap() };

    assert_eq!(hex::encode(&encrypt_result.to_vec(), false), "23c75d1b3228742ddb12eeef5a5016e37a8980a77fabc6dd01e6a355d88851c611d37e0d17a2f9c30f659da6d42ba77aca9b84bd6a95e3924f47d9093fbf16e0fb55b165ec193489645b4f7d2573959305c8fa70f88fe5affc43e3084a5878d1");
}

#[test]
fn test_crypto_aes_cbc_decrypt_pkcs7_padding() {
    let key2 =
        hex::decode("bbc82a01ebdb14698faee4a9e5038de72c995a9f6bcdb21903d62408b0c5ca96").unwrap();
    let key2 = TWDataHelper::create(key2);

    let iv = hex::decode("debb62725b21c7577e4e498e10f096c7").unwrap();
    let iv = TWDataHelper::create(iv);

    let cipher = hex::decode("e7df9810ce66defcc03023ee945f5958c1d4697bf97945daeab5059c2bc6262642cbca82982ac690e77e16671770c200f348f743a7c6e5df5c74eb892ef9b45a9b5ddf0f08fa60c49e5b694688d1b0b521b43975e65b4e8d557a83f4d1aab0af").unwrap();
    let cipher = TWDataHelper::create(cipher);

    let decrypt_result = unsafe { crypto_aes_cbc_decrypt(cipher.ptr(), iv.ptr(), key2.ptr(), 1) };
    let decrypt_result = unsafe { TWData::from_ptr_as_mut(decrypt_result).unwrap() };

    assert_eq!(hex::encode(&decrypt_result.to_vec(), false), "7b226a736f6e727063223a22322e30222c226964223a313535343334333833343735323434362c226572726f72223a7b22636f6465223a2d33323030302c226d657373616765223a2253657373696f6e2052656a6563746564227d7d");
}
