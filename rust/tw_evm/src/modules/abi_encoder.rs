// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::abi::decode::decode_params;
use crate::abi::function::Function;
use crate::abi::param::Param;
use crate::abi::param_token::ParamToken;
use crate::abi::param_type::ParamType;
use crate::abi::token::Token;
use crate::abi::{AbiError, AbiResult};
use crate::evm_context::EvmContext;
use serde::Deserialize;
use serde::Serialize;
use std::borrow::Cow;
use std::collections::HashMap;
use std::marker::PhantomData;
use tw_coin_entry::error::{SigningError, SigningErrorType, SigningResult};
use tw_coin_entry::signing_output_error;
use tw_hash::H32;
use tw_number::U256;
use tw_proto::EthereumAbi::Proto;

pub struct AbiEncoder<Context: EvmContext> {
    _phantom: PhantomData<Context>,
}

impl<Context: EvmContext> AbiEncoder<Context> {
    pub fn decode_contract_call(
        input: Proto::ContractCallDecodingInput,
    ) -> Proto::ContractCallDecodingOutput<'static> {
        Self::decode_contract_call_impl(input)
            .unwrap_or_else(|err| signing_output_error!(Proto::ContractCallDecodingOutput, err))
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

    pub fn decode_params(
        input: Proto::ParamsDecodingInput<'_>,
    ) -> Proto::ParamsDecodingOutput<'static> {
        Self::decode_params_impl(input)
            .unwrap_or_else(|err| signing_output_error!(Proto::ParamsDecodingOutput, err))
    }

    // TODO return `Proto::AbiError`
    pub fn decode_params_impl(
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
            Token::FixedArray { arr, .. } => ParamEnum::fixed_array(Self::array_to_proto(arr)),
            Token::Array { arr, .. } => ParamEnum::array(Self::array_to_proto(arr)),
            Token::Tuple { params } => ParamEnum::tuple(Self::tuple_to_proto(params)),
        };
        Proto::ParamValue { param: value }
    }

    fn number_n_proto(u: U256, bits: usize) -> Proto::NumberNParam<'static> {
        Proto::NumberNParam {
            bits: bits as u32,
            value: Cow::Owned(u.to_big_endian_compact()),
        }
    }

    fn array_to_proto(arr: Vec<Token>) -> Proto::ArrayParam<'static> {
        let values = arr.into_iter().map(Self::token_to_proto).collect();
        Proto::ArrayParam { values }
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
