// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::H512;
use sha2::Sha512;

pub fn pbkdf2_hmac_sha512(password: &[u8], salt: &[u8], rounds: u32) -> H512 {
    pbkdf2::pbkdf2_hmac_array::<Sha512, { H512::LEN }>(password, salt, rounds).into()
}
