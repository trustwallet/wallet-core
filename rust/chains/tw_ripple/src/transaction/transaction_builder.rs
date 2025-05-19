// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::classic_address::ClassicAddress;
use crate::address::RippleAddress;
use crate::transaction::common_fields::CommonFields;
use crate::transaction::transactions::escrow_cancel::EscrowCancel;
use crate::transaction::transactions::escrow_create::EscrowCreate;
use crate::transaction::transactions::escrow_finish::EscrowFinish;
use crate::transaction::transactions::nftoken_accept_offer::NFTokenAcceptOffer;
use crate::transaction::transactions::nftoken_burn::NFTokenBurn;
use crate::transaction::transactions::nftoken_cancel_offer::NFTokenCancelOffer;
use crate::transaction::transactions::nftoken_create_offer::NFTokenCreateOffer;
use crate::transaction::transactions::payment::Payment;
use crate::transaction::transactions::trust_set::TrustSet;
use crate::types::amount::issued_currency::IssuedCurrency;
use crate::types::amount::native_amount::NativeAmount;
use crate::types::amount::Amount;
use crate::types::vector256::Vector256;
use std::str::FromStr;
use tw_coin_entry::error::prelude::*;
use tw_encoding::hex::as_hex::AsHex;
use tw_hash::H256;
use tw_keypair::ecdsa::secp256k1;
use tw_memory::Data;

#[derive(Default)]
pub struct TransactionBuilder {
    common_fields: CommonFields,
}

impl TransactionBuilder {
    pub fn account(&mut self, account: RippleAddress) -> &mut Self {
        self.common_fields.account = Some(account);
        self
    }

    pub fn account_str(&mut self, account: &str) -> SigningResult<&mut Self> {
        let addr = RippleAddress::from_str(account)
            .into_tw()
            .context("Invalid 'account' address")?;
        Ok(self.account(addr))
    }

    pub fn fee(&mut self, fee: NativeAmount) -> &mut Self {
        self.common_fields.fee = Some(fee);
        self
    }

    pub fn flags(&mut self, flags: u32) -> &mut Self {
        self.common_fields.flags = Some(flags);
        self
    }

    pub fn last_ledger_sequence(&mut self, last_ledger_sequence: u32) -> &mut Self {
        self.common_fields.last_ledger_sequence = Some(last_ledger_sequence);
        self
    }

    pub fn sequence(&mut self, sequence: u32) -> &mut Self {
        self.common_fields.sequence = Some(sequence);
        self
    }

    pub fn signing_pub_key(&mut self, signing_pub_key: &secp256k1::PublicKey) -> &mut Self {
        self.common_fields.signing_pub_key = Some(AsHex(signing_pub_key.compressed()));
        self
    }

    pub fn source_tag(&mut self, source_tag: u32) -> &mut Self {
        self.common_fields.source_tag = Some(source_tag);
        self
    }

    pub fn payment(
        self,
        amount: Amount,
        destination: ClassicAddress,
        destination_tag: Option<u32>,
    ) -> SigningResult<Payment> {
        self.check_ready()?;
        Ok(Payment {
            common_fields: self.common_fields,
            amount,
            destination,
            destination_tag,
        })
    }

    pub fn trust_set(self, limit_amount: IssuedCurrency) -> SigningResult<TrustSet> {
        self.check_ready()?;
        Ok(TrustSet {
            common_fields: self.common_fields,
            limit_amount,
        })
    }

    pub fn escrow_create(
        self,
        amount: NativeAmount,
        destination: ClassicAddress,
        destination_tag: Option<u32>,
        cancel_after: Option<u32>,
        finish_after: Option<u32>,
        condition: Option<Data>,
    ) -> SigningResult<EscrowCreate> {
        self.check_ready()?;

        if cancel_after.is_none() && finish_after.is_none() {
            return SigningError::err(SigningErrorType::Error_invalid_params)
                .context("Either CancelAfter or FinishAfter must be specified");
        }

        if finish_after.is_none() && condition.is_none() {
            return SigningError::err(SigningErrorType::Error_invalid_params)
                .context("Either Condition or FinishAfter must be specified");
        }

        Ok(EscrowCreate {
            common_fields: self.common_fields,
            amount,
            destination,
            destination_tag,
            cancel_after,
            finish_after,
            condition: condition.map(AsHex),
        })
    }

    pub fn escrow_cancel(
        self,
        owner: ClassicAddress,
        offer_sequence: u32,
    ) -> SigningResult<EscrowCancel> {
        self.check_ready()?;
        Ok(EscrowCancel {
            common_fields: self.common_fields,
            owner,
            offer_sequence,
        })
    }

    pub fn escrow_finish(
        self,
        owner: ClassicAddress,
        offer_sequence: u32,
        condition: Option<Data>,
        fulfillment: Option<Data>,
    ) -> SigningResult<EscrowFinish> {
        self.check_ready()?;
        Ok(EscrowFinish {
            common_fields: self.common_fields,
            owner,
            offer_sequence,
            condition: condition.map(AsHex),
            fulfillment: fulfillment.map(AsHex),
        })
    }

    pub fn nftoken_burn(
        self,
        nftoken_id: H256,
        owner: Option<ClassicAddress>,
    ) -> SigningResult<NFTokenBurn> {
        self.check_ready()?;
        Ok(NFTokenBurn {
            common_fields: self.common_fields,
            nftoken_id,
            owner,
        })
    }

    pub fn nftoken_create_offer(
        self,
        nftoken_id: H256,
        amount: Amount,
        owner: Option<ClassicAddress>,
        expiration: Option<u32>,
        destination: Option<ClassicAddress>,
    ) -> SigningResult<NFTokenCreateOffer> {
        self.check_ready()?;
        Ok(NFTokenCreateOffer {
            common_fields: self.common_fields,
            nftoken_id,
            amount,
            owner,
            expiration,
            destination,
        })
    }

    pub fn nftoken_accept_offer(
        self,
        nftoken_sell_offer: Option<H256>,
        nftoken_buy_offer: Option<H256>,
    ) -> SigningResult<NFTokenAcceptOffer> {
        self.check_ready()?;
        Ok(NFTokenAcceptOffer {
            common_fields: self.common_fields,
            nftoken_sell_offer: nftoken_sell_offer.map(AsHex),
            nftoken_buy_offer: nftoken_buy_offer.map(AsHex),
        })
    }

    pub fn nftoken_cancel_offer(
        self,
        nftoken_offers: Vec<H256>,
    ) -> SigningResult<NFTokenCancelOffer> {
        self.check_ready()?;
        Ok(NFTokenCancelOffer {
            common_fields: self.common_fields,
            nftoken_offers: Vector256::new(nftoken_offers),
        })
    }

    fn check_ready(&self) -> SigningResult<()> {
        if self.common_fields.signing_pub_key.is_none() {
            return SigningError::err(SigningErrorType::Error_internal)
                .context("'SigningPubKey' isn't set");
        }
        if self.common_fields.account.is_none() {
            return SigningError::err(SigningErrorType::Error_internal)
                .context("'Account' isn't set");
        }
        Ok(())
    }
}
