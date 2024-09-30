// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::compiler::HAS_CRC32;
use crate::signing_request::builder::SigningRequestBuilder;
use crate::signing_request::cell_creator::ExternalMessageCreator;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::signing_output_error;
use tw_misc::traits::ToBytesVec;
use tw_proto::TheOpenNetwork::Proto;
use tw_ton_sdk::boc::BagOfCells;
use tw_ton_sdk::error::cell_to_signing_error;

pub struct TheOpenNetworkSigner;

impl TheOpenNetworkSigner {
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
        let signing_request = SigningRequestBuilder::build(&input)?;

        let external_message =
            ExternalMessageCreator::create_external_message_to_sign(&signing_request)
                .map_err(cell_to_signing_error)?;

        // Whether to add 'StateInit' reference.
        let state_init = signing_request.seqno == 0;
        let signed_tx = signing_request
            .wallet
            .sign_transaction(external_message, state_init)
            .context("Error signing/wrapping an external message")?
            .build()
            .context("Error generating signed message cell")
            .map_err(cell_to_signing_error)?;

        let signed_tx_hash = signed_tx.cell_hash();
        let signed_tx_encoded = BagOfCells::from_root(signed_tx)
            .to_base64(HAS_CRC32)
            .context("Error serializing signed transaction as BoC")
            .map_err(cell_to_signing_error)?;

        Ok(Proto::SigningOutput {
            encoded: signed_tx_encoded.into(),
            hash: signed_tx_hash.to_vec().into(),
            ..Proto::SigningOutput::default()
        })
    }
}
