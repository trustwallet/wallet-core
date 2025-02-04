// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::classic_address::ClassicAddress;
use crate::address::RippleAddress;
use crate::transaction::common_fields::CommonFields;
use crate::transaction::transactions::escrow_cancel::EscrowCancel;
use crate::transaction::transactions::payment::Payment;
use crate::transaction::transactions::trust_set::TrustSet;
use crate::types::amount::issued_currency::IssuedCurrency;
use crate::types::amount::native_amount::NativeAmount;
use crate::types::amount::Amount;
use std::str::FromStr;
use tw_coin_entry::error::prelude::{
    IntoTWError, ResultContext, SigningError, SigningErrorType, SigningResult,
};
use tw_encoding::hex::as_hex::AsHex;
use tw_keypair::ecdsa::secp256k1;

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
        self.common_fields.flags = flags;
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

    pub fn signing_pub_key_str(&mut self, signing_pub_key: &str) -> SigningResult<&mut Self> {
        let signing_pub_key = secp256k1::PublicKey::try_from(signing_pub_key)
            .into_tw()
            .context("Invalid signing public key")?;
        Ok(self.signing_pub_key(&signing_pub_key))
    }

    pub fn source_tag(&mut self, source_tag: u32) -> &mut Self {
        self.common_fields.source_tag = Some(source_tag);
        self
    }

    pub fn txn_signature(
        &mut self,
        txn_signature: &secp256k1::Signature,
    ) -> SigningResult<&mut Self> {
        let sign = txn_signature
            .to_der()
            .into_tw()
            .context("Error encoding secp256k1 signature as DER")?;
        self.common_fields.txn_signature = Some(AsHex(sign));
        Ok(self)
    }

    pub fn payment(
        self,
        amount: Amount,
        destination: RippleAddress,
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
