// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::abi::decode::decode_params;
use crate::abi::param::Param;
use crate::abi::param_token::ParamToken;
use crate::abi::{AbiError, AbiResult};
use serde::Deserialize;
use tw_memory::Data;

#[derive(Debug, Default, Deserialize)]
pub struct Function {
    /// Function name.
    pub name: String,
    /// Function input.
    pub inputs: Vec<Param>,
    /// Function output.
    pub outputs: Vec<Param>,
    /// Whether the function reads or modifies blockchain state.
    #[serde(rename = "stateMutability", default)]
    pub state_mutability: ethabi::StateMutability,
}

impl Function {
    /// Returns a signature that uniquely identifies this function including `inputs` **only**.
    ///
    /// Examples:
    /// - `functionName()`
    /// - `functionName()`
    /// - `functionName(bool)`
    /// - `functionName(uint256,bytes32)`
    ///
    /// The method implementation is inspired by
    /// https://github.com/rust-ethereum/ethabi/blob/b1710adc18f5b771d2d2519c87248b1ba9430778/ethabi/src/function.rs#L88-L97
    pub fn signature_with_inputs(&self) -> String {
        let inputs = self
            .inputs
            .iter()
            .map(|p| p.kind.to_ethabi().to_string())
            .collect::<Vec<_>>()
            .join(",");
        format!("{}({inputs})", self.name)
    }

    /// Parses the ABI function input to a list of tokens.
    pub fn decode_input(&self, data: &[u8]) -> AbiResult<Vec<ParamToken>> {
        decode_params(&self.inputs, data)
    }

    /// Encodes function input to Eth ABI binary.
    pub fn encode_input(&self, params: Vec<ParamToken>) -> AbiResult<Data> {
        let ethabi_tokens: Vec<_> = params
            .into_iter()
            .map(|param| param.value.into_ethabi_token())
            .collect();

        self.to_ethabi_function()
            .encode_input(&ethabi_tokens)
            .map_err(|_| AbiError::InvalidParams)
    }

    pub(crate) fn to_ethabi_function(&self) -> ethabi::Function {
        let inputs = self
            .inputs
            .iter()
            .map(Param::to_ethabi_param)
            .collect::<Vec<_>>();
        let outputs = self
            .outputs
            .iter()
            .map(Param::to_ethabi_param)
            .collect::<Vec<_>>();

        #[allow(deprecated)]
        ethabi::Function {
            name: self.name.clone(),
            inputs,
            outputs,
            state_mutability: self.state_mutability,
            constant: None,
        }
    }
}
