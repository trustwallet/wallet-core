// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::abi::{convert_address, convert_u256, AbiError, AbiResult};
use crate::address::Address;
use ethabi::{Contract, Token};
use lazy_static::lazy_static;
use tw_memory::Data;
use tw_number::U256;

/// Generated via https://remix.ethereum.org
/// Solidity: https://github.com/OpenZeppelin/openzeppelin-contracts/blob/v4.9.2/contracts/token/ERC721/IERC721.sol
const ERC721_ABI: &str = include_str!("resource/erc721.abi.json");

lazy_static! {
    static ref ERC721: Contract = serde_json::from_str(ERC721_ABI).unwrap();
}

pub struct Erc721;

impl Erc721 {
    pub fn encode_transfer_from(from: Address, to: Address, token_id: U256) -> AbiResult<Data> {
        let func = ERC721.function("transferFrom")?;
        func.encode_input(&[
            Token::Address(convert_address(from)),
            Token::Address(convert_address(to)),
            Token::Uint(convert_u256(token_id)),
        ])
        .map_err(AbiError::from)
    }
}
