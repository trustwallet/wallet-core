// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use std::str::FromStr;
use move_core_types::account_address::AccountAddress;
use move_core_types::language_storage::TypeTag;
use tw_proto::Aptos::Proto::mod_SigningInput::OneOftransaction_payload;
use crate::constants::{GAS_UNIT_PRICE, MAX_GAS_AMOUNT};
use crate::transaction::RawTransaction;
use crate::transaction_payload::{convert_proto_struct_tag_to_type_tag, EntryFunction, TransactionPayload};
use tw_proto::Aptos::Proto::SigningInput;
use crate::aptos_move_packages::{aptos_account_create_account, aptos_account_transfer, coin_transfer};

pub struct TransactionBuilder {
    sender: Option<AccountAddress>,
    sequence_number: Option<u64>,
    payload: TransactionPayload,
    max_gas_amount: u64,
    gas_unit_price: u64,
    expiration_timestamp_secs: u64,
    chain_id: u8,
}

impl TransactionBuilder {
    pub fn new(
        payload: TransactionPayload,
        expiration_timestamp_secs: u64,
        chain_id: u8,
    ) -> Self {
        Self {
            payload,
            chain_id,
            expiration_timestamp_secs,
            max_gas_amount: MAX_GAS_AMOUNT,
            gas_unit_price: std::cmp::max(GAS_UNIT_PRICE, 1),
            sender: None,
            sequence_number: None,
        }
    }

    pub fn sender(mut self, sender: AccountAddress) -> Self {
        self.sender = Some(sender);
        self
    }

    pub fn sequence_number(mut self, sequence_number: u64) -> Self {
        self.sequence_number = Some(sequence_number);
        self
    }

    pub fn max_gas_amount(mut self, max_gas_amount: u64) -> Self {
        self.max_gas_amount = max_gas_amount;
        self
    }

    pub fn gas_unit_price(mut self, gas_unit_price: u64) -> Self {
        self.gas_unit_price = gas_unit_price;
        self
    }

    pub fn chain_id(mut self, chain_id: u8) -> Self {
        self.chain_id = chain_id;
        self
    }

    pub fn expiration_timestamp_secs(mut self, expiration_timestamp_secs: u64) -> Self {
        self.expiration_timestamp_secs = expiration_timestamp_secs;
        self
    }

    pub fn build(self) -> RawTransaction {
        RawTransaction::new(
            self.sender.expect("sender must have been set"),
            self.sequence_number
                .expect("sequence number must have been set"),
            self.payload,
            self.max_gas_amount,
            self.gas_unit_price,
            self.expiration_timestamp_secs,
            self.chain_id,
        )
    }
}

#[derive(Clone, Debug)]
pub struct TransactionFactory {
    max_gas_amount: u64,
    gas_unit_price: u64,
    transaction_expiration_time: u64,
    chain_id: u8,
}

impl TransactionFactory {
    pub fn new(chain_id: u8) -> Self {
        Self {
            max_gas_amount: MAX_GAS_AMOUNT,
            gas_unit_price: GAS_UNIT_PRICE,
            transaction_expiration_time: 30,
            chain_id,
        }
    }

    pub fn new_from_protobuf(input: SigningInput) -> TransactionBuilder {
        let factory = TransactionFactory::new(input.chain_id as u8)
            .with_gas_unit_price(input.gas_unit_price)
            .with_max_gas_amount(input.max_gas_amount)
            .with_transaction_expiration_time(input.expiration_timestamp_secs);
        match input.transaction_payload {
            OneOftransaction_payload::transfer(transfer) => {
                return factory.implicitly_create_user_account_and_transfer(AccountAddress::from_str(&transfer.to).unwrap(), transfer.amount);
            }
            OneOftransaction_payload::token_transfer(token_transfer) => {
                let func = token_transfer.function.unwrap();
                return factory.coins_transfer(AccountAddress::from_str(&token_transfer.to).unwrap(), token_transfer.amount,
                                              convert_proto_struct_tag_to_type_tag(func)
                )
            }
            OneOftransaction_payload::create_account(create_account) => {
                return factory.create_user_account(AccountAddress::from_str(&create_account.auth_key).unwrap());
            }
            OneOftransaction_payload::nft_message(_) => {}
            OneOftransaction_payload::register_token(_) => {}
            OneOftransaction_payload::liquid_staking_message(_) => {}
            OneOftransaction_payload::token_transfer_coins(_) => {}
            OneOftransaction_payload::None => {}
        }
        todo!()
    }

