// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::abi::param::Param;
use crate::abi::param_token::ParamToken;
use crate::abi::param_type::ParamType;
use crate::abi::token::Token;
use crate::abi::{AbiError, AbiResult};
use lazy_static::lazy_static;
use tw_number::U256;

lazy_static! {
    // 0x0000000000000000000000000000000000000000000000000000000000000020
    static ref DEFAULT_DYNAMIC_OFFSET: [u8; U256::BYTES] = U256::from(32u32).to_big_endian().take();
}

pub fn decode_params(params: &[Param], data: &[u8]) -> AbiResult<Vec<ParamToken>> {
    let ethabi_types: Vec<_> = params.iter().map(|p| p.ethabi_type()).collect();
    let ethabi_tokens =
        ethabi::decode(&ethabi_types, data).map_err(|_| AbiError::InvalidEncodedData)?;
    params
        .iter()
        .zip(ethabi_tokens.into_iter())
        .map(|(param, ethabi_token)| ParamToken::with_ethabi_token(param, ethabi_token))
        .collect::<AbiResult<Vec<_>>>()
}

pub fn decode_value(param_type: &ParamType, data: &[u8]) -> AbiResult<Token> {
    let ethabi_type = param_type.to_ethabi();

    let tokens = if ethabi_type.is_dynamic() {
        // If the token is dynamic, we need to append a dynamic offset that points to the `data`.
        let mut encoded = Vec::with_capacity(DEFAULT_DYNAMIC_OFFSET.len() + data.len());
        encoded.extend_from_slice(&DEFAULT_DYNAMIC_OFFSET[..]);
        encoded.extend_from_slice(&data);

        ethabi::decode(&[ethabi_type], &encoded)
    } else {
        ethabi::decode(&[ethabi_type], &data)
    }
    .map_err(|_| AbiError::InvalidEncodedData)?;

    // Expected exactly one token.
    let ethabi_token = tokens.into_iter().next().ok_or(AbiError::Internal)?;
    Token::with_ethabi_token(param_type, ethabi_token)
}
