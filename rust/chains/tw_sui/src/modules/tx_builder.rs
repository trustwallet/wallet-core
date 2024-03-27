// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::SuiAddress;
use crate::transaction::transaction_data::TransactionData;
use std::str::FromStr;
use tw_coin_entry::error::{SigningError, SigningErrorType, SigningResult};
use tw_encoding::{base64, bcs};
use tw_keypair::ed25519;
use tw_keypair::traits::KeyPairTrait;
use tw_proto::Sui::Proto;
use tw_proto::Sui::Proto::mod_SigningInput::OneOftransaction_payload as TransactionType;

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

    pub fn build(self) -> SigningResult<TransactionData> {
        match self.input.transaction_payload {
            TransactionType::sign_direct_message(ref direct) => self.sign_direct_from_proto(direct),
            _ => todo!(),
        }
    }

    fn sign_direct_from_proto(
        &self,
        sign_direct: &Proto::SignDirect<'_>,
    ) -> SigningResult<TransactionData> {
        let url = false;
        let tx: TransactionData = base64::decode(&sign_direct.unsigned_tx_msg, url)
            .and_then(|tx_bytes| bcs::decode(&tx_bytes))
            .map_err(|_| SigningError(SigningErrorType::Error_input_parse))?;

        Ok(tx)
    }

    fn signer_address(&self) -> SigningResult<SuiAddress> {
        if self.input.private_key.is_empty() {
            SuiAddress::from_str(&self.input.signer).map_err(SigningError::from)
        } else {
            let keypair = self.signer_key()?;
            SuiAddress::with_ed25519_pubkey(keypair.public()).map_err(SigningError::from)
        }
    }
}
