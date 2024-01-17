// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::abi::contract::Contract;
use crate::abi::token::Token;
use crate::abi::AbiResult;
use crate::address::Address;
use lazy_static::lazy_static;
use tw_memory::Data;
use tw_number::U256;

/// Generated via https://remix.ethereum.org
/// https://github.com/OpenZeppelin/openzeppelin-contracts/blob/v4.9.2/contracts/token/ERC20/IERC20.sol
const ERC20_ABI: &str = include_str!("resource/erc20.abi.json");

lazy_static! {
    static ref ERC20: Contract = serde_json::from_str(ERC20_ABI).unwrap();
}

pub struct Erc20;

impl Erc20 {
    pub fn transfer(recipient: Address, amount: U256) -> AbiResult<Data> {
        let func = ERC20.function("transfer")?;
        func.encode_input(&[Token::Address(recipient), Token::u256(amount)])
    }

    pub fn approve(spender: Address, amount: U256) -> AbiResult<Data> {
        let func = ERC20.function("approve")?;
        func.encode_input(&[Token::Address(spender), Token::u256(amount)])
    }
}
