// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::message::internal_message::transfer::TransferInternalMessage;
use crate::message::internal_message::InternalMessage;
use crate::message::payload::comment::CommentPayload;
use crate::message::payload::empty::EmptyPayload;
use crate::message::payload::jetton_transfer::JettonTransferPayload;
use crate::signing_request::{
    JettonTransferRequest, SigningRequest, TransferPayload, TransferRequest,
};
use std::sync::Arc;
use tw_coin_entry::error::prelude::ResultContext;
use tw_ton_sdk::boc::BagOfCells;
use tw_ton_sdk::cell::{Cell, CellArc};
use tw_ton_sdk::error::{CellError, CellErrorType, CellResult};

pub struct InternalMessageCreator;

impl InternalMessageCreator {
    pub fn create_internal_message(
        transfer_request: &TransferRequest,
    ) -> CellResult<InternalMessage> {
        let mut transfer_message = TransferInternalMessage::new(
            transfer_request.dest.clone(),
            transfer_request.ton_amount,
        );

        // Store a custom contract StateInit Cell if it's provided.
        if let Some(state_init) = Self::maybe_custom_state_init(transfer_request)? {
            transfer_message.with_state_init(state_init);
        }
        // In WalletCore, we always store the transfer data even if it's an empty Cell.
        transfer_message.with_data(Self::transfer_payload(transfer_request)?);

        let transfer_message_cell = transfer_message
            .build()
            .context("Error generating 'Transfer' internal message cell")?;

        Ok(InternalMessage::new(
            transfer_request.mode,
            transfer_message_cell,
        ))
    }

    fn transfer_payload(transfer_request: &TransferRequest) -> CellResult<CellArc> {
        match transfer_request.payload {
            Some(TransferPayload::JettonTransfer(ref jetton)) => {
                Self::jetton_transfer_payload(jetton, transfer_request.comment.clone())
            },
            Some(TransferPayload::Custom(ref custom)) => Self::custom_payload(custom),
            // Otherwise, this is an ordinary TON transfer with an optional comment.
            None => Self::maybe_comment_payload(transfer_request.comment.clone()),
        }
    }

    fn maybe_comment_payload(comment: Option<String>) -> CellResult<CellArc> {
        match comment {
            Some(comment) => CommentPayload::new(comment)
                .build()
                .context("Error generating Transfer's comment payload"),
            None => EmptyPayload
                .build()
                .context("Error generating Transfer's empty payload"),
        }
        .map(Cell::into_arc)
    }

    fn jetton_transfer_payload(
        jetton: &JettonTransferRequest,
        comment: Option<String>,
    ) -> CellResult<CellArc> {
        let custom_payload_cell = if let Some(ref custom_payload) = jetton.custom_payload {
            Some(
                BagOfCells::parse_base64(custom_payload)
                    .context("Error parsing JettonTransfer custom_payload")?
                    .single_root()
                    .map(Arc::clone)
                    .context("custom_payload must contain only one single root")?,
            )
        } else {
            None
        };

        let mut payload = JettonTransferPayload::new(jetton.dest.clone(), jetton.jetton_amount);
        payload
            .with_query_id(jetton.query_id)
            .with_response_destination(jetton.response_address.clone())
            .with_custom_payload(custom_payload_cell)
            .with_forward_ton_amount(jetton.forward_ton_amount);

        if let Some(comment) = comment {
            payload.with_comment(comment);
        }

        payload
            .build()
            .map(Cell::into_arc)
            .context("Error generating Jetton Transfer payload")
    }

    fn custom_payload(payload: &str) -> CellResult<CellArc> {
        BagOfCells::parse_base64(payload)
            .context("Error parsing custom Transfer payload")?
            .single_root()
            .map(Arc::clone)
            .context("Custom Transfer payload must contain only one single root")
    }

    fn maybe_custom_state_init(request: &TransferRequest) -> CellResult<Option<CellArc>> {
        let Some(ref state_init) = request.state_init else {
            return Ok(None);
        };

        let state_init_cell = BagOfCells::parse_base64(state_init)
            .context("Error parsing Transfer stateInit")?
            .single_root()
            .map(Arc::clone)
            .context("stateInit must contain only one single root")?;
        Ok(Some(state_init_cell))
    }
}

pub struct ExternalMessageCreator;

impl ExternalMessageCreator {
    pub fn create_external_message_to_sign(request: &SigningRequest) -> CellResult<Cell> {
        if request.messages.is_empty() {
            return CellError::err(CellErrorType::CellBuilderError)
                .context("There must be at least one Transfer message");
        }

        let internal_messages = request
            .messages
            .iter()
            .map(InternalMessageCreator::create_internal_message)
            .collect::<CellResult<Vec<_>>>()?;

        request
            .wallet
            .create_external_body(request.expire_at, request.seqno, internal_messages)
            .context("Error generating an external message cell")
    }
}
