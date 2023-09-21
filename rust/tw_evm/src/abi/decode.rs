// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::abi::param::Param;
use crate::abi::param_token::ParamToken;
use crate::abi::{AbiError, AbiResult};

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
