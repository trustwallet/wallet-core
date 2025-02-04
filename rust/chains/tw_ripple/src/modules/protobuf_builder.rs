// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::RippleAddress;
use crate::transaction::transaction_builder::TransactionBuilder;
use crate::transaction::transaction_type::TransactionType;
use crate::types::account_id::AccountId;
use crate::types::amount::issued_currency::IssuedCurrency;
use crate::types::amount::native_amount::NativeAmount;
use crate::types::amount::Amount;
use crate::types::currency::Currency;
use bigdecimal::BigDecimal;
use std::str::FromStr;
use tw_coin_entry::error::prelude::*;
use tw_hash::H256;
use tw_keypair::ecdsa::secp256k1;
use tw_misc::traits::{OptionalEmpty, OptionalInt};
use tw_proto::Ripple::Proto;
use tw_proto::Ripple::Proto::mod_SigningInput::OneOfoperation_oneof as OperationType;

pub struct ProtobufBuilder<'a> {
    input: &'a Proto::SigningInput<'a>,
}

impl<'a> ProtobufBuilder<'a> {
    pub fn new(input: &'a Proto::SigningInput<'a>) -> Self {
        ProtobufBuilder { input }
    }

    pub fn build_tx(self) -> SigningResult<TransactionType> {
        match self.input.operation_oneof {
            OperationType::op_payment(ref payment) => self.payment(payment),
            OperationType::op_trust_set(ref trust_set) => self.trust_set(trust_set),
            OperationType::op_escrow_create(ref escrow_create) => self.escrow_create(escrow_create),
            _ => todo!(),
        }
    }

    pub fn payment(&self, payment: &Proto::OperationPayment) -> SigningResult<TransactionType> {
        use tw_proto::Ripple::Proto::mod_OperationPayment::OneOfamount_oneof as AmountType;

        let amount = match payment.amount_oneof {
            AmountType::amount(native) => Amount::NativeAmount(NativeAmount(native)),
            AmountType::currency_amount(ref currency) => {
                Amount::IssuedCurrency(Self::issued_currency(currency)?)
            },
            AmountType::None => {
                return SigningError::err(SigningErrorType::Error_invalid_params)
                    .context("'OperationPayment.amount' isn't specified");
            },
        };
        let destination = RippleAddress::from_str(payment.destination.as_ref())
            .into_tw()
            .context("Invalid 'Payment.destination' address")?;
        let destination_tag = payment.destination_tag.zero_or_some();
        self.prepare_builder()?
            .payment(amount, destination, destination_tag)
            .map(TransactionType::Payment)
    }

    pub fn trust_set(
        &self,
        trust_set: &Proto::OperationTrustSet,
    ) -> SigningResult<TransactionType> {
        let limit_amount_proto = trust_set
            .limit_amount
            .as_ref()
            .or_tw_err(SigningErrorType::Error_invalid_requested_token_amount)
            .context("No 'OperationTrustSet.limitAmount' provided")?;

        let limit_amount = Self::issued_currency(limit_amount_proto)?;
        self.prepare_builder()?
            .trust_set(limit_amount)
            .map(TransactionType::TrustSet)
    }

    pub fn escrow_create(
        &self,
        escrow_create: &Proto::OperationEscrowCreate,
    ) -> SigningResult<TransactionType> {
        let destination = RippleAddress::from_str(escrow_create.destination.as_ref())
            .into_tw()
            .context("Invalid 'EscrowCreate.destination' address")?;
        let condition = escrow_create
            .condition
            .to_string()
            .empty_or_some()
            .map(|condition| H256::from_str(condition.as_str()))
            .transpose()
            .tw_err(SigningErrorType::Error_invalid_params)
            .context("Invalid 'OperationEscrowCreate.condition'")?;
        self.prepare_builder()?
            .escrow_create(
                NativeAmount(escrow_create.amount),
                destination,
                escrow_create.destination_tag.zero_or_some(),
                escrow_create.cancel_after.zero_or_some(),
                escrow_create.finish_after.zero_or_some(),
                condition,
            )
            .map(TransactionType::EscrowCreate)
    }

    pub fn prepare_builder(&self) -> SigningResult<TransactionBuilder> {
        let signing_public_key = if !self.input.private_key.is_empty() {
            secp256k1::PrivateKey::try_from(self.input.private_key.as_ref())
                .into_tw()
                .context("Invalid private key")?
                .public()
        } else if !self.input.public_key.is_empty() {
            secp256k1::PublicKey::try_from(self.input.public_key.as_ref())
                .into_tw()
                .context("Invalid public key")?
        } else {
            return SigningError::err(SigningErrorType::Error_invalid_params)
                .context("Expected either 'privateKey' or 'publicKey' to be provided");
        };

        let mut builder = TransactionBuilder::default();
        builder
            .fee(NativeAmount(self.input.fee))
            .flags(self.input.flags)
            .sequence(self.input.sequence)
            .last_ledger_sequence(self.input.last_ledger_sequence)
            .account_str(self.input.account.as_ref())?
            .signing_pub_key(&signing_public_key);
        Ok(builder)
    }

    fn issued_currency(input: &Proto::CurrencyAmount) -> SigningResult<IssuedCurrency> {
        let value = BigDecimal::from_str(input.value.as_ref())
            .tw_err(SigningErrorType::Error_invalid_requested_token_amount)
            .context("Invalid 'CurrencyAmount.amount'")?;
        let currency = Currency::from_str(input.currency.as_ref())
            .context("Invalid 'CurrencyAmount.currency'")?;
        let issuer = AccountId::from_str(input.issuer.as_ref())
            .context("Invalid 'CurrencyAmount.accountId'")?;
        Ok(IssuedCurrency {
            value,
            currency,
            issuer,
        })
    }
}
