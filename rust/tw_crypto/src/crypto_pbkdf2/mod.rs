// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use pbkdf2::pbkdf2_hmac;
use sha2::Sha256;

pub fn pbkdf2_hmac_sha256(
    password: &[u8],
    salt: &[u8],
    iterations: u32,
    desired_len: usize,
) -> Vec<u8> {
    let mut output = vec![0u8; desired_len];
    pbkdf2_hmac::<Sha256>(password, salt, iterations, &mut output);
    output
}