    pub fn with_max_gas_amount(mut self, max_gas_amount: u64) -> Self {
        self.max_gas_amount = max_gas_amount;
        self
    }

    pub fn with_gas_unit_price(mut self, gas_unit_price: u64) -> Self {
        self.gas_unit_price = gas_unit_price;
        self
    }

    pub fn with_transaction_expiration_time(mut self, transaction_expiration_time: u64) -> Self {
        self.transaction_expiration_time = transaction_expiration_time;
        self
    }

    pub fn with_chain_id(mut self, chain_id: u8) -> Self {
        self.chain_id = chain_id;
        self
    }

    pub fn get_max_gas_amount(&self) -> u64 {
        self.max_gas_amount
    }

    pub fn get_gas_unit_price(&self) -> u64 {
        self.gas_unit_price
    }

    pub fn get_transaction_expiration_time(&self) -> u64 {
        self.transaction_expiration_time
    }

    pub fn payload(&self, payload: TransactionPayload) -> TransactionBuilder {
        self.transaction_builder(payload)
    }

    pub fn entry_function(&self, func: EntryFunction) -> TransactionBuilder {
        self.payload(TransactionPayload::EntryFunction(func))
    }

    pub fn create_user_account(&self, to: AccountAddress) -> TransactionBuilder {
        self.payload(aptos_account_create_account(to))
    }

    pub fn implicitly_create_user_account_and_transfer(
        &self,
        to: AccountAddress,
        amount: u64,
    ) -> TransactionBuilder {
        self.payload(aptos_account_transfer(to, amount))
    }

    pub fn coins_transfer(&self, to: AccountAddress,
                          amount: u64, coin_type: TypeTag) -> TransactionBuilder {
        self.payload(coin_transfer(coin_type, to, amount))
    }
    pub fn transfer(&self, _to: AccountAddress, _amount: u64) -> TransactionBuilder {
        todo!()
    }

    pub fn account_transfer(&self, _to: AccountAddress, _amount: u64) -> TransactionBuilder {
        todo!()
    }

    fn transaction_builder(&self, payload: TransactionPayload) -> TransactionBuilder {
        TransactionBuilder {
            sender: None,
            sequence_number: None,
            payload,
            max_gas_amount: self.max_gas_amount,
            gas_unit_price: self.gas_unit_price,
            expiration_timestamp_secs: self.expiration_timestamp(),
            chain_id: self.chain_id,
        }
    }

    fn expiration_timestamp(&self) -> u64 {
        self.transaction_expiration_time
    }
}

#[cfg(test)]
mod tests {
    use std::str::FromStr;
    use move_core_types::account_address::AccountAddress;
    use move_core_types::language_storage::TypeTag;
    use tw_encoding::hex;
    use crate::transaction_builder::TransactionFactory;

