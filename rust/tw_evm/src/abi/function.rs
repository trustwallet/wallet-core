// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::abi::decode::decode_params;
use crate::abi::param::Param;
use crate::abi::param_token::NamedToken;
use crate::abi::token::Token;
use crate::abi::{AbiError, AbiErrorKind, AbiResult};
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
    /// Whether the function reads or modifies blockchain state.
    #[serde(rename = "stateMutability", default)]
    pub state_mutability: ethabi::StateMutability,
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
        self.to_ethabi_function().signature()
    }

    /// Parses the ABI function input to a list of tokens.
    pub fn decode_input(&self, data: &[u8]) -> AbiResult<Vec<NamedToken>> {
        decode_params(&self.inputs, data)
    }

    /// Encodes function input to Eth ABI binary.
    pub fn encode_input<I>(&self, tokens: I) -> AbiResult<Data>
    where
        I: IntoIterator<Item = Token>,
    {
        let ethabi_tokens: Vec<_> = tokens.into_iter().map(Token::into_ethabi_token).collect();
        self.to_ethabi_function()
            .encode_input(&ethabi_tokens)
            // Given tokens don't match the ABI.
            .map_err(|_| AbiError(AbiErrorKind::Error_abi_mismatch))
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
