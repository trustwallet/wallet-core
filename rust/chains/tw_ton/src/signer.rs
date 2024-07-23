// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::message::internal_message::transfer::TransferInternalMessage;
use crate::message::internal_message::InternalMessage;
use crate::message::payload::comment::CommentPayload;
use crate::message::payload::empty::EmptyPayload;
use crate::signing_request::builder::SigningRequestBuilder;
use crate::signing_request::SigningRequest;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::signing_output_error;
use tw_proto::TheOpenNetwork::Proto;
use tw_ton_sdk::boc::BagOfCells;
use tw_ton_sdk::error::cell_to_signing_error;

const HAS_CRC32: bool = true;

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
        let SigningRequest {
            wallet,
            transfer_request,
        } = SigningRequestBuilder::build(&input)?;

        let mut transfer_message =
            TransferInternalMessage::new(transfer_request.dest, transfer_request.ton_amount);

        // Whether to add 'StateInit' reference.
        let state_init = transfer_request.seqno == 0;
        if transfer_request.seqno == 0 {}

        let transfer_payload = match transfer_request.comment {
            Some(comment) => CommentPayload::new(comment)
                .build()
                .context("Error generating Transfer's comment payload")
                .map_err(cell_to_signing_error)?,
            None => EmptyPayload
                .build()
                .context("Error generating Transfer's empty payload")
                .map_err(cell_to_signing_error)?,
        };
        transfer_message.with_data(transfer_payload);

        let transfer_message_cell = transfer_message
            .build()
            .context("Error generating 'Transfer' internal message cell")
            .map_err(cell_to_signing_error)?;
        let internal_message = InternalMessage::new(transfer_request.mode, transfer_message_cell);

        let external_message = wallet
            .create_external_body(
                transfer_request.expire_at,
                transfer_request.seqno,
                vec![internal_message],
            )
            .context("Error generating an external message cell")
            .map_err(cell_to_signing_error)?;

        let signed_tx = wallet
            .sign_transaction(external_message, state_init)
            .context("Error signing/wrapping an external message")?
            .build()
            .context("Error generating signed message cell")
            .map_err(cell_to_signing_error)?;

        let signed_tx_encoded = BagOfCells::from_root(signed_tx)
            .to_base64(HAS_CRC32)
            .context("Error serializing signed transaction as BoC")
            .map_err(cell_to_signing_error)?;

        Ok(Proto::SigningOutput {
            encoded: signed_tx_encoded.into(),
            ..Proto::SigningOutput::default()
        })
    }
}
