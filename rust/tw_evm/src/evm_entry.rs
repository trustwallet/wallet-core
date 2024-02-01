// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::evm_context::EvmContext;
use crate::modules::abi_encoder::AbiEncoder;
use crate::modules::rlp_encoder::RlpEncoder;
use tw_memory::Data;
use tw_proto::EthereumAbi::Proto as AbiProto;
use tw_proto::EthereumRlp::Proto as RlpProto;
use tw_proto::{deserialize, serialize, ProtoResult};

/// An EVM-compatible chain entry.
pub trait EvmEntry {
    type Context: EvmContext;

    /// Encodes an item or a list of items as Eth RLP binary format.
    #[inline]
    fn encode_rlp(input: RlpProto::EncodingInput<'_>) -> RlpProto::EncodingOutput<'static> {
        RlpEncoder::<Self::Context>::encode_with_proto(input)
    }

    /// Decodes function call data to human readable json format, according to input abi json.
    #[inline]
    fn decode_abi_contract_call(
        input: AbiProto::ContractCallDecodingInput<'_>,
    ) -> AbiProto::ContractCallDecodingOutput<'static> {
        AbiEncoder::<Self::Context>::decode_contract_call(input)
    }

    /// Decodes a function input or output data according to a given ABI.
    #[inline]
    fn decode_abi_params(
        input: AbiProto::ParamsDecodingInput<'_>,
    ) -> AbiProto::ParamsDecodingOutput {
        AbiEncoder::<Self::Context>::decode_params(input)
    }

    /// Decodes an Eth ABI value according to a given type.
    #[inline]
    fn decode_abi_value(
        input: AbiProto::ValueDecodingInput<'_>,
    ) -> AbiProto::ValueDecodingOutput<'static> {
        AbiEncoder::<Self::Context>::decode_value(input)
    }

    /// Returns the function type signature, of the form "baz(int32,uint256)".
    #[inline]
    fn get_abi_function_signature(input: AbiProto::FunctionGetTypeInput<'_>) -> String {
        AbiEncoder::<Self::Context>::get_function_signature(input)
    }

    // Encodes function inputs to Eth ABI binary.
    #[inline]
    fn encode_abi_function(
        input: AbiProto::FunctionEncodingInput<'_>,
    ) -> AbiProto::FunctionEncodingOutput<'static> {
        AbiEncoder::<Self::Context>::encode_contract_call(input)
    }
}

/// The [`EvmEntry`] trait extension.
pub trait EvmEntryExt {
    /// Encodes an item or a list of items as Eth RLP binary format.
    fn encode_rlp(&self, input: &[u8]) -> ProtoResult<Data>;

    /// Decodes function call data to human readable json format, according to input abi json.
    fn decode_abi_contract_call(&self, input: &[u8]) -> ProtoResult<Data>;

    /// Decodes a function input or output data according to a given ABI.
    fn decode_abi_params(&self, input: &[u8]) -> ProtoResult<Data>;

    /// Returns the function type signature, of the form "baz(int32,uint256)".
    fn get_abi_function_signature(&self, input: &[u8]) -> ProtoResult<String>;

    /// Encodes function inputs to Eth ABI binary.
    fn encode_abi_function(&self, input: &[u8]) -> ProtoResult<Data>;

    /// Decodes an Eth ABI value according to a given type.
    fn decode_abi_value(&self, input: &[u8]) -> ProtoResult<Data>;
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

    fn decode_abi_contract_call(&self, input: &[u8]) -> ProtoResult<Data> {
        let input = deserialize(input)?;
        let output = <Self as EvmEntry>::decode_abi_contract_call(input);
        serialize(&output)
    }

    fn decode_abi_params(&self, input: &[u8]) -> ProtoResult<Data> {
        let input = deserialize(input)?;
        let output = <Self as EvmEntry>::decode_abi_params(input);
        serialize(&output)
    }

    fn get_abi_function_signature(&self, input: &[u8]) -> ProtoResult<String> {
        let input = deserialize(input)?;
        Ok(<Self as EvmEntry>::get_abi_function_signature(input))
    }

    fn encode_abi_function(&self, input: &[u8]) -> ProtoResult<Data> {
        let input = deserialize(input)?;
        let output = <Self as EvmEntry>::encode_abi_function(input);
        serialize(&output)
    }

    fn decode_abi_value(&self, input: &[u8]) -> ProtoResult<Data> {
        let input = deserialize(input)?;
        let output = <Self as EvmEntry>::decode_abi_value(input);
        serialize(&output)
    }
}
