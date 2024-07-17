// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::modules::tx_builder::{TWTransaction, TWTransactionBuilder};
use crate::modules::tx_signer::TxSigner;
use std::borrow::Cow;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::signing_output_error;
use tw_encoding::base64::{self, STANDARD};
use tw_proto::Sui::Proto;

pub struct SuiSigner;

impl SuiSigner {
    pub fn sign(
        coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
    ) -> Proto::SigningOutput<'static> {
        Self::sign_impl(coin, input)
            .unwrap_or_else(|e| signing_output_error!(Proto::SigningOutput, e))
    }

    fn sign_impl(
        _coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
    ) -> SigningResult<Proto::SigningOutput<'static>> {
        let builder = TWTransactionBuilder::new(input);
        let signer_key = builder.signer_key()?;
        let tx_to_sign = builder.build()?;

        let (preimage, signature) = match tx_to_sign {
            TWTransaction::Transaction(tx) => TxSigner::sign(&tx, &signer_key)?,
            TWTransaction::SignDirect(tx_data) => TxSigner::sign_direct(tx_data, &signer_key)?,
        };

        let unsigned_tx = base64::encode(&preimage.unsigned_tx_data, STANDARD);
        Ok(Proto::SigningOutput {
            unsigned_tx: Cow::from(unsigned_tx),
            signature: Cow::from(signature.to_base64()),
            ..Proto::SigningOutput::default()
        })
    }
}
