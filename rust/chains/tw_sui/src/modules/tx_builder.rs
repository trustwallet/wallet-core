// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::SuiAddress;
use crate::transaction::sui_types::{ObjectDigest, ObjectID, ObjectRef, SequenceNumber};
use crate::transaction::transaction_builder::TransactionBuilder;
use crate::transaction::transaction_data::TransactionData;
use std::borrow::Cow;
use std::str::FromStr;
use tw_coin_entry::error::{SigningError, SigningErrorType, SigningResult};
use tw_encoding::base64;
use tw_keypair::ed25519;
use tw_keypair::traits::KeyPairTrait;
use tw_memory::Data;
use tw_proto::Sui::Proto;
use tw_proto::Sui::Proto::mod_SigningInput::OneOftransaction_payload as TransactionType;

pub enum TWTransaction {
    Transaction(TransactionData),
    SignDirect(Data),
}

pub struct TWTransactionBuilder<'a> {
    input: Proto::SigningInput<'a>,
}

impl<'a> TWTransactionBuilder<'a> {
    pub fn new(input: Proto::SigningInput<'a>) -> Self {
        TWTransactionBuilder { input }
    }

    pub fn signer_key(&self) -> SigningResult<ed25519::sha512::KeyPair> {
        ed25519::sha512::KeyPair::try_from(self.input.private_key.as_ref())
            .map_err(SigningError::from)
    }

    pub fn build(self) -> SigningResult<TWTransaction> {
        let tx_data = match self.input.transaction_payload {
            TransactionType::sign_direct_message(ref direct) => {
                let raw_data = self.sign_direct_from_proto(direct)?;
                return Ok(TWTransaction::SignDirect(raw_data));
            },
            TransactionType::pay_sui(ref pay_sui) => self.pay_sui_from_proto(pay_sui),
            TransactionType::pay_all_sui(ref pay_all_sui) => {
                self.pay_all_sui_from_proto(pay_all_sui)
            },
            TransactionType::pay(ref pay) => self.pay_from_proto(pay),
            TransactionType::request_add_stake(ref stake) => self.stake_from_proto(stake),
            TransactionType::request_withdraw_stake(ref withdraw) => {
                self.withdraw_from_proto(withdraw)
            },
            TransactionType::transfer_object(ref transfer_obj) => {
                self.transfer_object_from_proto(transfer_obj)
            },
            TransactionType::None => Err(SigningError(SigningErrorType::Error_invalid_params)),
        }?;
        Ok(TWTransaction::Transaction(tx_data))
    }

    fn sign_direct_from_proto(&self, sign_direct: &Proto::SignDirect<'_>) -> SigningResult<Data> {
        let url = false;
        base64::decode(&sign_direct.unsigned_tx_msg, url)
            .map_err(|_| SigningError(SigningErrorType::Error_input_parse))
    }

    fn pay_sui_from_proto(&self, pay_sui: &Proto::PaySui<'_>) -> SigningResult<TransactionData> {
        let signer = self.signer_address()?;
        let input_coins = Self::build_coins(&pay_sui.input_coins)?;
        let recipients = Self::parse_addresses(&pay_sui.recipients)?;

        TransactionBuilder::pay_sui(
            signer,
            input_coins,
            recipients,
            pay_sui.amounts.clone(),
            self.input.gas_budget,
            self.input.reference_gas_price,
        )
    }

    fn pay_all_sui_from_proto(
        &self,
        pay_all_sui: &Proto::PayAllSui<'_>,
    ) -> SigningResult<TransactionData> {
        let signer = self.signer_address()?;
        let input_coins = Self::build_coins(&pay_all_sui.input_coins)?;
        let recipient = SuiAddress::from_str(&pay_all_sui.recipient)?;

        TransactionBuilder::pay_all_sui(
            signer,
            input_coins,
            recipient,
            self.input.gas_budget,
            self.input.reference_gas_price,
        )
    }

    fn pay_from_proto(&self, pay: &Proto::Pay<'_>) -> SigningResult<TransactionData> {
        let signer = self.signer_address()?;
        let input_coins = Self::build_coins(&pay.input_coins)?;
        let recipients = Self::parse_addresses(&pay.recipients)?;
        let gas = Self::require_coin(&pay.gas)?;

        TransactionBuilder::pay(
            signer,
            input_coins,
            recipients,
            pay.amounts.clone(),
            gas,
            self.input.gas_budget,
            self.input.reference_gas_price,
        )
    }

    fn stake_from_proto(
        &self,
        stake: &Proto::RequestAddStake<'_>,
    ) -> SigningResult<TransactionData> {
        let signer = self.signer_address()?;

        let input_coins = Self::build_coins(&stake.coins)?;
        let amount = stake.amount.as_ref().map(|a| a.amount);
        let validator = SuiAddress::from_str(stake.validator.as_ref())?;
        let gas = Self::require_coin(&stake.gas)?;

        TransactionBuilder::request_add_stake(
            signer,
            input_coins,
            amount,
            validator,
            gas,
            self.input.gas_budget,
            self.input.reference_gas_price,
        )
    }

    fn withdraw_from_proto(
        &self,
        withdraw: &Proto::RequestWithdrawStake<'_>,
    ) -> SigningResult<TransactionData> {
        let signer = self.signer_address()?;

        let staked_sui = Self::require_coin(&withdraw.staked_sui)?;
        let gas = Self::require_coin(&withdraw.gas)?;

        TransactionBuilder::request_withdraw_stake(
            signer,
            staked_sui,
            gas,
            self.input.gas_budget,
            self.input.reference_gas_price,
        )
    }

    fn transfer_object_from_proto(
        &self,
        transfer_obj: &Proto::TransferObject<'_>,
    ) -> SigningResult<TransactionData> {
        let signer = self.signer_address()?;

        let recipient = SuiAddress::from_str(&transfer_obj.recipient)?;
        let object = Self::require_coin(&transfer_obj.object)?;
        let gas = Self::require_coin(&transfer_obj.gas)?;

        TransactionBuilder::transfer_object(
            signer,
            object,
            recipient,
            gas,
            self.input.gas_budget,
            self.input.reference_gas_price,
        )
    }

    fn signer_address(&self) -> SigningResult<SuiAddress> {
        if self.input.private_key.is_empty() {
            SuiAddress::from_str(&self.input.signer).map_err(SigningError::from)
        } else {
            let keypair = self.signer_key()?;
            SuiAddress::with_ed25519_pubkey(keypair.public()).map_err(SigningError::from)
        }
    }

    fn build_coins(coins: &[Proto::ObjectRef]) -> SigningResult<Vec<ObjectRef>> {
        coins.iter().map(Self::build_coin).collect()
    }

    fn require_coin(maybe_coin: &Option<Proto::ObjectRef>) -> SigningResult<ObjectRef> {
        let coin = maybe_coin
            .as_ref()
            .ok_or(SigningError(SigningErrorType::Error_invalid_params))?;
        Self::build_coin(coin)
    }

    fn build_coin(coin: &Proto::ObjectRef) -> SigningResult<ObjectRef> {
        let object_id = ObjectID::from_str(coin.object_id.as_ref())?;
        let version = SequenceNumber(coin.version);
        let object_digest = ObjectDigest::from_str(coin.object_digest.as_ref())?;

        Ok((object_id, version, object_digest))
    }

    fn parse_addresses(addresses: &[Cow<'_, str>]) -> SigningResult<Vec<SuiAddress>> {
        let mut res = Vec::with_capacity(addresses.len());
        for addr in addresses {
            res.push(SuiAddress::from_str(addr.as_ref())?);
        }
        Ok(res)
    }
}
