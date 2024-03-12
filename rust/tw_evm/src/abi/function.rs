// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::abi::decode::decode_params;
use crate::abi::encode::encode_tokens;
use crate::abi::param::Param;
use crate::abi::param_token::NamedToken;
use crate::abi::signature::short_signature;
use crate::abi::token::Token;
use crate::abi::{AbiError, AbiErrorKind, AbiResult};
use itertools::Itertools;
use serde::Deserialize;
use tw_memory::Data;

#[derive(Clone, Debug, Default, Deserialize)]
pub struct Function {
    /// Function name.
    pub name: String,
    /// Function input.
    pub inputs: Vec<Param>,
    /// Function output.
    pub outputs: Vec<Param>,
}

impl Function {
    /// Returns a signature that uniquely identifies this function.
    ///
    /// Examples:
    /// - `functionName()`
    /// - `functionName():(uint256)`
    /// - `functionName(bool):(uint256,string)`
    /// - `functionName(uint256,bytes32):(string,uint256)`
    pub fn signature(&self) -> String {
        let inputs = self.inputs.iter().map(|p| p.kind.to_type_long()).join(",");

        let outputs = self.outputs.iter().map(|p| p.kind.to_type_long()).join(",");

        match (inputs.len(), outputs.len()) {
            (_, 0) => format!("{}({inputs})", self.name),
            (_, _) => format!("{}({inputs}):({outputs})", self.name),
        }
    }

    /// Parses the ABI function input to a list of tokens.
    pub fn decode_input(&self, data: &[u8]) -> AbiResult<Vec<NamedToken>> {
        decode_params(&self.inputs, data)
    }

    /// Encodes function input to Eth ABI binary.
    pub fn encode_input(&self, tokens: &[Token]) -> AbiResult<Data> {
        // Check if the given tokens match `Self::inputs` ABI.
        let input_param_types: Vec<_> =
            self.inputs.iter().map(|param| param.kind.clone()).collect();
        for (token, kind) in tokens.iter().zip(input_param_types.iter()) {
            if token.to_param_type() != *kind {
                return Err(AbiError(AbiErrorKind::Error_abi_mismatch));
            }
        }

        let signed = short_signature(&self.name, &input_param_types);
        let encoded = encode_tokens(tokens);
        Ok(signed.into_iter().chain(encoded).collect())
    }
}
