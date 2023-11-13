// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::aptos_move_packages::{
    aptos_account_create_account, aptos_account_transfer, aptos_account_transfer_coins,
    coin_transfer, managed_coin_register, token_transfers_cancel_offer_script,
    token_transfers_claim_script, token_transfers_offer_script,
};
use crate::constants::{GAS_UNIT_PRICE, MAX_GAS_AMOUNT};
use crate::liquid_staking::{
    tortuga_claim, tortuga_stake, tortuga_unstake, LiquidStakingOperation,
};
use crate::nft::NftOperation;
use crate::transaction::RawTransaction;
use crate::transaction_payload::{
    convert_proto_struct_tag_to_type_tag, EntryFunction, TransactionPayload,
};
use move_core_types::account_address::AccountAddress;
use move_core_types::language_storage::TypeTag;
use serde_json::Value;
use std::str::FromStr;
use tw_coin_entry::error::{SigningError, SigningErrorType, SigningResult};
use tw_proto::Aptos::Proto::mod_SigningInput::OneOftransaction_payload;
use tw_proto::Aptos::Proto::SigningInput;

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
    pub fn sender(mut self, sender: AccountAddress) -> Self {
        self.sender = Some(sender);
        self
    }

    pub fn sequence_number(mut self, sequence_number: u64) -> Self {
        self.sequence_number = Some(sequence_number);
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

    pub fn new_from_protobuf(input: SigningInput) -> SigningResult<TransactionBuilder> {
        let factory = TransactionFactory::new(input.chain_id as u8)
            .with_gas_unit_price(input.gas_unit_price)
            .with_max_gas_amount(input.max_gas_amount)
            .with_transaction_expiration_time(input.expiration_timestamp_secs);
        match input.transaction_payload {
            OneOftransaction_payload::transfer(transfer) => Ok(factory
                .implicitly_create_user_account_and_transfer(
                    AccountAddress::from_str(&transfer.to).unwrap(),
                    transfer.amount,
                )),
            OneOftransaction_payload::token_transfer(token_transfer) => {
                let func = token_transfer.function.unwrap();
                Ok(factory.coins_transfer(
                    AccountAddress::from_str(&token_transfer.to).unwrap(),
                    token_transfer.amount,
                    convert_proto_struct_tag_to_type_tag(func),
                ))
            },
            OneOftransaction_payload::create_account(create_account) => Ok(factory
                .create_user_account(AccountAddress::from_str(&create_account.auth_key).unwrap())),
            OneOftransaction_payload::nft_message(nft_message) => {
                Ok(factory.nft_ops(nft_message.into()))
            },
            OneOftransaction_payload::register_token(register_token) => Ok(factory.register_token(
                convert_proto_struct_tag_to_type_tag(register_token.function.unwrap()),
            )),
            OneOftransaction_payload::liquid_staking_message(msg) => {
                Ok(factory.liquid_staking_ops(msg.into()))
            },
            OneOftransaction_payload::token_transfer_coins(token_transfer_coins) => {
                let func = token_transfer_coins.function.unwrap();
                Ok(factory.implicitly_create_user_and_coins_transfer(
                    AccountAddress::from_str(&token_transfer_coins.to).unwrap(),
                    token_transfer_coins.amount,
                    convert_proto_struct_tag_to_type_tag(func),
                ))
            },
            OneOftransaction_payload::None => {
                let is_blind_sign = !input.any_encoded.is_empty();
                let v = serde_json::from_str::<Value>(&input.any_encoded)?;
                if is_blind_sign {
                    Ok(factory.payload(TransactionPayload::EntryFunction(
                        EntryFunction::try_from(v).unwrap(),
                    )))
                } else {
                    Err(SigningError(SigningErrorType::Error_input_parse))
                }
            },
        }
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

    pub fn payload(&self, payload: TransactionPayload) -> TransactionBuilder {
        self.transaction_builder(payload)
    }

    pub fn create_user_account(&self, to: AccountAddress) -> TransactionBuilder {
        self.payload(aptos_account_create_account(to))
    }

    pub fn register_token(&self, coin_type: TypeTag) -> TransactionBuilder {
        self.payload(managed_coin_register(coin_type))
    }

    pub fn nft_ops(&self, operation: NftOperation) -> TransactionBuilder {
        match operation {
            NftOperation::Claim(claim) => self.payload(token_transfers_claim_script(
                claim.sender,
                claim.creator,
                claim.collection,
                claim.name,
                claim.property_version,
            )),
            NftOperation::Cancel(offer) => self.payload(token_transfers_cancel_offer_script(
                offer.receiver,
                offer.creator,
                offer.collection,
                offer.name,
                offer.property_version,
            )),
            NftOperation::Offer(offer) => self.payload(token_transfers_offer_script(
                offer.receiver,
                offer.creator,
                offer.collection,
                offer.name,
                offer.property_version,
                offer.amount,
            )),
        }
    }

    pub fn liquid_staking_ops(&self, operation: LiquidStakingOperation) -> TransactionBuilder {
        match operation {
            LiquidStakingOperation::Stake(stake) => {
                self.payload(tortuga_stake(stake.smart_contract_address, stake.amount))
            },
            LiquidStakingOperation::Unstake(unstake) => self.payload(tortuga_unstake(
                unstake.smart_contract_address,
                unstake.amount,
            )),
            LiquidStakingOperation::Claim(claim) => {
                self.payload(tortuga_claim(claim.smart_contract_address, claim.idx))
            },
        }
    }

    pub fn implicitly_create_user_account_and_transfer(
        &self,
        to: AccountAddress,
        amount: u64,
    ) -> TransactionBuilder {
        self.payload(aptos_account_transfer(to, amount))
    }

    pub fn coins_transfer(
        &self,
        to: AccountAddress,
        amount: u64,
        coin_type: TypeTag,
    ) -> TransactionBuilder {
        self.payload(coin_transfer(coin_type, to, amount))
    }

    pub fn implicitly_create_user_and_coins_transfer(
        &self,
        to: AccountAddress,
        amount: u64,
        coin_type: TypeTag,
    ) -> TransactionBuilder {
        self.payload(aptos_account_transfer_coins(coin_type, to, amount))
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
