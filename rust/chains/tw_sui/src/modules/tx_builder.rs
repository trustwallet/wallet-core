// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::SuiAddress;
use crate::transaction::sui_types::{
    ChainIdentifier, ObjectDigest, ObjectID, ObjectRef, SequenceNumber, TransactionExpiration,
};
use crate::transaction::transaction_builder::{
    AddressBalanceWithdrawal, GasPayment, TransactionBuilder,
};
use crate::transaction::transaction_data::TransactionData;
use move_core_types::language_storage::TypeTag;
use move_core_types::parser::parse_type_tag;
use std::borrow::Cow;
use std::str::FromStr;
use tw_coin_entry::error::prelude::*;
use tw_encoding::base64::{self, STANDARD};
use tw_keypair::ed25519;
use tw_keypair::traits::KeyPairTrait;
use tw_memory::Data;
use tw_proto::Sui::Proto;
use tw_proto::Sui::Proto::mod_SigningInput::OneOftransaction_payload as TransactionType;

pub enum TWTransaction {
    Transaction(Box<TransactionData>),
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
                self.reject_valid_during("sign_direct_message")?;
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
            TransactionType::raw_json(ref raw_json) => self.raw_json_from_proto(raw_json),
            TransactionType::None => SigningError::err(SigningErrorType::Error_invalid_params),
        }?;
        Ok(TWTransaction::Transaction(Box::new(tx_data)))
    }

    fn sign_direct_from_proto(&self, sign_direct: &Proto::SignDirect<'_>) -> SigningResult<Data> {
        base64::decode(&sign_direct.unsigned_tx_msg, STANDARD)
            .tw_err(SigningErrorType::Error_input_parse)
            .context("Error parsing Raw Unsigned TX message as base64")
    }

    fn pay_sui_from_proto(&self, pay_sui: &Proto::PaySui<'_>) -> SigningResult<TransactionData> {
        let signer = self.signer_address()?;
        self.check_reservation(pay_sui.address_balance_amount)?;
        let input_coins = Self::build_coins(&pay_sui.input_coins)?;
        let recipients = Self::parse_addresses(&pay_sui.recipients)
            .context("Invalid one of the recipients addresses")?;

        TransactionBuilder::pay_sui_funds(
            signer,
            input_coins,
            pay_sui.address_balance_amount,
            recipients,
            pay_sui.amounts.clone(),
            self.input.gas_budget,
            self.input.reference_gas_price,
            self.expiration()?,
        )
    }

    fn pay_all_sui_from_proto(
        &self,
        pay_all_sui: &Proto::PayAllSui<'_>,
    ) -> SigningResult<TransactionData> {
        let signer = self.signer_address()?;
        self.check_reservation(pay_all_sui.address_balance_amount)?;
        let input_coins = Self::build_coins(&pay_all_sui.input_coins)?;
        let recipient = SuiAddress::from_str(&pay_all_sui.recipient)
            .into_tw()
            .context("Invalid recipient address")?;

        TransactionBuilder::pay_all_sui_funds(
            signer,
            input_coins,
            pay_all_sui.address_balance_amount,
            recipient,
            self.input.gas_budget,
            self.input.reference_gas_price,
            self.expiration()?,
        )
    }

    fn pay_from_proto(&self, pay: &Proto::Pay<'_>) -> SigningResult<TransactionData> {
        let signer = self.signer_address()?;
        self.check_reservation(pay.address_balance_amount)?;
        let input_coins = Self::build_coins(&pay.input_coins)?;
        let recipients = Self::parse_addresses(&pay.recipients)
            .context("Invalid one of the recipients addresses")?;
        let gas = self.gas_payment(&pay.gas)?;
        let withdrawal = Self::token_withdrawal(pay)?;

        TransactionBuilder::pay(
            signer,
            input_coins,
            withdrawal,
            recipients,
            pay.amounts.clone(),
            gas,
            self.input.gas_budget,
            self.input.reference_gas_price,
        )
    }

    /// SIP-58: `address_balance_amount` of `coin_type`. Both or neither, so that neither is silently ignored.
    fn token_withdrawal(pay: &Proto::Pay<'_>) -> SigningResult<Option<AddressBalanceWithdrawal>> {
        match (pay.address_balance_amount, pay.coin_type.is_empty()) {
            (0, true) => Ok(None),
            (0, false) => SigningError::err(SigningErrorType::Error_invalid_params)
                .context("'coin_type' is set but 'address_balance_amount' is 0"),
            (_, true) => SigningError::err(SigningErrorType::Error_invalid_params)
                .context("'coin_type' is required to withdraw from the address balance"),
            (amount, false) => {
                let coin_type = parse_type_tag(&pay.coin_type)
                    .map_err(|_| SigningError::new(SigningErrorType::Error_invalid_params))
                    .context("Invalid 'coin_type'")?;
                if !matches!(coin_type, TypeTag::Struct(_)) {
                    return SigningError::err(SigningErrorType::Error_invalid_params)
                        .context("'coin_type' must be a coin struct like `0x2::sui::SUI`");
                }
                Ok(Some(AddressBalanceWithdrawal { amount, coin_type }))
            },
        }
    }

    fn stake_from_proto(
        &self,
        stake: &Proto::RequestAddStake<'_>,
    ) -> SigningResult<TransactionData> {
        let signer = self.signer_address()?;
        self.check_reservation(stake.address_balance_amount)?;

        let input_coins = Self::build_coins(&stake.coins)?;
        let amount = stake.amount.as_ref().map(|a| a.amount);
        let validator = SuiAddress::from_str(stake.validator.as_ref())
            .into_tw()
            .context("Invalid validator address")?;
        let gas = self.gas_payment(&stake.gas)?;

        TransactionBuilder::request_add_stake(
            signer,
            input_coins,
            stake.address_balance_amount,
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

        let staked_sui =
            Self::require_coin(&withdraw.staked_sui).context("No 'staked_sui' coin specified")?;
        let gas = self.gas_payment(&withdraw.gas)?;

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

        let recipient = SuiAddress::from_str(&transfer_obj.recipient)
            .into_tw()
            .context("Invalid recipient address")?;
        let object = Self::require_coin(&transfer_obj.object).context("No 'object' specified")?;
        let gas = self.gas_payment(&transfer_obj.gas)?;

        TransactionBuilder::transfer_object(
            signer,
            object,
            recipient,
            gas,
            self.input.gas_budget,
            self.input.reference_gas_price,
        )
    }

    fn raw_json_from_proto(&self, raw_json: &str) -> SigningResult<TransactionData> {
        self.reject_valid_during("raw_json")?;
        TransactionBuilder::raw_json(
            raw_json,
            self.input.gas_budget,
            self.input.reference_gas_price,
        )
    }

    /// SIP-58: the `valid_during` window, which also means gas is paid from the address balance.
    fn expiration(&self) -> SigningResult<Option<TransactionExpiration>> {
        let Some(valid_during) = self.input.valid_during.as_ref() else {
            return Ok(None);
        };
        // The chain accepts the current epoch and the next one at most (SIP-58): `[N, N]` or `[N, N + 1]`.
        if valid_during.max_epoch < valid_during.min_epoch
            || valid_during.max_epoch - valid_during.min_epoch > 1
        {
            return SigningError::err(SigningErrorType::Error_invalid_params).context(
                "'valid_during' must be the current epoch or the current and the next one",
            );
        }
        if self.input.gas_budget == 0 {
            return SigningError::err(SigningErrorType::Error_invalid_params)
                .context("'gas_budget' must be set: it is withdrawn from the address balance");
        }
        Ok(Some(TransactionExpiration::ValidDuring {
            min_epoch: Some(valid_during.min_epoch),
            max_epoch: Some(valid_during.max_epoch),
            min_timestamp: None,
            max_timestamp: None,
            chain: ChainIdentifier::from_str(&valid_during.chain)?,
            nonce: valid_during.nonce,
        }))
    }

    /// With gas paid from the address balance the node reserves `withdraw + gas_budget` from it, and
    /// rejects the transaction when the balance is lower. The balance is unknown here, so only the
    /// arithmetic can be checked: the caller is responsible for `withdraw + gas_budget <= balance`.
    fn check_reservation(&self, withdraw: u64) -> SigningResult<()> {
        if self.input.valid_during.is_some()
            && withdraw.checked_add(self.input.gas_budget).is_none()
        {
            return SigningError::err(SigningErrorType::Error_invalid_params)
                .context("Address balance withdrawal plus gas budget overflows u64");
        }
        Ok(())
    }

    /// `valid_during` only has a meaning for transactions this builder assembles. Ignoring it for a
    /// prebuilt transaction would sign something other than what the caller asked for.
    fn reject_valid_during(&self, arm: &str) -> SigningResult<()> {
        if self.input.valid_during.is_some() {
            return SigningError::err(SigningErrorType::Error_invalid_params)
                .with_context(|| format!("'valid_during' is not supported with '{arm}'"));
        }
        Ok(())
    }

    /// A gas coin, or the address balance when `valid_during` is set. Exactly one of them.
    fn gas_payment(&self, gas: &Option<Proto::ObjectRef>) -> SigningResult<GasPayment> {
        match (self.expiration()?, gas) {
            (Some(expiration), None) => Ok(GasPayment::AddressBalance(expiration)),
            (Some(_), Some(_)) => SigningError::err(SigningErrorType::Error_invalid_params)
                .context(
                    "'gas' must be unset when 'valid_during' pays gas from the address balance",
                ),
            (None, gas) => Self::require_coin(gas)
                .context("No 'gas' coin specified")
                .map(GasPayment::Coin),
        }
    }

    fn signer_address(&self) -> SigningResult<SuiAddress> {
        if self.input.private_key.is_empty() {
            SuiAddress::from_str(&self.input.signer)
                .into_tw()
                .context("Invalid signer address")
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
            .or_tw_err(SigningErrorType::Error_invalid_params)?;
        Self::build_coin(coin)
    }

    fn build_coin(coin: &Proto::ObjectRef) -> SigningResult<ObjectRef> {
        let object_id = ObjectID::from_str(coin.object_id.as_ref()).context("Invalid Object ID")?;
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
