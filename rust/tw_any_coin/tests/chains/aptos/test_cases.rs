// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use serde_json::{json, Value as Json};
use tw_proto::Aptos::Proto;
use tw_proto::Aptos::Proto::mod_SigningInput::OneOftransaction_payload as TransactionPayloadEnum;

pub(super) mod transfer_b4d62afd {
    use super::*;

    /// Expected private key.
    pub const PRIVATE_KEY: &str =
        "5d996aa76b3212142792d9130796cd2e11e3c445a93118c08414df4f66bc60ec";
    pub const ENCODED: &str = "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3063000000000000000200000000000000000000000000000000000000000000000000000000000000010d6170746f735f6163636f756e74087472616e7366657200022007968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3008e803000000000000fe4d3200000000006400000000000000c2276ada00000000210020ea526ba1710343d953461ff68641f1b7df5f23b9042ffa2d2a798d3adb3f3d6c405707246db31e2335edc4316a7a656a11691d1d1647f6e864d1ab12f43428aaaf806cf02120d0b608cdd89c5c904af7b137432aacdd60cc53f9fad7bd33578e01";
    /// Expected `raw_txn`.
    pub const RAW_TXN: &str = "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3063000000000000000200000000000000000000000000000000000000000000000000000000000000010d6170746f735f6163636f756e74087472616e7366657200022007968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3008e803000000000000fe4d3200000000006400000000000000c2276ada0000000021";
    /// Expected signature.
    pub const SIGNATURE: &str = "5707246db31e2335edc4316a7a656a11691d1d1647f6e864d1ab12f43428aaaf806cf02120d0b608cdd89c5c904af7b137432aacdd60cc53f9fad7bd33578e01";
    /// Expected preimage data to be signed.
    pub const DATA_TO_SIGN: &str = "b5e97db07fa0bd0e5598aa3643a9bc6f6693bddc1a9fec9e674a461eaa00b19307968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3063000000000000000200000000000000000000000000000000000000000000000000000000000000010d6170746f735f6163636f756e74087472616e7366657200022007968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3008e803000000000000fe4d3200000000006400000000000000c2276ada0000000021";

    pub fn aptos_sign_transfer_input() -> Proto::SigningInput<'static> {
        let transfer = Proto::TransferMessage {
            to: "0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30".into(),
            amount: 1000,
        };

        Proto::SigningInput {
            sender: "0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30".into(),
            sequence_number: 99,
            max_gas_amount: 3296766,
            gas_unit_price: 100,
            expiration_timestamp_secs: 3664390082,
            chain_id: 33,
            any_encoded: Default::default(),
            transaction_payload: TransactionPayloadEnum::transfer(transfer),
            ..Proto::SigningInput::default()
        }
    }

    pub fn expected_json() -> Json {
        json!({
            "expiration_timestamp_secs": "3664390082",
            "gas_unit_price": "100",
            "max_gas_amount": "3296766",
            "payload": {
                "arguments": ["0x7968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30","1000"],
                "function": "0x1::aptos_account::transfer",
                "type": "entry_function_payload",
                "type_arguments": []
            },
            "sender": "0x7968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30",
            "sequence_number": "99",
            "signature": {
                "public_key": "0xea526ba1710343d953461ff68641f1b7df5f23b9042ffa2d2a798d3adb3f3d6c",
                "signature": "0x5707246db31e2335edc4316a7a656a11691d1d1647f6e864d1ab12f43428aaaf806cf02120d0b608cdd89c5c904af7b137432aacdd60cc53f9fad7bd33578e01",
                "type": "ed25519_signature"
            }
        })
    }
}
