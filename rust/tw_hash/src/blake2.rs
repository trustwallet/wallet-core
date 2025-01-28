// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::Error;
use blake2b_ref::Blake2bBuilder;
use std::ops::RangeInclusive;

pub const OUTPUT_HASH_LEN_RANGE: RangeInclusive<usize> = 1..=64;
pub const PERSONAL_INPUT_MAX_LEN: usize = 16;

pub fn blake2_b(input: &[u8], hash_size: usize) -> Result<Vec<u8>, Error> {
    verify_hash_size(hash_size)?;

    let mut hasher = Blake2bBuilder::new(hash_size).build();
    hasher.update(input);
    let mut buf = vec![0; hash_size];
    hasher.finalize(&mut buf);
    Ok(buf)
}

pub fn blake2_b_personal(
    input: &[u8],
    hash_size: usize,
    personal_input: &[u8],
) -> Result<Vec<u8>, Error> {
    verify_hash_size(hash_size)?;
    verify_personal(personal_input)?;

    let mut output: Vec<u8> = vec![0; hash_size];
    let mut blake2b = Blake2bBuilder::new(hash_size)
        .personal(personal_input)
        .build();
    blake2b.update(input);
    blake2b.finalize(&mut output);
    Ok(output)
}

pub fn verify_hash_size(hash_size: usize) -> Result<(), Error> {
    if OUTPUT_HASH_LEN_RANGE.contains(&hash_size) {
        Ok(())
    } else {
        Err(Error::InvalidHashLength)
    }
}

pub fn verify_personal(personal: &[u8]) -> Result<(), Error> {
    if personal.len() <= PERSONAL_INPUT_MAX_LEN {
        Ok(())
    } else {
        Err(Error::InvalidArgument)
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_blake2_b_invalid_hash_size() {
        let input = b"Hello world";
        blake2_b(input, 65).unwrap_err();
    }

    #[test]
    fn test_blake2_b_personal_invalid_hash_size() {
        let input = b"Hello world";
        let personal_data = b"MyApp Files Hash";
        blake2_b_personal(input, 65, personal_data).unwrap_err();
    }

    #[test]
    fn test_blake2_b_personal_invalid_personal_data() {
        let input = b"Hello world";
        let personal_data = b"MyApp Files Hash ...";
        blake2_b_personal(input, 64, personal_data).unwrap_err();
    }
}
