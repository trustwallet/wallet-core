// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::abi::decode::{decode_params, decode_value};
use crate::abi::function::Function;
use crate::abi::param::Param;
use crate::abi::param_token::NamedToken;
use crate::abi::param_type::ParamType;
use crate::abi::token::Token;
use crate::abi::{AbiError, AbiErrorKind, AbiResult};
use crate::abi_output_error;
use crate::address::Address;
use crate::evm_context::EvmContext;
use serde::Deserialize;
use serde::Serialize;
use std::borrow::Cow;
use std::collections::HashMap;
use std::marker::PhantomData;
use std::str::FromStr;
use tw_encoding::hex::as_hex;
use tw_hash::H32;
use tw_misc::traits::ToBytesVec;
use tw_number::{I256, U256};
use tw_proto::EthereumAbi::Proto;

use crate::abi::non_empty_array::{NonEmptyArray, NonEmptyBytes, NonZeroLen};
use crate::abi::uint::UintBits;
use tw_coin_entry::error::prelude::*;
use Proto::mod_ParamType::OneOfparam as ProtoParamType;
use Proto::mod_ParamsDecodingInput::OneOfabi as AbiEnum;
use Proto::mod_Token::OneOftoken as TokenEnum;

pub struct AbiEncoder<Context: EvmContext> {
    _phantom: PhantomData<Context>,
}

