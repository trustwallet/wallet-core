// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::abi::contract::Contract;
use crate::abi::param_type::ParamType;
use crate::abi::token::Token;
use crate::abi::AbiResult;
use crate::address::Address;
use lazy_static::lazy_static;
use tw_memory::Data;
use tw_number::U256;

/// Generated via https://remix.ethereum.org
/// https://github.com/eth-infinitism/account-abstraction/blob/develop/contracts/samples/SimpleAccount.sol
const ERC4337_SIMPLE_ACCOUNT_ABI: &str = include_str!("resource/erc4337.simple_account.abi.json");

lazy_static! {
    static ref ERC4337_SIMPLE_ACCOUNT: Contract =
        serde_json::from_str(ERC4337_SIMPLE_ACCOUNT_ABI).unwrap();
}

pub struct ExecuteArgs {
    pub to: Address,
    pub value: U256,
    pub data: Data,
}

pub struct Erc4337SimpleAccount;

impl Erc4337SimpleAccount {
    pub fn encode_execute(args: ExecuteArgs) -> AbiResult<Data> {
        let func = ERC4337_SIMPLE_ACCOUNT.function("execute")?;
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
        let func = ERC4337_SIMPLE_ACCOUNT.function("executeBatch")?;

        let args = args.into_iter();
        let capacity = {
            let (lower, upper) = args.size_hint();
            upper.unwrap_or(lower)
        };

        let mut addresses = Vec::with_capacity(capacity);
        let mut values = Vec::with_capacity(capacity);
        let mut datas = Vec::with_capacity(capacity);

        for arg in args {
            addresses.push(Token::Address(arg.to));
            values.push(Token::u256(arg.value));
            datas.push(Token::Bytes(arg.data));
        }

        func.encode_input(&[
            Token::array(ParamType::Address, addresses),
            Token::array(ParamType::u256(), values),
            Token::array(ParamType::Bytes, datas),
        ])
    }
}
