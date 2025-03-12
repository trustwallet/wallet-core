// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::abi::encode::encode_tokens;
use crate::abi::non_empty_array::NonEmptyBytes;
use crate::abi::token::Token;
use crate::address::Address;
use crate::transaction::signature::Signature;
use crate::transaction::{SignedTransaction, TransactionCommon, UnsignedTransaction};
use serde::Serialize;
use tw_coin_entry::error::prelude::*;
use tw_encoding::hex;
use tw_hash::sha3::keccak256;
use tw_hash::H256;
use tw_memory::Data;
use tw_number::U256;

pub struct UserOperationV0_7 {
    pub nonce: U256,
    pub entry_point: Address,
    pub sender: Address,
    pub init_code: Data,
    pub account_gas_limits: Data,
    pub pre_verification_gas: U256,
    pub gas_fees: Data,
    pub paymaster_and_data: Data,
    pub payload: Data,
}

impl TransactionCommon for UserOperationV0_7 {
    #[inline]
    fn payload(&self) -> Data {
        self.payload.clone()
    }
}

impl UnsignedTransaction for UserOperationV0_7 {
    type SignedTransaction = SignedUserOperationV0_7;

    fn pre_hash(&self, chain_id: U256) -> H256 {
        let encode_hash = keccak256(&self.encode(chain_id));
        let encode_hash =
            NonEmptyBytes::new(encode_hash).expect("keccak256 must not return an empty hash");

        let tokens = [
            Token::FixedBytes(encode_hash),
            Token::Address(self.entry_point),
            Token::u256(chain_id),
        ];
        let encoded = encode_tokens(&tokens);
        let pre_hash = keccak256(&encoded);
        H256::try_from(pre_hash.as_slice()).expect("keccak256 returns 32 bytes")
    }

    fn encode(&self, _chain_id: U256) -> Data {
        let init_code_hash = keccak256(&self.init_code);
        let init_code_hash =
            NonEmptyBytes::new(init_code_hash).expect("keccak256 must not return an empty hash");

        let payload_hash = keccak256(&self.payload);
        let payload_hash =
            NonEmptyBytes::new(payload_hash).expect("keccak256 must not return an empty hash");

        let paymaster_and_data_hash = keccak256(&self.paymaster_and_data);
        let paymaster_and_data_hash = NonEmptyBytes::new(paymaster_and_data_hash)
            .expect("keccak256 must not return an empty hash");

        let account_gas_limits = NonEmptyBytes::new(self.account_gas_limits.clone())
            .expect("keccak256 must not return an empty hash");

        let gas_fees = NonEmptyBytes::new(self.gas_fees.clone())
            .expect("keccak256 must not return an empty hash");

        let tokens = [
            Token::Address(self.sender),
            Token::u256(self.nonce),
            Token::FixedBytes(init_code_hash),
            Token::FixedBytes(payload_hash),
            Token::FixedBytes(account_gas_limits),
            Token::u256(self.pre_verification_gas),
            Token::FixedBytes(gas_fees),
            Token::FixedBytes(paymaster_and_data_hash),
        ];

        encode_tokens(&tokens)
    }

    #[inline]
    fn try_into_signed(
        self,
        signature: tw_keypair::ecdsa::secp256k1::Signature,
        _chain_id: U256,
    ) -> SigningResult<Self::SignedTransaction> {
        Ok(SignedUserOperationV0_7 {
            unsigned: self,
            signature: Signature::new(signature),
        })
    }
}

pub struct SignedUserOperationV0_7 {
    unsigned: UserOperationV0_7,
    signature: Signature,
}

impl TransactionCommon for SignedUserOperationV0_7 {
    #[inline]
    fn payload(&self) -> Data {
        self.unsigned.payload.clone()
    }
}

impl SignedTransaction for SignedUserOperationV0_7 {
    type Signature = Signature;

