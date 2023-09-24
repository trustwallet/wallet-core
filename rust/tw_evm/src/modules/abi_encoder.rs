// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::abi::decode::{decode_params, decode_value};
use crate::abi::function::Function;
use crate::abi::param::Param;
use crate::abi::param_token::ParamToken;
use crate::abi::param_type::ParamType;
use crate::abi::token::Token;
use crate::abi::{AbiError, AbiResult};
use crate::address::Address;
use crate::evm_context::EvmContext;
use serde::Deserialize;
use serde::Serialize;
use std::borrow::Cow;
use std::collections::HashMap;
use std::marker::PhantomData;
use std::str::FromStr;
use tw_coin_entry::error::{SigningError, SigningErrorType, SigningResult};
use tw_coin_entry::signing_output_error;
use tw_hash::H32;
use tw_misc::traits::ToBytesVec;
use tw_number::U256;
use tw_proto::EthereumAbi::Proto;

pub struct AbiEncoder<Context: EvmContext> {
    _phantom: PhantomData<Context>,
}

impl<Context: EvmContext> AbiEncoder<Context> {
    #[inline]
    pub fn decode_contract_call(
        input: Proto::ContractCallDecodingInput,
    ) -> Proto::ContractCallDecodingOutput<'static> {
        Self::decode_contract_call_impl(input)
            .unwrap_or_else(|err| signing_output_error!(Proto::ContractCallDecodingOutput, err))
    }

    #[inline]
    pub fn decode_params(
        input: Proto::ParamsDecodingInput<'_>,
    ) -> Proto::ParamsDecodingOutput<'static> {
        Self::decode_params_impl(input)
            .unwrap_or_else(|err| signing_output_error!(Proto::ParamsDecodingOutput, err))
    }

    #[inline]
    pub fn decode_value(
        input: Proto::ValueDecodingInput<'_>,
    ) -> Proto::ValueDecodingOutput<'static> {
        Self::decode_value_impl(input)
            .unwrap_or_else(|err| signing_output_error!(Proto::ValueDecodingOutput, err))
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
            .unwrap_or_else(|err| signing_output_error!(Proto::FunctionEncodingOutput, err))
    }

    // TODO use `Context`.
    fn decode_contract_call_impl(
        input: Proto::ContractCallDecodingInput,
    ) -> SigningResult<Proto::ContractCallDecodingOutput<'static>> {
        if input.encoded.len() < H32::len() {
            return Err(SigningError(SigningErrorType::Error_invalid_params));
        }
        let short_signature = &input.encoded[0..H32::len()];
        let short_signature =
            H32::try_from(short_signature).expect("The length expected to be checked above");
        let encoded_data = &input.encoded[H32::len()..];

        let abi_json: SmartContractCallAbiJson =
            serde_json::from_str(&input.smart_contract_abi_json)
                // TODO return `Proto::AbiError`
                .map_err(|_| SigningError(SigningErrorType::Error_invalid_params))?;

        let function = abi_json
            .map
            .get(&short_signature)
            .ok_or(SigningError(SigningErrorType::Error_general))?;

        let decoded_tokens = function.decode_input(encoded_data)?;

        // Serialize the `decoded_json` result.
        let decoded_res = SmartContractCallDecodedInputJson {
            function: function.signature_with_inputs(),
            inputs: &decoded_tokens,
        };
        let decoded_json = serde_json::to_string(&decoded_res)
            .map_err(|_| SigningError(SigningErrorType::Error_internal))?;

        // Serialize the Proto parameters.
        let decoded_protos = decoded_tokens
            .into_iter()
            .map(Self::param_token_to_proto)
            .collect();

        Ok(Proto::ContractCallDecodingOutput {
            decoded_json: Cow::Owned(decoded_json),
            params: decoded_protos,
            ..Proto::ContractCallDecodingOutput::default()
        })
    }

    // TODO return `Proto::AbiError`
    fn decode_params_impl(
        input: Proto::ParamsDecodingInput<'_>,
    ) -> SigningResult<Proto::ParamsDecodingOutput<'static>> {
        use Proto::mod_ParamsDecodingInput::OneOfabi as AbiEnum;

        let abi = match input.abi {
            AbiEnum::abi_json(abi_json) => serde_json::from_str(&abi_json)
                .map_err(|_| SigningError(SigningErrorType::Error_invalid_params))?,
            AbiEnum::abi_params(abi_params) => abi_params
                .params
                .into_iter()
                .map(Self::named_param_type_from_proto)
                .collect::<AbiResult<Vec<_>>>()?,
            AbiEnum::None => return Err(SigningError(SigningErrorType::Error_invalid_params)),
        };

        let decoded_tokens = decode_params(&abi, &input.encoded)?;
        // Serialize the Proto parameters.
        let decoded_protos = decoded_tokens
            .into_iter()
            .map(Self::param_token_to_proto)
            .collect();

        Ok(Proto::ParamsDecodingOutput {
            params: decoded_protos,
            ..Proto::ParamsDecodingOutput::default()
        })
    }

    pub fn decode_value_impl(
        input: Proto::ValueDecodingInput<'_>,
    ) -> SigningResult<Proto::ValueDecodingOutput<'static>> {
        let param_type = ParamType::try_from_type_short(&input.param_type)?;
        let token = decode_value(&param_type, &input.encoded)?;
        let token_str = token.to_string();
        Ok(Proto::ValueDecodingOutput {
            param: Some(Self::token_to_proto(token)),
            param_str: token_str.into(),
            ..Proto::ValueDecodingOutput::default()
        })
    }

    fn get_function_signature_impl(input: Proto::FunctionGetTypeInput<'_>) -> String {
        let function_inputs = input
            .inputs
            .into_iter()
            .map(Self::named_param_type_from_proto)
            .collect::<AbiResult<Vec<_>>>()
            .unwrap_or_default();

        let fun = Function {
            name: input.function_name.to_string(),
            inputs: function_inputs,
            ..Function::default()
        };
        fun.signature_with_inputs()
    }

    // TODO return `Proto::AbiError`
    fn encode_contract_call_impl(
        input: Proto::FunctionEncodingInput<'_>,
    ) -> SigningResult<Proto::FunctionEncodingOutput<'static>> {
        let input_tokens = input
            .params
            .into_iter()
            .map(Self::named_param_from_proto)
            .collect::<AbiResult<Vec<_>>>()?;
        let input_types: Vec<_> = input_tokens.iter().map(|token| token.to_param()).collect();

        let fun = Function {
            name: input.function_name.to_string(),
            inputs: input_types,
            ..Function::default()
        };

        let encoded = fun.encode_input(input_tokens)?;
        Ok(Proto::FunctionEncodingOutput {
            function_type: fun.signature_with_inputs().into(),
            encoded: encoded.into(),
            ..Proto::FunctionEncodingOutput::default()
        })
    }

    fn named_param_type_to_proto(param: Param) -> Proto::NamedParamType<'static> {
        Proto::NamedParamType {
            name: Cow::Owned(param.name.unwrap_or_default()),
            param: Some(Self::param_type_to_proto(param.kind)),
        }
    }

    fn named_param_type_from_proto(
        named_param_type: Proto::NamedParamType<'_>,
    ) -> AbiResult<Param> {
        let name = if named_param_type.name.is_empty() {
            None
        } else {
            Some(named_param_type.name.to_string())
        };

        let proto_param_type = named_param_type.param.ok_or(AbiError::InvalidParamType)?;
        let kind = Self::param_type_from_proto(proto_param_type)?;

        Ok(Param {
            name,
            kind,
            internal_type: None,
        })
    }

    fn named_param_from_proto(named_param: Proto::NamedParam<'_>) -> AbiResult<ParamToken> {
        let value = named_param.value.ok_or(AbiError::MissingParamValue)?;
        Ok(ParamToken {
            name: Some(named_param.name.into()),
            value: Self::param_value_from_proto(value)?,
            internal_type: None,
        })
    }

    fn param_value_from_proto(param_value: Proto::ParamValue<'_>) -> AbiResult<Token> {
        use Proto::mod_ParamValue::OneOfparam as ParamEnum;

        match param_value.param {
            ParamEnum::boolean(bool) => Ok(Token::Bool(bool)),
            ParamEnum::number_uint(u) => {
                let uint = Self::number_n_from_proto(&u.value)?;
                Ok(Token::Uint {
                    bits: u.bits as usize,
                    uint,
                })
            },
            ParamEnum::number_int(i) => {
                let int = Self::number_n_from_proto(&i.value)?;
                Ok(Token::Int {
                    bits: i.bits as usize,
                    int,
                })
            },
            ParamEnum::string_value(str) => Ok(Token::String(str.to_string())),
            ParamEnum::address(addr) => {
                let addr = Address::from_str(&addr).map_err(|_| AbiError::InvalidAddressValue)?;
                Ok(Token::Address(addr))
            },
            ParamEnum::byte_array(bytes) => Ok(Token::Bytes(bytes.to_vec())),
            ParamEnum::byte_array_fix(bytes) => Ok(Token::FixedBytes(bytes.to_vec())),
            ParamEnum::array(arr) => {
                let (arr, kind) = Self::array_from_proto(arr)?;
                Ok(Token::Array { arr, kind })
            },
            ParamEnum::fixed_array(arr) => {
                let (arr, kind) = Self::array_from_proto(arr)?;
                Ok(Token::FixedArray { arr, kind })
            },
            ParamEnum::tuple(Proto::TupleParam { params }) => {
                let params = params
                    .into_iter()
                    .map(Self::named_param_from_proto)
                    .collect::<AbiResult<Vec<_>>>()?;
                Ok(Token::Tuple { params })
            },
            ParamEnum::None => Err(AbiError::MissingParamValue),
        }
    }

    fn array_from_proto(array: Proto::ArrayParam<'_>) -> AbiResult<(Vec<Token>, ParamType)> {
        let element_type = array.element_type.ok_or(AbiError::MissingParamType)?;
        let kind = Self::param_type_from_proto(element_type)?;
        let arr = array
            .values
            .into_iter()
            .map(Self::param_value_from_proto)
            .collect::<AbiResult<Vec<_>>>()?;
        Ok((arr, kind))
    }

    fn param_type_to_proto(param_type: ParamType) -> Proto::ParamType<'static> {
        // TODO move to top
        use Proto::mod_ParamType::OneOfparam as ProtoParamType;

        let param = match param_type {
            ParamType::Address => ProtoParamType::address(Proto::AddressType {}),
            ParamType::FixedBytes { len } => {
                let size = len as u64;
                ProtoParamType::byte_array_fix(Proto::ByteArrayFixType { size })
            },
            ParamType::Bytes => ProtoParamType::byte_array(Proto::ByteArrayType {}),
            ParamType::Int { bits } => {
                ProtoParamType::number_int(Proto::NumberNType { bits: bits as u32 })
            },
            ParamType::Uint { bits } => {
                ProtoParamType::number_uint(Proto::NumberNType { bits: bits as u32 })
            },
            ParamType::Bool => ProtoParamType::boolean(Proto::BoolType {}),
            ParamType::String => ProtoParamType::string_param(Proto::StringType {}),
            ParamType::FixedArray { kind, len } => {
                let size = len as u64;
                let element_type = Some(Box::new(Self::param_type_to_proto(*kind)));
                ProtoParamType::fixed_array(Box::new(Proto::FixedArrayType { size, element_type }))
            },
            ParamType::Array { kind } => {
                let element_type = Some(Box::new(Self::param_type_to_proto(*kind)));
                ProtoParamType::array(Box::new(Proto::ArrayType { element_type }))
            },
            ParamType::Tuple { params } => {
                let params: Vec<_> = params
                    .into_iter()
                    .map(Self::named_param_type_to_proto)
                    .collect();
                ProtoParamType::tuple(Proto::TupleType { params })
            },
        };
        Proto::ParamType { param }
    }

    fn param_type_from_proto(param_type: Proto::ParamType<'_>) -> AbiResult<ParamType> {
        use Proto::mod_ParamType::OneOfparam as ProtoParamType;

        match param_type.param {
            ProtoParamType::boolean(_) => Ok(ParamType::Bool),
            ProtoParamType::number_int(i) => Ok(ParamType::Int {
                bits: i.bits as usize,
            }),
            ProtoParamType::number_uint(u) => Ok(ParamType::Uint {
                bits: u.bits as usize,
            }),
            ProtoParamType::string_param(_) => Ok(ParamType::String),
            ProtoParamType::address(_) => Ok(ParamType::Address),
            ProtoParamType::byte_array(_) => Ok(ParamType::Bytes),
            ProtoParamType::byte_array_fix(bytes) => Ok(ParamType::FixedBytes {
                len: bytes.size as usize,
            }),
            ProtoParamType::array(arr) => {
                let element_type = arr.element_type.ok_or(AbiError::InvalidParamType)?;
                let kind = Self::param_type_from_proto(*element_type)?;
                Ok(ParamType::Array {
                    kind: Box::new(kind),
                })
            },
            ProtoParamType::fixed_array(arr) => {
                let element_type = arr.element_type.ok_or(AbiError::InvalidParamType)?;
                let kind = Self::param_type_from_proto(*element_type)?;
                Ok(ParamType::FixedArray {
                    len: arr.size as usize,
                    kind: Box::new(kind),
                })
            },
            ProtoParamType::tuple(tuple) => {
                let params = tuple
                    .params
                    .into_iter()
                    .map(Self::named_param_type_from_proto)
                    .collect::<AbiResult<Vec<_>>>()?;
                Ok(ParamType::Tuple { params })
            },
            ProtoParamType::None => Err(AbiError::InvalidParamType),
        }
    }

    fn param_token_to_proto(token: ParamToken) -> Proto::NamedParam<'static> {
        Proto::NamedParam {
            name: Cow::Owned(token.name.unwrap_or_default()),
            value: Some(Self::token_to_proto(token.value)),
        }
    }

    fn token_to_proto(token: Token) -> Proto::ParamValue<'static> {
        use Proto::mod_ParamValue::OneOfparam as ParamEnum;

        let value = match token {
            Token::Address(addr) => ParamEnum::address(Cow::Owned(addr.to_string())),
            Token::FixedBytes(bytes) => ParamEnum::byte_array_fix(Cow::Owned(bytes)),
            Token::Bytes(bytes) => ParamEnum::byte_array(Cow::Owned(bytes)),
            Token::Int { int, bits } => ParamEnum::number_int(Self::number_n_proto(int, bits)),
            Token::Uint { uint, bits } => ParamEnum::number_uint(Self::number_n_proto(uint, bits)),
            Token::Bool(bool) => ParamEnum::boolean(bool),
            Token::String(str) => ParamEnum::string_value(Cow::Owned(str)),
            Token::FixedArray { kind, arr } => {
                ParamEnum::fixed_array(Self::array_to_proto(kind, arr))
            },
            Token::Array { kind, arr, .. } => ParamEnum::array(Self::array_to_proto(kind, arr)),
            Token::Tuple { params } => ParamEnum::tuple(Self::tuple_to_proto(params)),
        };
        Proto::ParamValue { param: value }
    }

    fn number_n_from_proto(encoded: &[u8]) -> AbiResult<U256> {
        U256::from_big_endian_slice(encoded).map_err(|_| AbiError::InvalidUintValue)
    }

    fn number_n_proto(u: U256, bits: usize) -> Proto::NumberNParam<'static> {
        Proto::NumberNParam {
            bits: bits as u32,
            value: Cow::Owned(u.to_big_endian_compact()),
        }
    }

    fn array_to_proto(kind: ParamType, arr: Vec<Token>) -> Proto::ArrayParam<'static> {
        Proto::ArrayParam {
            values: arr.into_iter().map(Self::token_to_proto).collect(),
            element_type: Some(Self::param_type_to_proto(kind)),
        }
    }

    fn tuple_to_proto(params: Vec<ParamToken>) -> Proto::TupleParam<'static> {
        let params = params.into_iter().map(Self::param_token_to_proto).collect();
        Proto::TupleParam { params }
    }
}

#[derive(Deserialize)]
struct SmartContractCallAbiJson {
    #[serde(flatten)]
    map: HashMap<H32, Function>,
}

#[derive(Serialize)]
struct SmartContractCallDecodedInputJson<'a> {
    function: String,
    inputs: &'a [ParamToken],
}
