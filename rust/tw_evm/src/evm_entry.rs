// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_memory::Data;
use tw_proto::{deserialize, serialize, MessageRead, MessageWrite, ProtoResult};

/// An EVM-compatible chain entry.
pub trait EvmEntry {
    type RlpEncodingInput<'a>: MessageRead<'a>;
    type RlpEncodingOutput: MessageWrite;

    type DecodeContractCallInput<'a>: MessageRead<'a>;
    type DecodeContractCallOutput: MessageWrite;

    type DecodeParamsInput<'a>: MessageRead<'a>;
    type DecodeParamsOutput: MessageWrite;

    type GetFunctionSignatureInput<'a>: MessageRead<'a>;

    /// Encodes an item or a list of items as Eth RLP binary format.
    fn encode_rlp(input: Self::RlpEncodingInput<'_>) -> Self::RlpEncodingOutput;

    /// Decodes function call data to human readable json format, according to input abi json.
    fn decode_contract_call(
        input: Self::DecodeContractCallInput<'_>,
    ) -> Self::DecodeContractCallOutput;

    /// Decodes a function input or output data according to a given ABI.
    fn decode_params(input: Self::DecodeParamsInput<'_>) -> Self::DecodeParamsOutput;

    /// Returns the function type signature, of the form "baz(int32,uint256)".
    fn get_function_signature(input: Self::GetFunctionSignatureInput<'_>) -> String;
}

/// The [`EvmEntry`] trait extension.
pub trait EvmEntryExt {
    /// Encodes an item or a list of items as Eth RLP binary format.
    fn encode_rlp(&self, input: &[u8]) -> ProtoResult<Data>;

    /// Decodes function call data to human readable json format, according to input abi json.
    fn decode_contract_call(&self, input: &[u8]) -> ProtoResult<Data>;

    /// Decodes a function input or output data according to a given ABI.
    fn decode_params(&self, input: &[u8]) -> ProtoResult<Data>;

    /// Returns the function type signature, of the form "baz(int32,uint256)".
    fn get_function_signature(&self, input: &[u8]) -> ProtoResult<String>;
}

impl<T> EvmEntryExt for T
where
    T: EvmEntry,
{
    fn encode_rlp(&self, input: &[u8]) -> ProtoResult<Data> {
        let input = deserialize(input)?;
        let output = <Self as EvmEntry>::encode_rlp(input);
        serialize(&output)
    }

    fn decode_contract_call(&self, input: &[u8]) -> ProtoResult<Data> {
        let input = deserialize(input)?;
        let output = <Self as EvmEntry>::decode_contract_call(input);
        serialize(&output)
    }

    fn decode_params(&self, input: &[u8]) -> ProtoResult<Data> {
        let input = deserialize(input)?;
        let output = <Self as EvmEntry>::decode_params(input);
        serialize(&output)
    }

    fn get_function_signature(&self, input: &[u8]) -> ProtoResult<String> {
        let input = deserialize(input)?;
        Ok(<Self as EvmEntry>::get_function_signature(input))
    }
}