    fn setup_transaction(
        sender: &str,
        keypair_str: &str,
        transaction_type: &str,
        sequence_number: u64,
        to: &str,
        chain_id: u8,
        amount: u64,
        expected_raw_txn_bytes_str: &str,
        expected_signature_str: &str,
        expected_encoded_txn_str: &str,
        json_literal: &str,
        coin_type: Option<TypeTag>,
    ) {
        let account_address = AccountAddress::from_str(sender).unwrap();
        let to = AccountAddress::from_str(to).unwrap();
        let keypair = tw_keypair::ed25519::sha512::KeyPair::try_from(keypair_str).unwrap();
        let factory = TransactionFactory::new(chain_id)
            .with_max_gas_amount(3296766)
            .with_gas_unit_price(100)
            .with_transaction_expiration_time(3664390082);

        let mut builder = match transaction_type {
            "transfer" => factory.implicitly_create_user_account_and_transfer(to.clone(), amount),
            "create_account" => { factory.create_user_account(to.clone()) }
            "coin_transfer" => { factory.coins_transfer(to.clone(), amount, coin_type.unwrap()) }
            _ => panic!("Unsupported transaction type"),
        };

        builder = builder.sender(account_address).sequence_number(sequence_number);
        let res = builder.build().sign(keypair).unwrap();

        assert_eq!(hex::encode(res.raw_txn_bytes(), false), expected_raw_txn_bytes_str);
        assert_eq!(hex::encode(res.authenticator().get_signature(), false), expected_signature_str);
        assert_eq!(hex::encode(res.encoded(), false), expected_encoded_txn_str);

        let json_value: serde_json::Value = serde_json::from_str(json_literal).unwrap();
        assert_eq!(res.to_json(), json_value);
    }