    fn encode(&self) -> Data {
        let mut signature = self.signature.to_rsv_bytes();
        signature[64] += 27;

        let prefix = true;
        let tx = SignedUserOperationV0_7Serde {
            call_data: hex::encode(&self.unsigned.payload, prefix),
            init_code: hex::encode(&self.unsigned.init_code, prefix),
            gas_fees: hex::encode(&self.unsigned.gas_fees, prefix),
            nonce: self.unsigned.nonce.to_string(),
            paymaster_and_data: hex::encode(&self.unsigned.paymaster_and_data, prefix),
            pre_verification_gas: self.unsigned.pre_verification_gas.to_string(),
            sender: self.unsigned.sender.to_string(),
            signature: hex::encode(signature.as_slice(), prefix),
            account_gas_limits: hex::encode(&self.unsigned.account_gas_limits, prefix),
        };
        serde_json::to_string(&tx)
            .expect("Simple structure should never fail on serialization")
            .into_bytes()
    }

    #[inline]
    fn signature(&self) -> &Self::Signature {
        &self.signature
    }
}

#[derive(Serialize)]
#[serde(rename_all = "camelCase")]
struct SignedUserOperationV0_7Serde {
    call_data: String,
    init_code: String,
    nonce: String,
    paymaster_and_data: String,
    pre_verification_gas: String,
    sender: String,
    signature: String,
    account_gas_limits: String,
    gas_fees: String,
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::str::FromStr;
    use tw_encoding::hex::DecodeHex;

    // Replicates https://github.com/alchemyplatform/rundler/blob/0caa06ce10717a2214c554995a8fb9f4bd18fa4b/crates/types/src/user_operation/v0_7.rs#L1189
    #[test]
    fn test_encode_user_operation() {
        let chain_id = U256::from(11155111u64);

        let user_op = UserOperationV0_7 {
            nonce: U256::from_str("0xF83D07238A7C8814A48535035602123AD6DBFA63000000000000000000000001").unwrap(),
            entry_point: Address::from("0x0000000071727De22E5E9d8BAf0edAc6f37da032"),
            sender: Address::from("0xb292Cf4a8E1fF21Ac27C4f94071Cd02C022C414b"),
            init_code: Vec::default(),
            account_gas_limits: "000000000000000000000000000114fc0000000000000000000000000012c9b5".decode_hex().unwrap(),
            gas_fees: "000000000000000000000000524121000000000000000000000000109a4a441a".decode_hex().unwrap(),
            pre_verification_gas: U256::from(48916u64),
            paymaster_and_data: Vec::default(),
            payload: "e9ae5c530000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000004000000000000000000000000000000000000000000000000000000000000001d8b292cf4a8e1ff21ac27c4f94071cd02c022c414b00000000000000000000000000000000000000000000000000000000000000009517e29f0000000000000000000000000000000000000000000000000000000000000002000000000000000000000000ad6330089d9a1fe89f4020292e1afe9969a5a2fc00000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000120000000000000000000000000000000000000000000000000000000000001518000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000018e2fbe8980000000000000000000000000000000000000000000000000000000000000008000000000000000000000000000000000000000000000000000000000000000800000000000000000000000002372912728f93ab3daaaebea4f87e6e28476d987000000000000000000000000000000000000000000000000002386f26fc10000000000000000000000000000000000000000000000000000000000000000006000000000000000000000000000000000000000000000000000000000000000000000000000000000".decode_hex().unwrap(),
        };

        let encoded = hex::encode(user_op.encode(chain_id), false);
        let expected = "000000000000000000000000b292cf4a8e1ff21ac27c4f94071cd02c022c414bf83d07238a7c8814a48535035602123ad6dbfa63000000000000000000000001c5d2460186f7233c927e7db2dcc703c0e500b653ca82273b7bfad8045d85a470f1b8863cae5d3c89d78ce8e239e0c416de4c9224226a78fbb6c4af63ed0eebf7000000000000000000000000000114fc0000000000000000000000000012c9b5000000000000000000000000000000000000000000000000000000000000bf14000000000000000000000000524121000000000000000000000000109a4a441ac5d2460186f7233c927e7db2dcc703c0e500b653ca82273b7bfad8045d85a470";
        assert_eq!(encoded, expected);

        let pre_hash = user_op.pre_hash(chain_id);
        let expected_pre_hash =
            H256::from("e486401370d145766c3cf7ba089553214a1230d38662ae532c9b62eb6dadcf7e");
        assert_eq!(pre_hash, expected_pre_hash);
    }
}
