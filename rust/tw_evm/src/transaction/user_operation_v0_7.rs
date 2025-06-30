// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::abi::encode::encode_tokens;
use crate::abi::non_empty_array::NonEmptyBytes;
use crate::abi::token::Token;
use crate::address::Address;
use crate::signature::legacy_replay_protection_u8;
use crate::transaction::signature::Signature;
use crate::transaction::{SignedTransaction, TransactionCommon, UnsignedTransaction};
use serde::ser::Error as SerError;
use serde::{Serialize, Serializer};
use tw_coin_entry::error::prelude::*;
use tw_encoding::hex::{self, as_hex_prefixed};
use tw_hash::sha3::keccak256;
use tw_hash::H256;
use tw_memory::Data;
use tw_number::serde::as_u256_hex;
use tw_number::U256;

use super::authorization_list::SignedAuthorization;

pub struct PackedUserOperation {
    pub sender: Address,
    pub nonce: U256,
    pub init_code: Data,
    pub call_data: Data,
    pub account_gas_limits: Data,
    pub pre_verification_gas: U256,
    pub gas_fees: Data,
    pub paymaster_and_data: Data,
}

impl PackedUserOperation {
    // Taken from https://github.com/alchemyplatform/rundler/blob/0caa06ce10717a2214c554995a8fb9f4bd18fa4b/crates/types/src/user_operation/v0_7.rs#L1017
    fn new(user_op: &UserOperationV0_7) -> Self {
        let init_code = if let Some(factory) = user_op.factory {
            let mut init_code = factory.bytes().into_vec();
            init_code.extend_from_slice(&user_op.factory_data);
            init_code
        } else {
            vec![]
        };

        let account_gas_limits =
            concat_u128_be(user_op.verification_gas_limit, user_op.call_gas_limit);
        let gas_fees = concat_u128_be(user_op.max_priority_fee_per_gas, user_op.max_fee_per_gas);

        let paymaster_and_data = if let Some(paymaster) = user_op.paymaster {
            let mut paymaster_and_data = paymaster.bytes().into_vec();
            paymaster_and_data
                .extend_from_slice(&user_op.paymaster_verification_gas_limit.to_be_bytes());
            paymaster_and_data
                .extend_from_slice(&user_op.paymaster_post_op_gas_limit.to_be_bytes());
            paymaster_and_data.extend_from_slice(&user_op.paymaster_data);
            paymaster_and_data
        } else {
            vec![]
        };

        Self {
            sender: user_op.sender,
            nonce: user_op.nonce,
            init_code,
            call_data: user_op.call_data.clone(),
            account_gas_limits: account_gas_limits.to_vec(),
            pre_verification_gas: user_op.pre_verification_gas,
            gas_fees: gas_fees.to_vec(),
            paymaster_and_data,
        }
    }

    fn pre_hash(&self, chain_id: U256, entry_point: Address) -> H256 {
        let encode_hash = keccak256(&self.encode());
        let encode_hash =
            NonEmptyBytes::new(encode_hash).expect("keccak256 must not return an empty hash");

        let tokens = [
            Token::FixedBytes(encode_hash),
            Token::Address(entry_point),
            Token::u256(chain_id),
        ];
        let encoded = encode_tokens(&tokens);
        let pre_hash = keccak256(&encoded);
        H256::try_from(pre_hash.as_slice()).expect("keccak256 returns 32 bytes")
    }

    fn encode(&self) -> Data {
        let init_code_hash = keccak256(&self.init_code);
        let init_code_hash =
            NonEmptyBytes::new(init_code_hash).expect("keccak256 must not return an empty hash");

        let call_data_hash = keccak256(&self.call_data);
        let call_data_hash =
            NonEmptyBytes::new(call_data_hash).expect("keccak256 must not return an empty hash");

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
            Token::FixedBytes(call_data_hash),
            Token::FixedBytes(account_gas_limits),
            Token::u256(self.pre_verification_gas),
            Token::FixedBytes(gas_fees),
            Token::FixedBytes(paymaster_and_data_hash),
        ];

        encode_tokens(&tokens)
    }
}

#[derive(Serialize)]
#[serde(rename_all = "camelCase")]
pub struct UserOperationV0_7 {
    pub sender: Address,
    #[serde(serialize_with = "U256::as_hex")]
    pub nonce: U256,

