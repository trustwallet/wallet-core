// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::abi::param_type::ParamType;
use tw_hash::sha3::keccak256;
use tw_hash::{H256, H32};

/// Returns the first four bytes of the Keccak-256 hash of the signature of the given params.
pub fn short_signature(name: &str, params: &[ParamType]) -> H32 {
    let mut result = H32::default();
    fill_signature(name, params, result.as_mut());
    result
}

/// Returns the full Keccak-256 hash of the signature of the given params.
pub fn long_signature(name: &str, params: &[ParamType]) -> H256 {
    let mut result = H256::default();
    fill_signature(name, params, result.as_mut());
    result
}

fn fill_signature(name: &str, params: &[ParamType], result: &mut [u8]) {
    let types = params
        .iter()
        .map(ParamType::to_type_long)
        .collect::<Vec<String>>()
        .join(",");

    let data: Vec<u8> = From::from(format!("{name}({types})").as_str());

    result.copy_from_slice(&keccak256(&data)[..result.len()])
}
