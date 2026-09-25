// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::classic_address::ClassicAddress;
use crate::address::RippleAddress;
use crate::transaction::common_fields::CommonFields;
use crate::transaction::json_transaction::JsonTransaction;
use crate::transaction::transaction_builder::TransactionBuilder;
use crate::transaction::transaction_type::TransactionType;
use crate::transaction::RippleTransaction;
use crate::types::account_id::AccountId;
use crate::types::amount::issued_currency::IssuedCurrency;
use crate::types::amount::native_amount::NativeAmount;
use crate::types::amount::Amount;
use crate::types::currency::Currency;
use bigdecimal::BigDecimal;
use serde::{Deserialize, Serialize};
use std::str::FromStr;
use tw_coin_entry::error::prelude::*;
use tw_encoding::hex::as_hex::AsHex;
use tw_encoding::hex::DecodeHex;
use tw_hash::H256;
use tw_keypair::ecdsa::secp256k1;
use tw_misc::traits::{OptionalEmpty, OptionalInt};
use tw_proto::Ripple::Proto;
use tw_proto::Ripple::Proto::mod_SigningInput::OneOfoperation_oneof as OperationType;

#[derive(Deserialize, Serialize)]
#[serde(untagged)]
#[allow(clippy::large_enum_variant)]
pub enum SigningRequest {
    Typed(TransactionType),
    RawJSON(JsonTransaction),
}

impl RippleTransaction for SigningRequest {
    fn common_types(&self) -> &CommonFields {
        match self {
            SigningRequest::Typed(ty) => ty.common_types(),
            SigningRequest::RawJSON(json) => json.common_types(),
        }
    }

    fn common_types_mut(&mut self) -> &mut CommonFields {
        match self {
            SigningRequest::Typed(ty) => ty.common_types_mut(),
            SigningRequest::RawJSON(json) => json.common_types_mut(),
        }
    }
}

pub struct ProtobufBuilder<'a> {
    input: &'a Proto::SigningInput<'a>,
}

impl<'a> ProtobufBuilder<'a> {
    pub fn new(input: &'a Proto::SigningInput<'a>) -> Self {
        ProtobufBuilder { input }
    }

    pub fn build(self) -> SigningResult<SigningRequest> {
        if self.input.raw_json.is_empty() {
            self.build_typed().map(SigningRequest::Typed)
        } else {
            self.build_tx_json().map(SigningRequest::RawJSON)
        }
    }

    /// Builds a transaction from `SigningInput.rawJson` JSON object,
    /// returns a [`JsonTransaction`] with deserialized [`CommonFields`].
    ///
    /// Note [`JsonTransaction`] implements [`RippleTransaction`]
    pub fn build_tx_json(self) -> SigningResult<JsonTransaction> {
        let mut tx: JsonTransaction = serde_json::from_str(self.input.raw_json.as_ref())
            .tw_err(SigningErrorType::Error_input_parse)
            .context("Invalid 'SigningInput.rawJson'")?;

        let expected_signing_pubkey = self.signing_public_key()?;

        // Check whether JSON transaction contains `SigningPubKey` field, otherwise set it.
        if tx.common_fields.signing_pub_key.is_none() {
            tx.common_fields.signing_pub_key = Some(AsHex(expected_signing_pubkey.compressed()));
        }

        // Check whether JSON transaction contains `Account` field, otherwise set it.
        if tx.common_fields.account.is_none() {
            let address = ClassicAddress::with_public_key(&expected_signing_pubkey)
                .into_tw()
                .context("Internal: error generating an address for the signing public key")?;
            tx.common_fields.account = Some(RippleAddress::Classic(address));
        }

        // Check whether `SigningInput.fee` is specified, or JSON transaction doesn't contain that field,
        // then override the field.
        if self.input.fee != 0 || tx.common_fields.fee.is_none() {
            tx.common_fields.fee = Some(self.fee()?);
        }

        // Check whether `SigningInput.sequence` is specified, or JSON transaction doesn't contain that field,
        // then override the field.
        if self.input.sequence != 0 || tx.common_fields.sequence.is_none() {
            tx.common_fields.sequence = Some(self.input.sequence);
        }

        // Check whether `SigningInput.last_ledger_sequence` is specified, or JSON transaction doesn't contain that field,
        // then override the field.
        if self.input.last_ledger_sequence != 0 || tx.common_fields.last_ledger_sequence.is_none() {
            tx.common_fields.last_ledger_sequence = Some(self.input.last_ledger_sequence);
        }

        Ok(tx)
    }