    #[serde(skip_serializing_if = "Option::is_none")]
    pub factory: Option<Address>,
    #[serde(skip_serializing_if = "Data::is_empty")]
    #[serde(with = "as_hex_prefixed")]
    pub factory_data: Data,

    #[serde(with = "as_hex_prefixed")]
    pub call_data: Data,

    #[serde(serialize_with = "as_u256_hex")]
    pub call_gas_limit: u128,
    #[serde(serialize_with = "as_u256_hex")]
    pub verification_gas_limit: u128,
    #[serde(serialize_with = "U256::as_hex")]
    pub pre_verification_gas: U256,

    #[serde(serialize_with = "as_u256_hex")]
    pub max_fee_per_gas: u128,
    #[serde(serialize_with = "as_u256_hex")]
    pub max_priority_fee_per_gas: u128,

    #[serde(skip_serializing_if = "Option::is_none")]
    pub paymaster: Option<Address>,
    #[serde(serialize_with = "as_u256_hex")]
    pub paymaster_verification_gas_limit: u128,
    #[serde(serialize_with = "as_u256_hex")]
    pub paymaster_post_op_gas_limit: u128,
    #[serde(skip_serializing_if = "Data::is_empty")]
    #[serde(with = "as_hex_prefixed")]
    pub paymaster_data: Data,

    #[serde(skip_serializing_if = "Option::is_none")]
    pub eip7702_auth: Option<SignedAuthorization>,

    #[serde(skip)]
    pub entry_point: Address,
}

impl TransactionCommon for UserOperationV0_7 {
    #[inline]
    fn payload(&self) -> Data {
        self.call_data.clone()
    }
}

impl UnsignedTransaction for UserOperationV0_7 {
    type SignedTransaction = SignedUserOperationV0_7;

    fn pre_hash(&self, chain_id: U256) -> H256 {
        PackedUserOperation::new(self).pre_hash(chain_id, self.entry_point)
    }