impl<Context: EvmContext> AbiEncoder<Context> {
    #[inline]
    pub fn decode_contract_call(
        input: Proto::ContractCallDecodingInput,
    ) -> Proto::ContractCallDecodingOutput<'static> {
        Self::decode_contract_call_impl(input)
            .unwrap_or_else(|err| abi_output_error!(Proto::ContractCallDecodingOutput, err))
    }

    #[inline]
    pub fn decode_params(
        input: Proto::ParamsDecodingInput<'_>,
    ) -> Proto::ParamsDecodingOutput<'static> {
        Self::decode_params_impl(input)
            .unwrap_or_else(|err| abi_output_error!(Proto::ParamsDecodingOutput, err))
    }

    #[inline]
    pub fn decode_value(
        input: Proto::ValueDecodingInput<'_>,
    ) -> Proto::ValueDecodingOutput<'static> {
        Self::decode_value_impl(input)
            .unwrap_or_else(|err| abi_output_error!(Proto::ValueDecodingOutput, err))
    }

    #[inline]
    pub fn get_function_signature(input: Proto::FunctionGetTypeInput<'_>) -> String {
        Self::get_function_signature_impl(input)
    }

    #[inline]
    pub fn encode_contract_call(
        input: Proto::FunctionEncodingInput<'_>,
    ) -> Proto::FunctionEncodingOutput<'static> {
        Self::encode_contract_call_impl(input)
            .unwrap_or_else(|err| abi_output_error!(Proto::FunctionEncodingOutput, err))
    }

    fn decode_contract_call_impl(
        input: Proto::ContractCallDecodingInput,
    ) -> AbiResult<Proto::ContractCallDecodingOutput<'static>> {
        if input.encoded.len() < H32::len() {
            return AbiError::err(AbiErrorKind::Error_decoding_data)
                .context("Encoded Contract Call bytes too short");
        }
        let short_signature = &input.encoded[0..H32::len()];
        let short_signature =
            H32::try_from(short_signature).expect("The length expected to be checked above");
        let encoded_data = &input.encoded[H32::len()..];

        let mut abi_json: SmartContractCallAbiJson =
            serde_json::from_str(&input.smart_contract_abi_json)
                .tw_err(|_| AbiErrorKind::Error_invalid_abi)
                .context("Error deserializing Smart Contract ABI as JSON")?;

        let function = abi_json
            .map
            .get_mut(&ContractCallSignature(short_signature))
            .or_tw_err(AbiErrorKind::Error_abi_mismatch)
            .with_context(|| {
                format!(
                    "Contract Call ABI does not have a function with {short_signature} signature"
                )
            })?;

        let decoded_tokens = function.decode_input(encoded_data)?;

        // Clear the `outputs` to avoid adding them to the signature.
        // This is a requirement that comes from legacy ABI implementation.
        function.outputs.clear();
        let function_signature = function.signature();

        // Serialize the `decoded_json` result.
        let decoded_res = SmartContractCallDecodedInputJson {
            function: function_signature,
            inputs: &decoded_tokens,
        };
        let decoded_json = serde_json::to_string(&decoded_res)
            .tw_err(|_| AbiErrorKind::Error_internal)
            .context("Error serializing Smart Contract Input as JSON")?;

        // Serialize the Proto parameters.
        let decoded_protos = decoded_tokens
            .into_iter()
            .map(Self::named_token_to_proto)
            .collect();

        Ok(Proto::ContractCallDecodingOutput {
            decoded_json: Cow::Owned(decoded_json),
            tokens: decoded_protos,
            ..Proto::ContractCallDecodingOutput::default()
        })
    }

    fn decode_params_impl(
        input: Proto::ParamsDecodingInput<'_>,
    ) -> AbiResult<Proto::ParamsDecodingOutput<'static>> {
        let abi = match input.abi {
            AbiEnum::abi_json(abi_json) => serde_json::from_str(&abi_json)
                .tw_err(|_| AbiErrorKind::Error_invalid_abi)
                .context("Error deserializing ABI as JSON")?,
            AbiEnum::abi_params(abi_params) => abi_params
                .params
                .into_iter()
                .map(Self::param_from_proto)
                .collect::<AbiResult<Vec<_>>>()?,
            AbiEnum::None => {
                return AbiError::err(AbiErrorKind::Error_invalid_abi).context("No ABI specified")
            },
        };

        let decoded_tokens = decode_params(&abi, &input.encoded)?;
        // Serialize the Proto parameters.
        let decoded_protos = decoded_tokens
            .into_iter()
            .map(Self::named_token_to_proto)
            .collect();

        Ok(Proto::ParamsDecodingOutput {
            tokens: decoded_protos,
            ..Proto::ParamsDecodingOutput::default()
        })
    }

    fn decode_value_impl(
        input: Proto::ValueDecodingInput<'_>,
    ) -> AbiResult<Proto::ValueDecodingOutput<'static>> {
        let param_type = DecodingValueType::from_str(&input.param_type)
            .context("Invalid parameter type")?
            .0;
        let token = decode_value(&param_type, &input.encoded)?;
        let token_str = token.to_string();
        Ok(Proto::ValueDecodingOutput {
            token: Some(Self::token_to_proto(token)),
            param_str: token_str.into(),
            ..Proto::ValueDecodingOutput::default()
        })
    }

    fn get_function_signature_impl(input: Proto::FunctionGetTypeInput<'_>) -> String {
        let function_inputs = input
            .inputs
            .into_iter()
            .map(Self::param_from_proto)
            .collect::<AbiResult<Vec<_>>>()
            .unwrap_or_default();

        let fun = Function {
            name: input.function_name.to_string(),
            inputs: function_inputs,
            ..Function::default()
        };
        fun.signature()
    }

    fn encode_contract_call_impl(
        input: Proto::FunctionEncodingInput<'_>,
    ) -> AbiResult<Proto::FunctionEncodingOutput<'static>> {
        let mut tokens = Vec::with_capacity(input.tokens.len());
        let mut input_types = Vec::with_capacity(input.tokens.len());

        for token in input.tokens {
            let named_token = Self::named_token_from_proto(token)?;
            input_types.push(named_token.to_param());
            tokens.push(named_token.value);
        }

        let fun = Function {
            name: input.function_name.to_string(),
            inputs: input_types,
            ..Function::default()
        };

        let encoded = fun.encode_input(&tokens)?;
        Ok(Proto::FunctionEncodingOutput {
            function_type: fun.signature().into(),
            encoded: encoded.into(),
            ..Proto::FunctionEncodingOutput::default()
        })
    }

    pub fn param_to_proto(param: Param) -> Proto::Param<'static> {
        Proto::Param {
            name: Cow::Owned(param.name.unwrap_or_default()),
            param: Some(Self::param_type_to_proto(param.kind)),
        }
    }

    fn param_from_proto(param: Proto::Param<'_>) -> AbiResult<Param> {
        let name = if param.name.is_empty() {
            None
        } else {
            Some(param.name.to_string())
        };

        let proto_param_type = param
            .param
            .or_tw_err(AbiErrorKind::Error_missing_param_type)
            .context("Missing parameter type")?;
        let kind = Self::param_type_from_proto(proto_param_type)?;

        Ok(Param {
            name,
            kind,
            internal_type: None,
        })
    }

    fn named_token_from_proto(named_token: Proto::Token<'_>) -> AbiResult<NamedToken> {
        Ok(NamedToken {
            name: Some(named_token.name.clone().into()),
            value: Self::token_from_proto(named_token)?,
            internal_type: None,
        })
    }

    fn token_from_proto(token: Proto::Token<'_>) -> AbiResult<Token> {
        match token.token {
            TokenEnum::boolean(bool) => Ok(Token::Bool(bool)),
            TokenEnum::number_uint(u) => {
                let uint = Self::u_number_n_from_proto(&u.value)?;
                Token::uint(u.bits as usize, uint)
            },
            TokenEnum::number_int(i) => {
                let int = Self::s_number_n_from_proto(&i.value)?;
                Token::int(i.bits as usize, int)
            },
            TokenEnum::string_value(str) => Ok(Token::String(str.to_string())),
            TokenEnum::address(addr) => {
                let addr = Address::from_str(&addr)
                    .tw_err(|_| AbiErrorKind::Error_invalid_address_value)?;
                Ok(Token::Address(addr))
            },
            TokenEnum::byte_array(bytes) => Ok(Token::Bytes(bytes.to_vec())),
            TokenEnum::byte_array_fix(bytes) => {
                let checked_bytes = NonEmptyBytes::new(bytes.to_vec())
                    .context("Empty `FixedBytes` collection is not allowed")?;
                Ok(Token::FixedBytes(checked_bytes))
            },
            TokenEnum::array(arr) => {
                let (arr, kind) = Self::array_from_proto(arr)?;
                Ok(Token::Array { arr, kind })
            },
            TokenEnum::fixed_array(arr) => {
                let (arr, kind) = Self::array_from_proto(arr)?;
                let arr = NonEmptyArray::new(arr)
                    .context("Empty `FixedArray` collection is not allowed")?;
                Ok(Token::FixedArray { arr, kind })
            },
            TokenEnum::tuple(Proto::TupleParam { params }) => {
                let params = params
                    .into_iter()
                    .map(Self::named_token_from_proto)
                    .collect::<AbiResult<Vec<_>>>()?;
                Ok(Token::Tuple { params })
            },
            TokenEnum::None => AbiError::err(AbiErrorKind::Error_missing_param_value),
        }
    }

    fn array_from_proto(array: Proto::ArrayParam<'_>) -> AbiResult<(Vec<Token>, ParamType)> {
        let element_type = array
            .element_type
            .or_tw_err(AbiErrorKind::Error_missing_param_type)?;
        let element_type = Self::param_type_from_proto(element_type)?;

        let mut array_tokens = Vec::with_capacity(array.elements.len());
        for proto_token in array.elements {
            let token = Self::token_from_proto(proto_token)?;
            let token_type = token.to_param_type();

            // Check if all tokens are the same as declared in `ArrayParam::element_type`.
            if token_type != element_type {
                return AbiError::err(AbiErrorKind::Error_invalid_param_type).with_context(|| {
                    format!("Expected '{element_type:?}' array element type, found {token_type:?}")
                });
            }
            array_tokens.push(token);
        }

        Ok((array_tokens, element_type))
    }

    fn param_type_to_proto(param_type: ParamType) -> Proto::ParamType<'static> {
        let param = match param_type {
            ParamType::Address => ProtoParamType::address(Proto::AddressType {}),
            ParamType::FixedBytes { len } => {
                let size = len.get() as u64;
                ProtoParamType::byte_array_fix(Proto::ByteArrayFixType { size })
            },
            ParamType::Bytes => ProtoParamType::byte_array(Proto::ByteArrayType {}),
            ParamType::Int { bits } => ProtoParamType::number_int(Proto::NumberNType {
                bits: bits.get() as u32,
            }),
            ParamType::Uint { bits } => ProtoParamType::number_uint(Proto::NumberNType {
                bits: bits.get() as u32,
            }),
            ParamType::Bool => ProtoParamType::boolean(Proto::BoolType {}),
            ParamType::String => ProtoParamType::string_param(Proto::StringType {}),
            ParamType::FixedArray { kind, len } => {
                let size = len.get() as u64;
                let element_type = Some(Box::new(Self::param_type_to_proto(*kind)));
                ProtoParamType::fixed_array(Box::new(Proto::FixedArrayType { size, element_type }))
            },
            ParamType::Array { kind } => {
                let element_type = Some(Box::new(Self::param_type_to_proto(*kind)));
                ProtoParamType::array(Box::new(Proto::ArrayType { element_type }))
            },
            ParamType::Tuple { params } => {
                let params: Vec<_> = params.into_iter().map(Self::param_to_proto).collect();
                ProtoParamType::tuple(Proto::TupleType { params })
            },
        };
        Proto::ParamType { param }
    }

    fn param_type_from_proto(param_type: Proto::ParamType<'_>) -> AbiResult<ParamType> {
        match param_type.param {
            ProtoParamType::boolean(_) => Ok(ParamType::Bool),
            ProtoParamType::number_int(i) => {
                let bits = UintBits::new(i.bits as usize)?;
                Ok(ParamType::Int { bits })
            },
            ProtoParamType::number_uint(u) => {
                let bits = UintBits::new(u.bits as usize)?;
                Ok(ParamType::Uint { bits })
            },
            ProtoParamType::string_param(_) => Ok(ParamType::String),
            ProtoParamType::address(_) => Ok(ParamType::Address),
            ProtoParamType::byte_array(_) => Ok(ParamType::Bytes),
            ProtoParamType::byte_array_fix(bytes) => {
                let len = NonZeroLen::new(bytes.size as usize)
                    .context("Expected non-zero 'FixByteArray' length")?;
                Ok(ParamType::FixedBytes { len })
            },
            ProtoParamType::array(arr) => {
                let element_type = arr
                    .element_type
                    .or_tw_err(AbiErrorKind::Error_missing_param_type)?;
                let kind = Self::param_type_from_proto(*element_type)?;
                Ok(ParamType::Array {
                    kind: Box::new(kind),
                })
            },
            ProtoParamType::fixed_array(arr) => {
                let element_type = arr
                    .element_type
                    .or_tw_err(AbiErrorKind::Error_missing_param_type)?;
                let kind = Box::new(Self::param_type_from_proto(*element_type)?);
                let len = NonZeroLen::new(arr.size as usize)?;
                Ok(ParamType::FixedArray { kind, len })
            },
            ProtoParamType::tuple(tuple) => {
                let params = tuple
                    .params
                    .into_iter()
                    .map(Self::param_from_proto)
                    .collect::<AbiResult<Vec<_>>>()?;
                if params.is_empty() {
                    return AbiError::err(AbiErrorKind::Error_invalid_abi)
                        .context("Empty 'Tuple' collection is not allowed");
                }
                Ok(ParamType::Tuple { params })
            },
            ProtoParamType::None => AbiError::err(AbiErrorKind::Error_missing_param_type),
        }
    }

    fn named_token_to_proto(token: NamedToken) -> Proto::Token<'static> {
        Proto::Token {
            name: Cow::Owned(token.name.unwrap_or_default()),
            ..Self::token_to_proto(token.value)
        }
    }

    pub fn token_to_proto(token: Token) -> Proto::Token<'static> {
        let value = match token {
            Token::Address(addr) => TokenEnum::address(Cow::Owned(addr.to_string())),
            Token::FixedBytes(bytes) => TokenEnum::byte_array_fix(Cow::Owned(bytes.into_vec())),
            Token::Bytes(bytes) => TokenEnum::byte_array(Cow::Owned(bytes)),
            Token::Int { int, bits } => TokenEnum::number_int(Self::s_number_n_proto(int, bits)),
            Token::Uint { uint, bits } => {
                TokenEnum::number_uint(Self::u_number_n_proto(uint, bits))
            },
            Token::Bool(bool) => TokenEnum::boolean(bool),
            Token::String(str) => TokenEnum::string_value(Cow::Owned(str)),
            Token::FixedArray { kind, arr } => {
                TokenEnum::fixed_array(Self::array_to_proto(kind, arr.into_vec()))
            },
            Token::Array { kind, arr, .. } => TokenEnum::array(Self::array_to_proto(kind, arr)),
            Token::Tuple { params } => TokenEnum::tuple(Self::tuple_to_proto(params)),
        };
        Proto::Token {
            name: "".into(),
            token: value,
        }
    }

    fn s_number_n_from_proto(encoded: &[u8]) -> AbiResult<I256> {
        I256::from_big_endian_slice(encoded).tw_err(|_| AbiErrorKind::Error_invalid_uint_value)
    }

    fn u_number_n_from_proto(encoded: &[u8]) -> AbiResult<U256> {
        U256::from_big_endian_slice(encoded).tw_err(|_| AbiErrorKind::Error_invalid_uint_value)
    }

    fn s_number_n_proto(i: I256, bits: UintBits) -> Proto::NumberNParam<'static> {
        Proto::NumberNParam {
            bits: bits.get() as u32,
            value: Cow::Owned(i.to_big_endian_compact()),
        }
    }

    fn u_number_n_proto(u: U256, bits: UintBits) -> Proto::NumberNParam<'static> {
        Proto::NumberNParam {
            bits: bits.get() as u32,
            value: Cow::Owned(u.to_big_endian_compact()),
        }
    }

    fn array_to_proto(kind: ParamType, arr: Vec<Token>) -> Proto::ArrayParam<'static> {
        Proto::ArrayParam {
            elements: arr.into_iter().map(Self::token_to_proto).collect(),
            element_type: Some(Self::param_type_to_proto(kind)),
        }
    }

    fn tuple_to_proto(params: Vec<NamedToken>) -> Proto::TupleParam<'static> {
        let params = params.into_iter().map(Self::named_token_to_proto).collect();
        Proto::TupleParam { params }
    }
}

#[derive(Deserialize)]
struct SmartContractCallAbiJson {
    #[serde(flatten)]
    map: HashMap<ContractCallSignature, Function>,
}

#[derive(Eq, Deserialize, Hash, PartialEq, Serialize)]
struct ContractCallSignature(#[serde(with = "as_hex")] H32);

#[derive(Serialize)]
struct SmartContractCallDecodedInputJson<'a> {
    function: String,
    inputs: &'a [NamedToken],
}

/// A value type used on [`AbiEncoder::decode_value`].
/// Please note [`AbiEncoder::decode_value`] doesn't support `ParamType::Tuple` for decoding.
struct DecodingValueType(ParamType);

impl FromStr for DecodingValueType {
    type Err = AbiError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let param_type = ParamType::try_from_type_short(s)?;
        if param_type.has_tuple_components() {
            return AbiError::err(AbiErrorKind::Error_invalid_param_type);
        }
        Ok(DecodingValueType(param_type))
    }
}
