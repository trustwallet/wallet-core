// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

pub mod error;

use error::Error;
use error::Result;
use pbkdf2::pbkdf2_hmac;
use sha2::{Sha256, Sha512};

const MIN_SALT_SIZE: usize = 16;
const MAX_OUTPUT_SIZE: usize = 1024;
const MAX_ITERATIONS: u32 = 1000000;

pub fn pbkdf2_hmac_sha256(
    password: &[u8],
    salt: &[u8],
    iterations: u32,
    desired_len: usize,
) -> Result<Vec<u8>> {
    if desired_len > MAX_OUTPUT_SIZE || iterations > MAX_ITERATIONS {
        return Err(Error::InvalidOutputSize);
    }
    if salt.len() < MIN_SALT_SIZE {
        return Err(Error::InvalidSaltSize);
    }

    let mut output = vec![0u8; desired_len];
    pbkdf2_hmac::<Sha256>(password, salt, iterations, &mut output);
    Ok(output)
}

pub fn pbkdf2_hmac_512(
    password: &[u8],
    salt: &[u8],
    iterations: u32,
    desired_len: usize,
) -> Result<Vec<u8>> {
    if desired_len > MAX_OUTPUT_SIZE || iterations > MAX_ITERATIONS {
        return Err(Error::InvalidOutputSize);
    }
    if salt.len() < MIN_SALT_SIZE {
        return Err(Error::InvalidSaltSize);
    }

    let mut output = vec![0u8; desired_len];
    pbkdf2_hmac::<Sha512>(password, salt, iterations, &mut output);
    Ok(output)
}
