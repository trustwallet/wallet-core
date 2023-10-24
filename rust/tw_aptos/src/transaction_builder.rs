// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use move_core_types::account_address::AccountAddress;
use tw_keypair::traits::KeyPairTrait;
use crate::constants::{GAS_UNIT_PRICE, MAX_GAS_AMOUNT};
use crate::transaction::RawTransaction;
use crate::transaction_payload::{EntryFunction, TransactionPayload};
use tw_proto::Aptos::Proto;
use tw_proto::Aptos::Proto::mod_SigningInput::OneOftransaction_payload;
use tw_proto::Aptos::Proto::SigningInput;
use crate::address::Address;
use crate::aptos_move_packages::aptos_account_transfer;

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
        let keypair = tw_keypair::ed25519::sha512::KeyPair::try_from(input.private_key.to_vec().as_slice()).unwrap();
        match input.transaction_payload {
            OneOftransaction_payload::transfer(transfer) => {
                return factory.implicitly_create_user_account_and_transfer(keypair.public(), transfer.amount)
            }
            OneOftransaction_payload::token_transfer(_) => {}
            OneOftransaction_payload::create_account(_) => {}
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

    pub fn create_user_account(&self, public_key: &tw_keypair::ed25519::sha512::PublicKey) -> TransactionBuilder {
        todo!()
    }

    pub fn implicitly_create_user_account_and_transfer(
        &self,
        public_key: &tw_keypair::ed25519::sha512::PublicKey,
        amount: u64,
    ) -> TransactionBuilder {
        self.payload(aptos_account_transfer(Address::with_ed25519_pubkey(public_key).unwrap().inner(), amount))
    }

    pub fn transfer(&self, to: AccountAddress, amount: u64) -> TransactionBuilder {
        todo!()
    }

    pub fn account_transfer(&self, to: AccountAddress, amount: u64) -> TransactionBuilder {
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
        std::time::SystemTime::now()
            .duration_since(std::time::UNIX_EPOCH)
            .unwrap()
            .as_secs()
            + self.transaction_expiration_time
    }
}
