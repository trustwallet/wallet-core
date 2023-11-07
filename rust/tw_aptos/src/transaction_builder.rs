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
use crate::aptos_move_packages::{aptos_account_create_account, aptos_account_transfer, coin_transfer, managed_coin_register, token_transfers_cancel_offer_script, token_transfers_claim_script, token_transfers_offer_script};
use crate::nft::NftOperation;

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
                                              convert_proto_struct_tag_to_type_tag(func),
                );
            }
            OneOftransaction_payload::create_account(create_account) => {
                return factory.create_user_account(AccountAddress::from_str(&create_account.auth_key).unwrap());
            }
            OneOftransaction_payload::nft_message(nft_message) => {
                return factory.nft_ops(nft_message.into());
            }
            OneOftransaction_payload::register_token(register_token) => {
                return factory.register_token(convert_proto_struct_tag_to_type_tag(register_token.function.unwrap()))
            }
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

    pub fn register_token(&self, coin_type: TypeTag) -> TransactionBuilder {
        self.payload(managed_coin_register(coin_type))
    }

    pub fn nft_ops(&self, operation: NftOperation) -> TransactionBuilder {
        match operation {
            NftOperation::Claim(claim) => {
                self.payload(token_transfers_claim_script(claim.sender, claim.creator, claim.collection, claim.name, claim.property_version))
            }
            NftOperation::Cancel(offer) => {
                self.payload(token_transfers_cancel_offer_script(offer.receiver, offer.creator, offer.collection, offer.name, offer.property_version))
            }
            NftOperation::Offer(offer) => {
                self.payload(token_transfers_offer_script(offer.receiver, offer.creator, offer.collection, offer.name, offer.property_version, offer.amount))
            }
        }
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
    use crate::nft::{Offer, NftOperation, Claim};
    use crate::transaction_builder::TransactionFactory;


    pub struct AccountCreation {
        to: String,
    }
    pub struct Transfer {
        to: String,
        amount: u64,
    }

    pub struct TokenTransfer {
        transfer: Transfer,
        tag: TypeTag,
    }

    pub struct RegisterToken {
        coin_type: TypeTag,
    }

    pub enum OpsDetails {
        RegisterToken(RegisterToken),
        AccountCreation(AccountCreation),
        Transfer(Transfer),
        TokenTransfer(TokenTransfer),
        NftOps(NftOperation),
    }

    fn setup_transaction(
        sender: &str,
        keypair_str: &str,
        transaction_type: &str,
        sequence_number: u64,
        chain_id: u8,
        max_gas_amount: u64,
        expected_raw_txn_bytes_str: &str,
        expected_signature_str: &str,
        expected_encoded_txn_str: &str,
        json_literal: &str,
        ops_details: Option<OpsDetails>,
    ) {
        let account_address = AccountAddress::from_str(sender).unwrap();
        let keypair = tw_keypair::ed25519::sha512::KeyPair::try_from(keypair_str).unwrap();
        let factory = TransactionFactory::new(chain_id)
            .with_max_gas_amount(max_gas_amount)
            .with_gas_unit_price(100)
            .with_transaction_expiration_time(3664390082);

        let mut builder = match transaction_type {
            "transfer" => {
                if let OpsDetails::Transfer(transfer) = ops_details.unwrap() {
                    factory.implicitly_create_user_account_and_transfer(AccountAddress::from_str(&transfer.to).unwrap(), transfer.amount)
                } else {
                    panic!("Unsupported arguments")
                }
            },
            "create_account" => {
                if let OpsDetails::AccountCreation(account) = ops_details.unwrap() {
                    factory.create_user_account(AccountAddress::from_str(&account.to).unwrap())
                } else {
                    panic!("Unsupported arguments")
                }

            }
            "coin_transfer" => {
                if let OpsDetails::TokenTransfer(token_transfer) = ops_details.unwrap() {
                    factory.coins_transfer(AccountAddress::from_str(&token_transfer.transfer.to).unwrap(), token_transfer.transfer.amount, token_transfer.tag)
                } else {
                    panic!("Unsupported arguments")
                }
            }
            "nft_ops" => {
                if let OpsDetails::NftOps(nft) = ops_details.unwrap() {
                    factory.nft_ops(nft)
                } else {
                    panic!("Unsupported arguments")
                }
            }
            "register_token" => {
                if let OpsDetails::RegisterToken(register_token) = ops_details.unwrap() {
                    factory.register_token(register_token.coin_type)
                } else {
                    panic!("Unsupported arguments")
                }
            }
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
            33,
            3296766,
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
            Some(OpsDetails::Transfer(Transfer {
                to: "0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30".to_string(),
                amount: 1000,
            })),
        );
    }

    #[test]
    fn test_aptos_create_account() {
        setup_transaction(
            "0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30", // Sender's address
            "5d996aa76b3212142792d9130796cd2e11e3c445a93118c08414df4f66bc60ec", // Keypair
            "create_account",
            0, // Sequence number
            33,
            3296766,
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
            Some(OpsDetails::AccountCreation(AccountCreation {
                to: "0x3aa1672641a4e17b3d913b4c0301e805755a80b12756fc729c5878f12344d30e".to_string()
            })),
        );
    }

    #[test]
    fn test_aptos_coin_transfer() {
        setup_transaction(
            "0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30", // Sender's address
            "5d996aa76b3212142792d9130796cd2e11e3c445a93118c08414df4f66bc60ec", // Keypair
            "coin_transfer",
            24, // Sequence number
            32,
            3296766,
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
            Some(OpsDetails::TokenTransfer(TokenTransfer { transfer: Transfer { to: "0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30".to_string(), amount: 100000 }, tag: TypeTag::from_str("0x43417434fd869edee76cca2a4d2301e528a1551b1d719b75c350c3c97d15b8b9::coins::BTC").unwrap() })),
        );
    }

    #[test]
    fn test_nft_offer() {
        setup_transaction(
            "0x783135e8b00430253a22ba041d860c373d7a1501ccf7ac2d1ad37a8ed2775aee", // Sender's address
            "7bebb6d543d17f6fe4e685cfab239fa37896edd594ff859f1df32f244fb707e2", // Keypair
            "nft_ops",
            1, // Sequence number
            2,
            3296766,
            "783135e8b00430253a22ba041d860c373d7a1501ccf7ac2d1ad37a8ed2775aee01000000000000000200000000000000000000000000000000000000000000000000000000000000030f746f6b656e5f7472616e73666572730c6f666665725f73637269707400062007968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30209125e4054d884fdc7296b66e12c0d63a7baa0d88c77e8e784987c0a967c670ac0f0e546f70617a2054726f6f706572731514546f70617a2054726f6f70657220233230303638080000000000000000080100000000000000fe4d3200000000006400000000000000c2276ada0000000002", // Expected raw transaction bytes
            "af5c7357a83c69e3f425beb23eaf232f8bb36dea3b7cad4a7ab8d735cee999c8ec5285005adf69dc85a6c34b042dd0308fe92b76dad5d6ac88c7b9259902c10f", // Expected signature
            "783135e8b00430253a22ba041d860c373d7a1501ccf7ac2d1ad37a8ed2775aee01000000000000000200000000000000000000000000000000000000000000000000000000000000030f746f6b656e5f7472616e73666572730c6f666665725f73637269707400062007968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30209125e4054d884fdc7296b66e12c0d63a7baa0d88c77e8e784987c0a967c670ac0f0e546f70617a2054726f6f706572731514546f70617a2054726f6f70657220233230303638080000000000000000080100000000000000fe4d3200000000006400000000000000c2276ada00000000020020d1d99b67e37b483161a0fa369c46f34a3be4863c20e20fc7cdc669c0826a411340af5c7357a83c69e3f425beb23eaf232f8bb36dea3b7cad4a7ab8d735cee999c8ec5285005adf69dc85a6c34b042dd0308fe92b76dad5d6ac88c7b9259902c10f", // Expected encoded transaction
            r#"{
                    "expiration_timestamp_secs": "3664390082",
                    "gas_unit_price": "100",
                    "max_gas_amount": "3296766",
                    "payload": {
                        "arguments": [
                                      "0x7968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30",
                                      "0x9125e4054d884fdc7296b66e12c0d63a7baa0d88c77e8e784987c0a967c670ac",
                                      "Topaz Troopers", "Topaz Trooper #20068", "0", "1"],
                        "function": "0x3::token_transfers::offer_script",
                        "type": "entry_function_payload",
                        "type_arguments": []
                    },
                    "sender": "0x783135e8b00430253a22ba041d860c373d7a1501ccf7ac2d1ad37a8ed2775aee",
                    "sequence_number": "1",
                    "signature": {
                        "public_key": "0xd1d99b67e37b483161a0fa369c46f34a3be4863c20e20fc7cdc669c0826a4113",
                        "signature": "0xaf5c7357a83c69e3f425beb23eaf232f8bb36dea3b7cad4a7ab8d735cee999c8ec5285005adf69dc85a6c34b042dd0308fe92b76dad5d6ac88c7b9259902c10f",
                        "type": "ed25519_signature"
                    }
                }"#, // Expected JSON literal
            Some(OpsDetails::NftOps(NftOperation::Offer(Offer {
                receiver: AccountAddress::from_str("0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30").unwrap(),
                creator: AccountAddress::from_str("0x9125e4054d884fdc7296b66e12c0d63a7baa0d88c77e8e784987c0a967c670ac").unwrap(),
                collection: "Topaz Troopers".as_bytes().to_vec(),
                name: "Topaz Trooper #20068".as_bytes().to_vec(),
                property_version: 0,
                amount: 1,
            }))),
        )
    }

    #[test]
    fn test_cancel_nft_offer() {
        setup_transaction(
            "0x7968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30", // Sender's address
            "5d996aa76b3212142792d9130796cd2e11e3c445a93118c08414df4f66bc60ec", // Keypair
            "nft_ops",
            21, // Sequence number
            2,
            3296766,
            "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3015000000000000000200000000000000000000000000000000000000000000000000000000000000030f746f6b656e5f7472616e73666572731363616e63656c5f6f666665725f736372697074000520783135e8b00430253a22ba041d860c373d7a1501ccf7ac2d1ad37a8ed2775aee209125e4054d884fdc7296b66e12c0d63a7baa0d88c77e8e784987c0a967c670ac0f0e546f70617a2054726f6f706572731514546f70617a2054726f6f70657220233230303638080000000000000000fe4d3200000000006400000000000000c2276ada0000000002", // Expected raw transaction bytes
            "826722d374e276f618123e77da3ac024c89a3f97db9e09e19aa8ed06c3cdfc57d4a21c7890137f9a7c0447cc303447ba10ca5b1908e889071e0a68f48c0f260a", // Expected signature
            "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3015000000000000000200000000000000000000000000000000000000000000000000000000000000030f746f6b656e5f7472616e73666572731363616e63656c5f6f666665725f736372697074000520783135e8b00430253a22ba041d860c373d7a1501ccf7ac2d1ad37a8ed2775aee209125e4054d884fdc7296b66e12c0d63a7baa0d88c77e8e784987c0a967c670ac0f0e546f70617a2054726f6f706572731514546f70617a2054726f6f70657220233230303638080000000000000000fe4d3200000000006400000000000000c2276ada00000000020020ea526ba1710343d953461ff68641f1b7df5f23b9042ffa2d2a798d3adb3f3d6c40826722d374e276f618123e77da3ac024c89a3f97db9e09e19aa8ed06c3cdfc57d4a21c7890137f9a7c0447cc303447ba10ca5b1908e889071e0a68f48c0f260a", // Expected encoded transaction
            r#"{
                    "expiration_timestamp_secs": "3664390082",
                    "gas_unit_price": "100",
                    "max_gas_amount": "3296766",
                    "payload": {
                        "arguments": [
                                      "0x783135e8b00430253a22ba041d860c373d7a1501ccf7ac2d1ad37a8ed2775aee",
                                      "0x9125e4054d884fdc7296b66e12c0d63a7baa0d88c77e8e784987c0a967c670ac",
                                      "Topaz Troopers", "Topaz Trooper #20068", "0"],
                        "function": "0x3::token_transfers::cancel_offer_script",
                        "type": "entry_function_payload",
                        "type_arguments": []
                    },
                    "sender": "0x7968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30",
                    "sequence_number": "21",
                    "signature": {
                        "public_key": "0xea526ba1710343d953461ff68641f1b7df5f23b9042ffa2d2a798d3adb3f3d6c",
                        "signature": "0x826722d374e276f618123e77da3ac024c89a3f97db9e09e19aa8ed06c3cdfc57d4a21c7890137f9a7c0447cc303447ba10ca5b1908e889071e0a68f48c0f260a",
                        "type": "ed25519_signature"
                    }
                }"#, // Expected JSON literal
            Some(OpsDetails::NftOps(NftOperation::Cancel(Offer {
                receiver: AccountAddress::from_str("0x783135e8b00430253a22ba041d860c373d7a1501ccf7ac2d1ad37a8ed2775aee").unwrap(),
                creator: AccountAddress::from_str("0x9125e4054d884fdc7296b66e12c0d63a7baa0d88c77e8e784987c0a967c670ac").unwrap(),
                collection: "Topaz Troopers".as_bytes().to_vec(),
                name: "Topaz Trooper #20068".as_bytes().to_vec(),
                property_version: 0,
                amount: 0,
            }))),
        )
    }

    #[test]
    fn test_claim_nft_offer() {
        setup_transaction(
            "0x7968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30", // Sender's address
            "5d996aa76b3212142792d9130796cd2e11e3c445a93118c08414df4f66bc60ec", // Keypair
            "nft_ops",
            19, // Sequence number
            2,
            3296766,
            "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3013000000000000000200000000000000000000000000000000000000000000000000000000000000030f746f6b656e5f7472616e73666572730c636c61696d5f736372697074000520783135e8b00430253a22ba041d860c373d7a1501ccf7ac2d1ad37a8ed2775aee209125e4054d884fdc7296b66e12c0d63a7baa0d88c77e8e784987c0a967c670ac0f0e546f70617a2054726f6f706572731514546f70617a2054726f6f70657220233230303638080000000000000000fe4d3200000000006400000000000000c2276ada0000000002", // Expected raw transaction bytes
            "ede1ffb5f8f663741c2ca9597af44af81c98f7a910261bb4125f758fd0c0ebbf5bacb34f1196ad45153177729eb6d478676b364ab747da17602713f65ca2dd0a", // Expected signature
            "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3013000000000000000200000000000000000000000000000000000000000000000000000000000000030f746f6b656e5f7472616e73666572730c636c61696d5f736372697074000520783135e8b00430253a22ba041d860c373d7a1501ccf7ac2d1ad37a8ed2775aee209125e4054d884fdc7296b66e12c0d63a7baa0d88c77e8e784987c0a967c670ac0f0e546f70617a2054726f6f706572731514546f70617a2054726f6f70657220233230303638080000000000000000fe4d3200000000006400000000000000c2276ada00000000020020ea526ba1710343d953461ff68641f1b7df5f23b9042ffa2d2a798d3adb3f3d6c40ede1ffb5f8f663741c2ca9597af44af81c98f7a910261bb4125f758fd0c0ebbf5bacb34f1196ad45153177729eb6d478676b364ab747da17602713f65ca2dd0a", // Expected encoded transaction
            r#"{
                    "expiration_timestamp_secs": "3664390082",
                    "gas_unit_price": "100",
                    "max_gas_amount": "3296766",
                    "payload": {
                        "arguments": [
                                      "0x783135e8b00430253a22ba041d860c373d7a1501ccf7ac2d1ad37a8ed2775aee",
                                      "0x9125e4054d884fdc7296b66e12c0d63a7baa0d88c77e8e784987c0a967c670ac",
                                      "Topaz Troopers", "Topaz Trooper #20068", "0"],
                        "function": "0x3::token_transfers::claim_script",
                        "type": "entry_function_payload",
                        "type_arguments": []
                    },
                    "sender": "0x7968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30",
                    "sequence_number": "19",
                    "signature": {
                        "public_key": "0xea526ba1710343d953461ff68641f1b7df5f23b9042ffa2d2a798d3adb3f3d6c",
                        "signature": "0xede1ffb5f8f663741c2ca9597af44af81c98f7a910261bb4125f758fd0c0ebbf5bacb34f1196ad45153177729eb6d478676b364ab747da17602713f65ca2dd0a",
                        "type": "ed25519_signature"
                    }
                }"#, // Expected JSON literal
            Some(OpsDetails::NftOps(NftOperation::Claim(Claim {
                sender: AccountAddress::from_str("0x783135e8b00430253a22ba041d860c373d7a1501ccf7ac2d1ad37a8ed2775aee").unwrap(),
                creator: AccountAddress::from_str("0x9125e4054d884fdc7296b66e12c0d63a7baa0d88c77e8e784987c0a967c670ac").unwrap(),
                collection: "Topaz Troopers".as_bytes().to_vec(),
                name: "Topaz Trooper #20068".as_bytes().to_vec(),
                property_version: 0,
            }))),
        )
    }

    #[test]
    fn test_register_token() {
        setup_transaction(
            "0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30", // Sender's address
            "5d996aa76b3212142792d9130796cd2e11e3c445a93118c08414df4f66bc60ec", // Keypair
            "register_token",
            23, // Sequence number
            2,
            2000000,
            "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3017000000000000000200000000000000000000000000000000000000000000000000000000000000010c6d616e616765645f636f696e0872656769737465720107e4497a32bf4a9fd5601b27661aa0b933a923191bf403bd08669ab2468d43b379096d6f76655f636f696e084d6f7665436f696e000080841e00000000006400000000000000c2276ada0000000002", // Expected raw transaction bytes
            "e230b49f552fb85356dbec9df13f0dc56228eb7a9c29a8af3a99f4ae95b86c72bdcaa4ff1e9beb0bd81c298b967b9d97449856ec8bc672a08e2efef345c37100", // Expected signature
            "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3017000000000000000200000000000000000000000000000000000000000000000000000000000000010c6d616e616765645f636f696e0872656769737465720107e4497a32bf4a9fd5601b27661aa0b933a923191bf403bd08669ab2468d43b379096d6f76655f636f696e084d6f7665436f696e000080841e00000000006400000000000000c2276ada00000000020020ea526ba1710343d953461ff68641f1b7df5f23b9042ffa2d2a798d3adb3f3d6c40e230b49f552fb85356dbec9df13f0dc56228eb7a9c29a8af3a99f4ae95b86c72bdcaa4ff1e9beb0bd81c298b967b9d97449856ec8bc672a08e2efef345c37100", // Expected encoded transaction
            r#"{
                    "expiration_timestamp_secs": "3664390082",
                    "gas_unit_price": "100",
                    "max_gas_amount": "2000000",
                    "payload": {
                        "arguments": [],
                        "function": "0x1::managed_coin::register",
                        "type": "entry_function_payload",
                        "type_arguments": ["0xe4497a32bf4a9fd5601b27661aa0b933a923191bf403bd08669ab2468d43b379::move_coin::MoveCoin"]
                    },
                    "sender": "0x7968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30",
                    "sequence_number": "23",
                    "signature": {
                        "public_key": "0xea526ba1710343d953461ff68641f1b7df5f23b9042ffa2d2a798d3adb3f3d6c",
                        "signature": "0xe230b49f552fb85356dbec9df13f0dc56228eb7a9c29a8af3a99f4ae95b86c72bdcaa4ff1e9beb0bd81c298b967b9d97449856ec8bc672a08e2efef345c37100",
                        "type": "ed25519_signature"
                    }
                }"#, // Expected JSON literal
            Some(OpsDetails::RegisterToken(RegisterToken { coin_type: TypeTag::from_str("0xe4497a32bf4a9fd5601b27661aa0b933a923191bf403bd08669ab2468d43b379::move_coin::MoveCoin").unwrap() })),
        )
    }
}
