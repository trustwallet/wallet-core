// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::abi::{convert_address, convert_u256};
use crate::address::Address;
use crate::transaction::signature::Signature;
use crate::transaction::{SignedTransaction, TransactionCommon, UnsignedTransaction};
use ethabi::Token;
use serde::Serialize;
use tw_encoding::hex;
use tw_hash::sha3::keccak256;
use tw_hash::H256;
use tw_number::U256;

pub struct UserOperation {
    pub nonce: U256,
    pub entry_point: Address,
    pub sender: Address,
    pub init_code: Vec<u8>,
    pub gas_limit: U256,
    pub verification_gas_limit: U256,
    pub max_fee_per_gas: U256,
    pub max_inclusion_fee_per_gas: U256,
    pub pre_verification_gas: U256,
    pub paymaster_and_data: Vec<u8>,
    pub payload: Vec<u8>,
}

impl TransactionCommon for UserOperation {
    fn payload(&self) -> Vec<u8> {
        self.payload.clone()
    }
}

impl UnsignedTransaction for UserOperation {
    type SignedTransaction = SignedUserOperation;

    fn pre_hash(&self, chain_id: U256) -> H256 {
        let encode_hash = keccak256(&self.encode(chain_id));

        let tokens = [
            Token::FixedBytes(encode_hash),
            Token::Address(convert_address(self.entry_point)),
            Token::Uint(convert_u256(chain_id)),
        ];
        let encoded = ethabi::encode(&tokens);
        let pre_hash = keccak256(&encoded);
        H256::try_from(pre_hash.as_slice()).expect("keccak256 returns 32 bytes")
    }

    fn encode(&self, _chain_id: U256) -> Vec<u8> {
        let init_code_hash = keccak256(&self.init_code);
        let payload_hash = keccak256(&self.payload);
        let paymaster_and_data_hash = keccak256(&self.paymaster_and_data);

        let tokens = [
            Token::Address(convert_address(self.sender)),
            Token::Uint(convert_u256(self.nonce)),
            Token::FixedBytes(init_code_hash),
            Token::FixedBytes(payload_hash),
            Token::Uint(convert_u256(self.gas_limit)),
            Token::Uint(convert_u256(self.verification_gas_limit)),
            Token::Uint(convert_u256(self.pre_verification_gas)),
            Token::Uint(convert_u256(self.max_fee_per_gas)),
            Token::Uint(convert_u256(self.max_inclusion_fee_per_gas)),
            Token::FixedBytes(paymaster_and_data_hash),
        ];

        ethabi::encode(&tokens)
    }

    fn into_signed(
        self,
        signature: tw_keypair::ecdsa::secp256k1::Signature,
        _chain_id: U256,
    ) -> Self::SignedTransaction {
        SignedUserOperation {
            unsigned: self,
            signature: Signature::new(signature),
        }
    }
}

pub struct SignedUserOperation {
    unsigned: UserOperation,
    signature: Signature,
}

impl TransactionCommon for SignedUserOperation {
    fn payload(&self) -> Vec<u8> {
        self.unsigned.payload.clone()
    }
}

impl SignedTransaction for SignedUserOperation {
    type Signature = Signature;

    fn encode(&self) -> Vec<u8> {
        let mut signature = self.signature.to_rsv_bytes();
        signature[64] += 27;

        let prefix = true;
        let tx = SignedUserOperationSerde {
            sender: self.unsigned.sender.to_string(),
            nonce: self.unsigned.nonce.to_string(),
            init_code: hex::encode(&self.unsigned.init_code, prefix),
            call_data: hex::encode(&self.unsigned.payload, prefix),
            call_gas_limit: self.unsigned.gas_limit.to_string(),
            verification_gas_limit: self.unsigned.verification_gas_limit.to_string(),
            max_fee_per_gas: self.unsigned.max_fee_per_gas.to_string(),
            max_priority_fee_per_gas: self.unsigned.max_inclusion_fee_per_gas.to_string(),
            paymaster_and_data: hex::encode(&self.unsigned.paymaster_and_data, prefix),
            pre_verification_gas: self.unsigned.pre_verification_gas.to_string(),
            signature: hex::encode(signature.as_slice(), prefix),
        };
        serde_json::to_string(&tx)
            .expect("Simple structure should never fail on serialization")
            .into_bytes()
    }

    fn signature(&self) -> &Self::Signature {
        &self.signature
    }
}

#[derive(Serialize)]
#[serde(rename_all = "camelCase")]
struct SignedUserOperationSerde {
    sender: String,
    nonce: String,
    init_code: String,
    call_data: String,
    call_gas_limit: String,
    verification_gas_limit: String,
    max_fee_per_gas: String,
    max_priority_fee_per_gas: String,
    paymaster_and_data: String,
    pre_verification_gas: String,
    signature: String,
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::abi::prebuild::erc4337::{Erc4337SimpleAccount, ExecuteArgs};

    #[test]
    fn test_encode_user_operation() {
        let chain_id = U256::from(97u64);

        let execute_args = ExecuteArgs {
            to: Address::from("0x61061fCAE11fD5461535e134EfF67A98CFFF44E9"),
            value: U256::from(0x2_386f_26fc_10000u64),
            data: Vec::default(),
        };
        let payload = Erc4337SimpleAccount::encode_execute(execute_args).unwrap();

        let user_op = UserOperation {
            nonce: U256::from(2u64),
            entry_point: Address::from("0x5FF137D4b0FDCD49DcA30c7CF57E578a026d2789"),
            sender: Address::from("0xb16Db98B365B1f89191996942612B14F1Da4Bd5f"),
            init_code: Vec::default(),
            gas_limit: U256::from(0x186a0u64),
            verification_gas_limit: U256::from(0x186a0u64),
            max_fee_per_gas: U256::from(0x1_a339_c9e9u64),
            max_inclusion_fee_per_gas: U256::from(0x1_a339_c9e9u64),
            pre_verification_gas: U256::from(0xb708u64),
            paymaster_and_data: Vec::default(),
            payload,
        };

        let encoded = hex::encode(user_op.encode(chain_id), false);
        let expected = "000000000000000000000000b16db98b365b1f89191996942612b14f1da4bd5f0000000000000000000000000000000000000000000000000000000000000002c5d2460186f7233c927e7db2dcc703c0e500b653ca82273b7bfad8045d85a470fbec3c1db0378685d954edd265aa6eb11e8474d828e6bda151810263838e457000000000000000000000000000000000000000000000000000000000000186a000000000000000000000000000000000000000000000000000000000000186a0000000000000000000000000000000000000000000000000000000000000b70800000000000000000000000000000000000000000000000000000001a339c9e900000000000000000000000000000000000000000000000000000001a339c9e9c5d2460186f7233c927e7db2dcc703c0e500b653ca82273b7bfad8045d85a470";
        assert_eq!(encoded, expected);

        let pre_hash = user_op.pre_hash(chain_id);
        let expected_pre_hash =
            H256::from("2d37191a8688f69090451ed90a0a9ba69d652c2062ee9d023b3ebe964a3ed2ae");
        assert_eq!(pre_hash, expected_pre_hash);
    }
}
