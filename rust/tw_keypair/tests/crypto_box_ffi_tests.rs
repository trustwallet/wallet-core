// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_encoding::hex::DecodeHex;
use tw_keypair::ffi::crypto_box::public_key::{
    tw_crypto_box_public_key_create_with_data, tw_crypto_box_public_key_data,
    tw_crypto_box_public_key_is_valid,
};
use tw_keypair::ffi::crypto_box::secret_key::{
    tw_crypto_box_secret_key_create, tw_crypto_box_secret_key_create_with_data,
    tw_crypto_box_secret_key_data, tw_crypto_box_secret_key_get_public_key,
    tw_crypto_box_secret_key_is_valid,
};
use tw_keypair::ffi::crypto_box::{tw_crypto_box_decrypt_easy, tw_crypto_box_encrypt_easy};
use tw_keypair::test_utils::tw_crypto_box_helpers::{
    TWCryptoBoxPublicKeyHelper, TWCryptoBoxSecretKeyHelper,
};
use tw_memory::test_utils::tw_data_helper::TWDataHelper;
use tw_memory::test_utils::tw_wrapper::TWWrapper;

fn random_key_pair() -> (TWCryptoBoxSecretKeyHelper, TWCryptoBoxPublicKeyHelper) {
    let secret = TWWrapper::wrap(unsafe { tw_crypto_box_secret_key_create() });
    let pubkey = TWWrapper::wrap(unsafe { tw_crypto_box_secret_key_get_public_key(secret.ptr()) });
    (secret, pubkey)
}

#[test]
fn test_encrypt_decrypt_easy() {
    let message = b"Well done is better than well said. -Benjamin Franklin";

    let (my_secret, my_pubkey) = random_key_pair();
    let (other_secret, other_pubkey) = random_key_pair();

    let message_data = TWDataHelper::create(message.to_vec());

    let encrypted = TWDataHelper::wrap(unsafe {
        tw_crypto_box_encrypt_easy(my_secret.ptr(), other_pubkey.ptr(), message_data.ptr())
    });

    // Step 2. Make sure the Box can be decrypted by the other side.
    let decrypted = TWDataHelper::wrap(unsafe {
        tw_crypto_box_decrypt_easy(other_secret.ptr(), my_pubkey.ptr(), encrypted.ptr())
    });

    assert_eq!(decrypted.to_vec().unwrap(), message);
}

#[test]
fn test_encrypt_decrypt_easy_error() {
    let (my_secret, _my_pubkey) = random_key_pair();

    let other_pubkey_data = TWDataHelper::create(
        "afccabc5b28a8a1fd1cd880516f9c854ae2498d0d1b978b53a59f38e4ae55747"
            .decode_hex()
            .unwrap(),
    );
    let other_pubkey = TWWrapper::wrap(unsafe {
        tw_crypto_box_public_key_create_with_data(other_pubkey_data.ptr())
    });

    // The given encrypted box cannot be decrypted by using `my_secret` and `other_pubkey`.
    let invalid_encrypted = "7a7b9c8fee6e3c597512848c7d513e7131193cdfd410ff6611522fdeea99d7160873182019d7a18502f22c5e3644d26a2b669365".decode_hex().unwrap();
    let invalid_encrypted_data = TWDataHelper::create(invalid_encrypted);

    let decrypted = TWDataHelper::wrap(unsafe {
        tw_crypto_box_decrypt_easy(
            my_secret.ptr(),
            other_pubkey.ptr(),
            invalid_encrypted_data.ptr(),
        )
    });

    assert!(decrypted.is_null());
}

#[test]
fn test_public_key() {
    let pubkey_bytes = "afccabc5b28a8a1fd1cd880516f9c854ae2498d0d1b978b53a59f38e4ae55747"
        .decode_hex()
        .unwrap();

    let pubkey_data = TWDataHelper::create(pubkey_bytes.clone());
    assert!(unsafe { tw_crypto_box_public_key_is_valid(pubkey_data.ptr()) });

    let pubkey =
        TWWrapper::wrap(unsafe { tw_crypto_box_public_key_create_with_data(pubkey_data.ptr()) });
    let actual_data = TWDataHelper::wrap(unsafe { tw_crypto_box_public_key_data(pubkey.ptr()) });
    assert_eq!(actual_data.to_vec().unwrap(), pubkey_bytes);
}

#[test]
fn test_secret_key() {
    let secret_bytes = "dd87000d4805d6fbd89ae1352f5e4445648b79d5e901c92aebcb610e9be468e4"
        .decode_hex()
        .unwrap();

    let secret_data = TWDataHelper::create(secret_bytes.clone());
    assert!(unsafe { tw_crypto_box_secret_key_is_valid(secret_data.ptr()) });

    let pubkey =
        TWWrapper::wrap(unsafe { tw_crypto_box_secret_key_create_with_data(secret_data.ptr()) });
    let actual_data = TWDataHelper::wrap(unsafe { tw_crypto_box_secret_key_data(pubkey.ptr()) });
    assert_eq!(actual_data.to_vec().unwrap(), secret_bytes);
}
