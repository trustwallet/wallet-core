// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::abi::contract::Contract;
use crate::abi::token::Token;
use crate::abi::AbiResult;
use crate::address::Address;
use lazy_static::lazy_static;
use tw_memory::Data;
use tw_number::U256;

/// Generated via https://remix.ethereum.org
/// Solidity: https://github.com/OpenZeppelin/openzeppelin-contracts/blob/v4.9.2/contracts/token/ERC1155/IERC1155.sol
const ERC1155_ABI: &str = include_str!("resource/erc1155.abi.json");

lazy_static! {
    static ref ERC1155: Contract = serde_json::from_str(ERC1155_ABI).unwrap();
}

pub struct Erc1155;

impl Erc1155 {
    pub fn encode_safe_transfer_from(
        from: Address,
        to: Address,
        token_id: U256,
        value: U256,
        data: Data,
    ) -> AbiResult<Data> {
        let func = ERC1155.function("safeTransferFrom")?;
        func.encode_input(&[
            Token::Address(from),
            Token::Address(to),
            Token::u256(token_id),
            Token::u256(value),
            Token::Bytes(data),
        ])
    }
}
