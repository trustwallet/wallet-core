// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use hmac::{Hmac, Mac};
use sha2::{Sha256, Sha512};

type HmacSha256 = Hmac<Sha256>;

pub fn hmac_sha256(key: &[u8], input: &[u8]) -> Vec<u8> {
    let mut mac = HmacSha256::new_from_slice(key).expect("Hmac constructor should never fail");
    mac.update(input);
    mac.finalize().into_bytes().to_vec()
}

pub fn hmac_sha512(key: &[u8], input: &[u8]) -> Vec<u8> {
    let mut mac = Hmac::<Sha512>::new_from_slice(key).expect("Hmac constructor should never fail");
    mac.update(input);
    mac.finalize().into_bytes().to_vec()
}