    pub fn build_typed(self) -> SigningResult<TransactionType> {
        match self.input.operation_oneof {
            OperationType::op_payment(ref payment) => self.payment(payment),
            OperationType::op_trust_set(ref trust_set) => self.trust_set(trust_set),
            OperationType::op_escrow_create(ref escrow_create) => self.escrow_create(escrow_create),
            OperationType::op_escrow_cancel(ref escrow_cancel) => self.escrow_cancel(escrow_cancel),
            OperationType::op_escrow_finish(ref escrow_finish) => self.escrow_finish(escrow_finish),
            OperationType::op_nftoken_burn(ref burn) => self.nftoken_burn(burn),
            OperationType::op_nftoken_create_offer(ref create) => self.nftoken_create_offer(create),
            OperationType::op_nftoken_accept_offer(ref accept) => self.nftoken_accept_offer(accept),
            OperationType::op_nftoken_cancel_offer(ref cancel) => self.nftoken_cancel_offer(cancel),
            OperationType::None => SigningError::err(SigningErrorType::Error_invalid_params)
                .context("`SigningInput.operation` is not provided"),
        }
    }

    pub fn payment(&self, payment: &Proto::OperationPayment) -> SigningResult<TransactionType> {
        use tw_proto::Ripple::Proto::mod_OperationPayment::OneOfamount_oneof as AmountType;

        let amount = match payment.amount_oneof {
            AmountType::amount(native) => {
                let native =
                    NativeAmount::new(native).context("Invalid 'OperationPayment.amount'")?;
                Amount::NativeAmount(native)
            },
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
            .context("Invalid 'Payment.destination' address")?
            .to_classic_address()
            .into_tw()
            .context("Error converting 'Payment.destination' to a Classic address")?;
        let destination_tag = payment
            .destination_tag
            .try_into_u32_optional("destinationTag")?;

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
            .context("Invalid 'EscrowCreate.destination' address")?
            .to_classic_address()
            .into_tw()
            .context("Error converting 'OperationEscrowCreate.destination' to a Classic address")?;

        let condition = escrow_create
            .condition
            .decode_hex()
            .tw_err(SigningErrorType::Error_invalid_params)
            .context("Invalid 'OperationEscrowCreate.condition'")?
            .empty_or_some();

        let native_amount = NativeAmount::new(escrow_create.amount)
            .context("Invalid 'OperationEscrowCreate.amount'")?;

        self.prepare_builder()?
            .escrow_create(
                native_amount,
                destination,
                escrow_create
                    .destination_tag
                    .try_into_u32_optional("destinationTag")?,
                escrow_create
                    .cancel_after
                    .try_into_u32_optional("cancelAfter")?,
                escrow_create
                    .finish_after
                    .try_into_u32_optional("finishAfter")?,
                condition,
            )
            .map(TransactionType::EscrowCreate)
    }

    pub fn escrow_cancel(
        &self,
        escrow_cancel: &Proto::OperationEscrowCancel,
    ) -> SigningResult<TransactionType> {
        let owner = ClassicAddress::from_str(escrow_cancel.owner.as_ref())
            .into_tw()
            .context("Invalid 'EscrowCancel.owner' address")?;
        self.prepare_builder()?
            .escrow_cancel(owner, escrow_cancel.offer_sequence)
            .map(TransactionType::EscrowCancel)
    }

    pub fn escrow_finish(
        &self,
        escrow_finish: &Proto::OperationEscrowFinish,
    ) -> SigningResult<TransactionType> {
        let owner = ClassicAddress::from_str(escrow_finish.owner.as_ref())
            .into_tw()
            .context("Invalid 'OperationEscrowFinish.owner' address")?;

        let condition = escrow_finish
            .condition
            .decode_hex()
            .tw_err(SigningErrorType::Error_invalid_params)
            .context("Invalid 'OperationEscrowFinish.condition'")?
            .empty_or_some();

        let fulfillment = escrow_finish
            .fulfillment
            .decode_hex()
            .tw_err(SigningErrorType::Error_invalid_params)
            .context("Invalid 'OperationEscrowFinish.fulfillment'")?
            .empty_or_some();

        self.prepare_builder()?
            .escrow_finish(owner, escrow_finish.offer_sequence, condition, fulfillment)
            .map(TransactionType::EscrowFinish)
    }

    pub fn nftoken_burn(
        &self,
        burn: &Proto::OperationNFTokenBurn,
    ) -> SigningResult<TransactionType> {
        let nftoken_id = H256::from_str(burn.nftoken_id.as_ref())
            .tw_err(SigningErrorType::Error_invalid_params)
            .context("Invalid 'OperationNFTokenBurn.nftokenId'")?;

        // Owner is the transaction signer.
        let owner = None;
        self.prepare_builder()?
            .nftoken_burn(nftoken_id, owner)
            .map(TransactionType::NFTokenBurn)
    }

    pub fn nftoken_create_offer(
        &self,
        create: &Proto::OperationNFTokenCreateOffer,
    ) -> SigningResult<TransactionType> {
        let nftoken_id = H256::from_str(create.nftoken_id.as_ref())
            .tw_err(SigningErrorType::Error_invalid_params)
            .context("Invalid 'OperationNFTokenCreateOffer.nftokenId'")?;
        let destination = ClassicAddress::from_str(create.destination.as_ref())
            .into_tw()
            .context("Invalid 'OperationNFTokenCreateOffer.destination'")?;

        // Currently, owner of the token can only give it away, gratis to the account identified by the `Destination` field.
        let amount = Amount::NativeAmount(NativeAmount::default());
        // Owner is the transaction signer.
        let owner = None;
        let expiration = None;

        self.prepare_builder()?
            .nftoken_create_offer(nftoken_id, amount, owner, expiration, Some(destination))
            .map(TransactionType::NFTokenCreateOffer)
    }

    pub fn nftoken_accept_offer(
        &self,
        accept: &Proto::OperationNFTokenAcceptOffer,
    ) -> SigningResult<TransactionType> {
        let nftoken_sell_offer = H256::from_str(accept.sell_offer.as_ref())
            .tw_err(SigningErrorType::Error_invalid_params)
            .context("Invalid 'OperationNFTokenAcceptOffer.sellOffer'")?;
        // Currently, only sell offer supported.
        let nftoken_buy_offer = None;

        self.prepare_builder()?
            .nftoken_accept_offer(Some(nftoken_sell_offer), nftoken_buy_offer)
            .map(TransactionType::NFTokenAcceptOffer)
    }

    pub fn nftoken_cancel_offer(
        &self,
        cancel: &Proto::OperationNFTokenCancelOffer,
    ) -> SigningResult<TransactionType> {
        let nftoken_offers = cancel
            .token_offers
            .iter()
            .map(|offer| H256::from_str(offer.as_ref()))
            .collect::<Result<Vec<_>, _>>()
            .tw_err(SigningErrorType::Error_invalid_params)
            .context("Invalid 'OperationNFTokenCancelOffer.tokenOffers'")?;

        self.prepare_builder()?
            .nftoken_cancel_offer(nftoken_offers)
            .map(TransactionType::NFTokenCancelOffer)
    }

    pub fn prepare_builder(&self) -> SigningResult<TransactionBuilder> {
        let signing_public_key = self.signing_public_key()?;
        let fee = self.fee()?;

        let mut builder = TransactionBuilder::default();
        builder
            .fee(fee)
            .flags(self.input.flags.try_into_u32("inputFlags")?)
            .sequence(self.input.sequence)
            .last_ledger_sequence(self.input.last_ledger_sequence)
            .account_str(self.input.account.as_ref())?
            .signing_pub_key(&signing_public_key);
        if self.input.source_tag != 0 {
            builder.source_tag(self.input.source_tag.try_into_u32("sourceTag")?);
        }
        Ok(builder)
    }

    fn signing_public_key(&self) -> SigningResult<secp256k1::PublicKey> {
        if !self.input.private_key.is_empty() {
            secp256k1::PrivateKey::try_from(self.input.private_key.as_ref())
                .into_tw()
                .context("Invalid private key")
                .map(|key| key.public())
        } else if !self.input.public_key.is_empty() {
            secp256k1::PublicKey::try_from(self.input.public_key.as_ref())
                .into_tw()
                .context("Invalid public key")
        } else {
            SigningError::err(SigningErrorType::Error_invalid_params)
                .context("Expected either 'privateKey' or 'publicKey' to be provided")
        }
    }

    fn fee(&self) -> SigningResult<NativeAmount> {
        NativeAmount::new(self.input.fee).context("Invalid fee")
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

trait AsUint32: TryInto<u32> {
    fn try_into_u32(self, param: &str) -> SigningResult<u32> {
        self.try_into()
            .map_err(|_| SigningError::new(SigningErrorType::Error_invalid_params))
            .with_context(|| format!("'{param}' must fit u32"))
    }

    /// Tries to convert `self` as `u32`.
    /// Returns error if `self` doesn't fit `u32` type, or returns `None` if `self` is 0.
    fn try_into_u32_optional(self, param: &str) -> SigningResult<Option<u32>> {
        self.try_into_u32(param).map(u32::zero_or_some)
    }
}

impl<T> AsUint32 for T where T: TryInto<u32> {}
