// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::ffi::crypto_box::public_key::{tw_crypto_box_public_key_delete, TWCryptoBoxPublicKey};
use crate::ffi::crypto_box::secret_key::{tw_crypto_box_secret_key_delete, TWCryptoBoxSecretKey};
use tw_memory::test_utils::tw_wrapper::{TWWrapper, WithDestructor};

pub type TWCryptoBoxSecretKeyHelper = TWWrapper<TWCryptoBoxSecretKey>;
pub type TWCryptoBoxPublicKeyHelper = TWWrapper<TWCryptoBoxPublicKey>;

impl WithDestructor for TWCryptoBoxSecretKey {
    fn destructor() -> unsafe extern "C" fn(*mut Self) {
        tw_crypto_box_secret_key_delete
    }
}

impl WithDestructor for TWCryptoBoxPublicKey {
    fn destructor() -> unsafe extern "C" fn(*mut Self) {
        tw_crypto_box_public_key_delete
    }
}
