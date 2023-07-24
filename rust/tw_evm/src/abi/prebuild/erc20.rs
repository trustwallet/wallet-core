// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::abi::{convert_address, convert_u256, AbiError, AbiResult};
use crate::address::Address;
use ethabi::{Contract, Token};
use lazy_static::lazy_static;
use tw_number::U256;

/// Generated via https://remix.ethereum.org
/// https://github.com/OpenZeppelin/openzeppelin-contracts/blob/v4.9.2/contracts/token/ERC20/IERC20.sol
const ERC20_ABI: &str = include_str!("resource/erc20.abi.json");

lazy_static! {
    static ref ERC20: Contract = serde_json::from_str(ERC20_ABI).unwrap();
}

pub struct Erc20;

impl Erc20 {
    pub fn transfer(recipient: Address, amount: U256) -> AbiResult<Vec<u8>> {
        let func = ERC20.function("transfer")?;
        func.encode_input(&[
            Token::Address(convert_address(recipient)),
            Token::Uint(convert_u256(amount)),
        ])
        .map_err(AbiError::from)
    }

    pub fn approve(spender: Address, amount: U256) -> AbiResult<Vec<u8>> {
        let func = ERC20.function("approve")?;
        func.encode_input(&[
            Token::Address(convert_address(spender)),
            Token::Uint(convert_u256(amount)),
        ])
        .map_err(AbiError::from)
    }
}
