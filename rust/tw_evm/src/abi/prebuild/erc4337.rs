// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::abi::{AbiError, AbiResult};
use crate::address::Address;
use ethabi::{Contract, Token};
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
            Token::Address(args.to.to_ethabi()),
            Token::Uint(args.value.to_ethabi()),
            Token::Bytes(args.data),
        ])
        .map_err(AbiError::from)
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
            addresses.push(Token::Address(arg.to.to_ethabi()));
            values.push(Token::Uint(arg.value.to_ethabi()));
            datas.push(Token::Bytes(arg.data));
        }

        func.encode_input(&[
            Token::Array(addresses),
            Token::Array(values),
            Token::Array(datas),
        ])
        .map_err(AbiError::from)
    }
}