    #[test]
    fn test_aptos_account_transfer() {
        setup_transaction(
            "0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30",
            "5d996aa76b3212142792d9130796cd2e11e3c445a93118c08414df4f66bc60ec",
            "transfer",
            99,
            "0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30",
            33,
            1000,
            "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3063000000000000000200000000000000000000000000000000000000000000000000000000000000010d6170746f735f6163636f756e74087472616e7366657200022007968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3008e803000000000000fe4d3200000000006400000000000000c2276ada0000000021",
            "5707246db31e2335edc4316a7a656a11691d1d1647f6e864d1ab12f43428aaaf806cf02120d0b608cdd89c5c904af7b137432aacdd60cc53f9fad7bd33578e01",
            "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3063000000000000000200000000000000000000000000000000000000000000000000000000000000010d6170746f735f6163636f756e74087472616e7366657200022007968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3008e803000000000000fe4d3200000000006400000000000000c2276ada00000000210020ea526ba1710343d953461ff68641f1b7df5f23b9042ffa2d2a798d3adb3f3d6c405707246db31e2335edc4316a7a656a11691d1d1647f6e864d1ab12f43428aaaf806cf02120d0b608cdd89c5c904af7b137432aacdd60cc53f9fad7bd33578e01",
            r#"{
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
        }"#,
            None,
        );
    }

    #[test]
    fn test_aptos_create_account() {
        setup_transaction(
            "0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30", // Sender's address
            "5d996aa76b3212142792d9130796cd2e11e3c445a93118c08414df4f66bc60ec", // Keypair
            "create_account",
            0, // Sequence number
            "0x3aa1672641a4e17b3d913b4c0301e805755a80b12756fc729c5878f12344d30e",
            33,
            0,
            "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3000000000000000000200000000000000000000000000000000000000000000000000000000000000010d6170746f735f6163636f756e740e6372656174655f6163636f756e740001203aa1672641a4e17b3d913b4c0301e805755a80b12756fc729c5878f12344d30efe4d3200000000006400000000000000c2276ada0000000021", // Expected raw transaction bytes
            "fcba3dfbec76721454ef414955f09f159660a13886b4edd8c579e3c779c29073afe7b25efa3fef9b21c2efb1cf16b4247fc0e5c8f63fdcd1c8d87f5d59f44501", // Expected signature
            "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3000000000000000000200000000000000000000000000000000000000000000000000000000000000010d6170746f735f6163636f756e740e6372656174655f6163636f756e740001203aa1672641a4e17b3d913b4c0301e805755a80b12756fc729c5878f12344d30efe4d3200000000006400000000000000c2276ada00000000210020ea526ba1710343d953461ff68641f1b7df5f23b9042ffa2d2a798d3adb3f3d6c40fcba3dfbec76721454ef414955f09f159660a13886b4edd8c579e3c779c29073afe7b25efa3fef9b21c2efb1cf16b4247fc0e5c8f63fdcd1c8d87f5d59f44501", // Expected encoded transaction
            r#"{
            "expiration_timestamp_secs": "3664390082",
            "gas_unit_price": "100",
            "max_gas_amount": "3296766",
            "payload": {
                "arguments": ["0x3aa1672641a4e17b3d913b4c0301e805755a80b12756fc729c5878f12344d30e"],
                "function": "0x1::aptos_account::create_account",
                "type": "entry_function_payload",
                "type_arguments": []
            },
            "sender": "0x7968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30",
            "sequence_number": "0",
            "signature": {
                "public_key": "0xea526ba1710343d953461ff68641f1b7df5f23b9042ffa2d2a798d3adb3f3d6c",
                "signature": "0xfcba3dfbec76721454ef414955f09f159660a13886b4edd8c579e3c779c29073afe7b25efa3fef9b21c2efb1cf16b4247fc0e5c8f63fdcd1c8d87f5d59f44501",
                "type": "ed25519_signature"
            }
        }"#, // Expected JSON literal
            None,
        );
    }

    #[test]
    fn test_aptos_coin_transfer() {
        setup_transaction(
            "0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30", // Sender's address
            "5d996aa76b3212142792d9130796cd2e11e3c445a93118c08414df4f66bc60ec", // Keypair
            "coin_transfer",
            24, // Sequence number
            "0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30",
            32,
            100000,
            "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30180000000000000002000000000000000000000000000000000000000000000000000000000000000104636f696e087472616e73666572010743417434fd869edee76cca2a4d2301e528a1551b1d719b75c350c3c97d15b8b905636f696e730342544300022007968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3008a086010000000000fe4d3200000000006400000000000000c2276ada0000000020", // Expected raw transaction bytes
            "7643ec8aae6198bd13ca6ea2962265859cba5a228e7d181131f6c022700dd02a7a04dc0345ad99a0289e5ab80b130b3864e6404079980bc226f1a13aee7d280a", // Expected signature
            "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30180000000000000002000000000000000000000000000000000000000000000000000000000000000104636f696e087472616e73666572010743417434fd869edee76cca2a4d2301e528a1551b1d719b75c350c3c97d15b8b905636f696e730342544300022007968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3008a086010000000000fe4d3200000000006400000000000000c2276ada00000000200020ea526ba1710343d953461ff68641f1b7df5f23b9042ffa2d2a798d3adb3f3d6c407643ec8aae6198bd13ca6ea2962265859cba5a228e7d181131f6c022700dd02a7a04dc0345ad99a0289e5ab80b130b3864e6404079980bc226f1a13aee7d280a", // Expected encoded transaction
            r#"{
                    "expiration_timestamp_secs": "3664390082",
                    "gas_unit_price": "100",
                    "max_gas_amount": "3296766",
                    "payload": {
                        "arguments": ["0x7968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30","100000"],
                        "function": "0x1::coin::transfer",
                        "type": "entry_function_payload",
                        "type_arguments": ["0x43417434fd869edee76cca2a4d2301e528a1551b1d719b75c350c3c97d15b8b9::coins::BTC"]
                    },
                    "sender": "0x7968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30",
                    "sequence_number": "24",
                    "signature": {
                        "public_key": "0xea526ba1710343d953461ff68641f1b7df5f23b9042ffa2d2a798d3adb3f3d6c",
                        "signature": "0x7643ec8aae6198bd13ca6ea2962265859cba5a228e7d181131f6c022700dd02a7a04dc0345ad99a0289e5ab80b130b3864e6404079980bc226f1a13aee7d280a",
                        "type": "ed25519_signature"
                    }
                }"#, // Expected JSON literal
            Some(TypeTag::from_str("0x43417434fd869edee76cca2a4d2301e528a1551b1d719b75c350c3c97d15b8b9::coins::BTC").unwrap()),
        );
    }
}
