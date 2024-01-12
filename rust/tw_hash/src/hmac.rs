// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use hmac::{Hmac, Mac};
use sha2::Sha256;

type HmacSha256 = Hmac<Sha256>;

pub fn hmac_sha256(key: &[u8], input: &[u8]) -> Vec<u8> {
    let mut mac = HmacSha256::new_from_slice(key).unwrap();
    mac.update(input);
    let res = mac.finalize();
    let code_bytes = res.into_bytes();
    code_bytes.to_vec()
}