    fn encode(&self, _chain_id: U256) -> Data {
        PackedUserOperation::new(self).encode()
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

#[derive(Serialize)]
#[serde(rename_all = "camelCase")]
pub struct SignedUserOperationV0_7 {
    #[serde(flatten)]
    unsigned: UserOperationV0_7,
    #[serde(serialize_with = "serialize_signature_with_legacy_replay_protect")]
    signature: Signature,
}

impl TransactionCommon for SignedUserOperationV0_7 {
    #[inline]
    fn payload(&self) -> Data {
        self.unsigned.call_data.clone()
    }
}

impl SignedTransaction for SignedUserOperationV0_7 {
    type Signature = Signature;

    fn encode(&self) -> Data {
        serde_json::to_string(self)
            .expect("Simple structure should never fail on serialization")
            .into_bytes()
    }

    #[inline]
    fn signature(&self) -> &Self::Signature {
        &self.signature
    }
}

fn concat_u128_be(a: u128, b: u128) -> [u8; 32] {
    let a = a.to_be_bytes();
    let b = b.to_be_bytes();
    std::array::from_fn(|i| {
        if let Some(i) = i.checked_sub(a.len()) {
            b[i]
        } else {
            a[i]
        }
    })
}

pub fn serialize_signature_with_legacy_replay_protect<S>(
    signature: &Signature,
    serializer: S,
) -> Result<S::Ok, S::Error>
where
    S: Serializer,
{
    let prefix = true;
    let mut rsv = signature.to_rsv_bytes();
    rsv[64] =
        legacy_replay_protection_u8(rsv[64]).map_err(|e| SerError::custom(format!("{e:?}")))?;

    let hex_str = hex::encode(rsv, prefix);
    serializer.serialize_str(&hex_str)
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::str::FromStr;
    use tw_encoding::hex::{DecodeHex, ToHex};

    // Replicates https://github.com/alchemyplatform/rundler/blob/0caa06ce10717a2214c554995a8fb9f4bd18fa4b/crates/types/src/user_operation/v0_7.rs#L1189
    #[test]
    fn test_encode_user_operation() {
        let chain_id = U256::from(11155111u64);
        let entry_point = Address::from("0x0000000071727De22E5E9d8BAf0edAc6f37da032");

        let user_op = UserOperationV0_7 {
            sender: Address::from("0xb292Cf4a8E1fF21Ac27C4f94071Cd02C022C414b"),
            nonce: U256::from_str("0xF83D07238A7C8814A48535035602123AD6DBFA63000000000000000000000001").unwrap(),
            factory: None,
            factory_data: Vec::default(),
            call_data: "e9ae5c530000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000004000000000000000000000000000000000000000000000000000000000000001d8b292cf4a8e1ff21ac27c4f94071cd02c022c414b00000000000000000000000000000000000000000000000000000000000000009517e29f0000000000000000000000000000000000000000000000000000000000000002000000000000000000000000ad6330089d9a1fe89f4020292e1afe9969a5a2fc00000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000120000000000000000000000000000000000000000000000000000000000001518000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000018e2fbe8980000000000000000000000000000000000000000000000000000000000000008000000000000000000000000000000000000000000000000000000000000000800000000000000000000000002372912728f93ab3daaaebea4f87e6e28476d987000000000000000000000000000000000000000000000000002386f26fc10000000000000000000000000000000000000000000000000000000000000000006000000000000000000000000000000000000000000000000000000000000000000000000000000000".decode_hex().unwrap(),
            call_gas_limit: 1231285u128,
            verification_gas_limit: 70908u128,
            pre_verification_gas: U256::from(48916u64),
            max_fee_per_gas: 71308035098u128,
            max_priority_fee_per_gas: 1380000000u128,
            paymaster: None,
            paymaster_verification_gas_limit: 0,
            paymaster_post_op_gas_limit: 0,
            paymaster_data: Vec::default(),
            eip7702_auth: None,
            entry_point,
        };

        let packed_user_op = PackedUserOperation {
            sender: Address::from("0xb292Cf4a8E1fF21Ac27C4f94071Cd02C022C414b"),
            nonce: U256::from_str("0xF83D07238A7C8814A48535035602123AD6DBFA63000000000000000000000001").unwrap(),
            init_code: Vec::default(),
            call_data: "e9ae5c530000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000004000000000000000000000000000000000000000000000000000000000000001d8b292cf4a8e1ff21ac27c4f94071cd02c022c414b00000000000000000000000000000000000000000000000000000000000000009517e29f0000000000000000000000000000000000000000000000000000000000000002000000000000000000000000ad6330089d9a1fe89f4020292e1afe9969a5a2fc00000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000120000000000000000000000000000000000000000000000000000000000001518000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000018e2fbe8980000000000000000000000000000000000000000000000000000000000000008000000000000000000000000000000000000000000000000000000000000000800000000000000000000000002372912728f93ab3daaaebea4f87e6e28476d987000000000000000000000000000000000000000000000000002386f26fc10000000000000000000000000000000000000000000000000000000000000000006000000000000000000000000000000000000000000000000000000000000000000000000000000000".decode_hex().unwrap(),
            account_gas_limits: "000000000000000000000000000114fc0000000000000000000000000012c9b5".decode_hex().unwrap(),
            pre_verification_gas: U256::from(48916u64),
            gas_fees: "000000000000000000000000524121000000000000000000000000109a4a441a".decode_hex().unwrap(),
            paymaster_and_data: Vec::default(),
        };

        let encoded_from_user_op = hex::encode(user_op.encode(chain_id), false);
        let encoded_from_packed_user_op = hex::encode(packed_user_op.encode(), false);
        let expected = "000000000000000000000000b292cf4a8e1ff21ac27c4f94071cd02c022c414bf83d07238a7c8814a48535035602123ad6dbfa63000000000000000000000001c5d2460186f7233c927e7db2dcc703c0e500b653ca82273b7bfad8045d85a470f1b8863cae5d3c89d78ce8e239e0c416de4c9224226a78fbb6c4af63ed0eebf7000000000000000000000000000114fc0000000000000000000000000012c9b5000000000000000000000000000000000000000000000000000000000000bf14000000000000000000000000524121000000000000000000000000109a4a441ac5d2460186f7233c927e7db2dcc703c0e500b653ca82273b7bfad8045d85a470";
        assert_eq!(encoded_from_user_op, expected);
        assert_eq!(encoded_from_packed_user_op, expected);

        let pre_hash_from_user_op = user_op.pre_hash(chain_id);
        let pre_hash_from_packed_user_op = packed_user_op.pre_hash(chain_id, entry_point);
        let expected_pre_hash =
            H256::from("e486401370d145766c3cf7ba089553214a1230d38662ae532c9b62eb6dadcf7e");
        assert_eq!(pre_hash_from_user_op, expected_pre_hash);
        assert_eq!(pre_hash_from_packed_user_op, expected_pre_hash);
    }

    #[test]
    fn test_encode_user_operation_2() {
        let chain_id = U256::from(31337u64);
        let entry_point = Address::from("0x0000000071727De22E5E9d8BAf0edAc6f37da032");

        let user_op = UserOperationV0_7 {
            sender: Address::from("0x174a240e5147D02dE4d7724D5D3E1c1bF11cE029"),
            nonce: U256::from(0u64),
            factory: Some(Address::from("0xf471789937856d80e589f5996cf8b0511ddd9de4")),
            factory_data: "f471789937856d80e589f5996cf8b0511ddd9de4".decode_hex().unwrap(),
            call_data: "00".decode_hex().unwrap(),
            call_gas_limit: 100000u128,
            verification_gas_limit: 100000u128,
            pre_verification_gas: U256::from(1000000u64),
            max_fee_per_gas: 100000u128,
            max_priority_fee_per_gas: 100000u128,
            paymaster: Some(Address::from("0xf62849f9a0b5bf2913b396098f7c7019b51a820a")),
            paymaster_verification_gas_limit: 99999u128,
            paymaster_post_op_gas_limit: 88888u128,
            paymaster_data: "00000000000b0000000000002e234dae75c793f67a35089c9d99245e1c58470b00000000000000000000000000000000000000000000000000000000000186a0072f35038bcacc31bcdeda87c1d9857703a26fb70a053f6e87da5a4e7a1e1f3c4b09fbe2dbff98e7a87ebb45a635234f4b79eff3225d07560039c7764291c97e1b".decode_hex().unwrap(),
            eip7702_auth: None,
            entry_point: Address::from("0x0000000071727De22E5E9d8BAf0edAc6f37da032"),
        };
        let packed_user_op = PackedUserOperation::new(&user_op);

        let expected_packed_user_op = PackedUserOperation {
            sender: Address::from("0x174a240e5147D02dE4d7724D5D3E1c1bF11cE029"),
            nonce: U256::from(0u64),
            init_code: "f471789937856d80e589f5996cf8b0511ddd9de4f471789937856d80e589f5996cf8b0511ddd9de4".decode_hex().unwrap(),
            call_data: "00".decode_hex().unwrap(),
            account_gas_limits: concat_u128_be(100000u128, 100000u128).to_vec(),
            pre_verification_gas: U256::from(1000000u64),
            gas_fees: concat_u128_be(100000u128, 100000u128).to_vec(),
            paymaster_and_data: "f62849f9a0b5bf2913b396098f7c7019b51a820a0000000000000000000000000001869f00000000000000000000000000015b3800000000000b0000000000002e234dae75c793f67a35089c9d99245e1c58470b00000000000000000000000000000000000000000000000000000000000186a0072f35038bcacc31bcdeda87c1d9857703a26fb70a053f6e87da5a4e7a1e1f3c4b09fbe2dbff98e7a87ebb45a635234f4b79eff3225d07560039c7764291c97e1b".decode_hex().unwrap(),
        };
        assert_eq!(packed_user_op.init_code, expected_packed_user_op.init_code);
        assert_eq!(
            packed_user_op.paymaster_and_data,
            expected_packed_user_op.paymaster_and_data
        );

        let pre_hash = packed_user_op.pre_hash(chain_id, entry_point);
        assert_eq!(
            H256::to_hex(&pre_hash),
            "648c415fb11c6fad4b274a0aea51e808e6e3ad2e500d5e76ee2fb61e7ea4d07c"
        );
        let expected_pre_hash = expected_packed_user_op.pre_hash(chain_id, entry_point);
        assert_eq!(
            H256::to_hex(&expected_pre_hash),
            "648c415fb11c6fad4b274a0aea51e808e6e3ad2e500d5e76ee2fb61e7ea4d07c"
        );
    }
}
