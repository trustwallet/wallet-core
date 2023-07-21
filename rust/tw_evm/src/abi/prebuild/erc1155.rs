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
/// Solidity: https://github.com/OpenZeppelin/openzeppelin-contracts/blob/v4.9.2/contracts/token/ERC1155/IERC1155.sol
const ERC1155_ABI: &str = include_str!("erc1155.abi.json");

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
        data: Vec<u8>,
    ) -> AbiResult<Vec<u8>> {
        let func = ERC1155.function("safeTransferFrom")?;
        func.encode_input(&[
            Token::Address(convert_address(from)),
            Token::Address(convert_address(to)),
            Token::Uint(convert_u256(token_id)),
            Token::Uint(convert_u256(value)),
            Token::Bytes(data),
        ])
        .map_err(AbiError::from)
    }
}
