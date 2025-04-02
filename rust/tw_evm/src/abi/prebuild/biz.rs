// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::abi::contract::Contract;
use crate::abi::function::Function;
use crate::abi::param_token::NamedToken;
use crate::abi::param_type::ParamType;
use crate::abi::prebuild::ExecuteArgs;
use crate::abi::token::Token;
use crate::abi::{AbiError, AbiErrorKind, AbiResult};
use lazy_static::lazy_static;
use tw_coin_entry::error::prelude::*;
use tw_memory::Data;

const ERC4337_BIZ_ACCOUNT_ABI: &str = include_str!("resource/erc4337.biz_account.abi.json");

lazy_static! {
    static ref ERC4337_BIZ_ACCOUNT: Contract =
        serde_json::from_str(ERC4337_BIZ_ACCOUNT_ABI).unwrap();
}

pub struct BizAccount;

impl BizAccount {
    pub fn encode_execute(args: ExecuteArgs) -> AbiResult<Data> {
        let func = ERC4337_BIZ_ACCOUNT.function("execute")?;
        func.encode_input(&[
            Token::Address(args.to),
            Token::u256(args.value),
            Token::Bytes(args.data),
        ])
    }

    pub fn encode_execute_4337_op(args: ExecuteArgs) -> AbiResult<Data> {
        let func = ERC4337_BIZ_ACCOUNT.function("execute4337Op")?;
        func.encode_input(&[
            Token::Address(args.to),
            Token::u256(args.value),
            Token::Bytes(args.data),
        ])
    }

    pub fn encode_execute_batch<I>(args: I) -> AbiResult<Data>
    where
        I: IntoIterator<Item = ExecuteArgs>,
    {
        let func = ERC4337_BIZ_ACCOUNT.function("executeBatch")?;
        encode_batch(func, args)
    }

    pub fn encode_execute_4337_ops<I>(args: I) -> AbiResult<Data>
    where
        I: IntoIterator<Item = ExecuteArgs>,
    {
        let func = ERC4337_BIZ_ACCOUNT.function("execute4337Ops")?;
        encode_batch(func, args)
    }
}

fn encode_batch<I>(function: &Function, args: I) -> AbiResult<Data>
where
    I: IntoIterator<Item = ExecuteArgs>,
{
    // `tuple[]`, where each item is a tuple of (address, uint256, bytes).
    let array_param = function
        .inputs
        .first()
        .or_tw_err(AbiErrorKind::Error_internal)
        .context("'Biz.execute4337Ops()' should contain only one argument")?;

    let ParamType::Array {
        kind: array_elem_type,
    } = array_param.kind.clone()
    else {
        return AbiError::err(AbiErrorKind::Error_internal).with_context(|| {
            format!(
                "'Biz.execute4337Ops()' input argument should be an array, found: {:?}",
                array_param.kind
            )
        });
    };

    let ParamType::Tuple {
        params: tuple_params,
    } = array_elem_type.as_ref()
    else {
        return AbiError::err(AbiErrorKind::Error_internal).with_context(|| {
            format!(
                "'Biz.execute4337Ops()' input argument should be an array of tuples, found: {array_elem_type:?}",
            )
        });
    };

    if tuple_params.len() != 3 {
        return AbiError::err(AbiErrorKind::Error_internal).with_context(|| {
            format!(
                "'Biz.execute4337Ops()' input argument should be an array of tuples with 3 elements, found: {}", tuple_params.len()
            )
        });
    }

    let array_tokens = args
        .into_iter()
        .map(|call| Token::Tuple {
            params: vec![
                NamedToken::with_param_and_token(&tuple_params[0], Token::Address(call.to)),
                NamedToken::with_param_and_token(&tuple_params[1], Token::u256(call.value)),
                NamedToken::with_param_and_token(&tuple_params[2], Token::Bytes(call.data)),
            ],
        })
        .collect();

    function.encode_input(&[Token::array(*array_elem_type, array_tokens)])
}
