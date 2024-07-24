// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::TonAddress;
use crate::message::internal_message::transfer::TransferInternalMessage;
use crate::message::internal_message::InternalMessage;
use crate::message::payload::comment::CommentPayload;
use crate::message::payload::empty::EmptyPayload;
use crate::message::payload::jetton_transfer::JettonTransferPayload;
use crate::wallet::wallet_v4::WalletV4;
use crate::wallet::TonWallet;
use tw_coin_entry::error::prelude::ResultContext;
use tw_number::U256;
use tw_ton_sdk::cell::Cell;
use tw_ton_sdk::error::CellResult;

pub mod builder;

pub enum TransferPayload {
    JettonTransfer(JettonTransferRequest),
}

pub struct TransferRequest {
    /// TON recipient address.
    /// Also determines whether the transaction is bounceable or not.
    pub dest: TonAddress,
    /// Amount to send in nanotons.
    pub ton_amount: U256,
    /// Message counter.
    /// https://ton.org/docs/develop/smart-contracts/guidelines/external-messages
    pub seqno: u32,
    /// Send mode.
    /// https://ton.org/docs/develop/func/stdlib#send_raw_message
    pub mode: u8,
    /// Expiration UNIX timestamp.
    pub expire_at: u32,
    /// Transfer comment message.
    pub comment: Option<String>,
    /// Transfer payload.
    pub payload: Option<TransferPayload>,
}

pub struct JettonTransferRequest {
    /// Arbitrary request number.
    query_id: u64,
    /// Amount of transferred jettons in elementary integer units.
    /// The real value transferred is `jetton_amount` multiplied by ten to the power of token decimal precision.
    jetton_amount: U256,
    /// Address of the new owner of the jettons.
    dest: TonAddress,
    /// Address where to send a response with confirmation of a successful transfer and the rest of the incoming message Toncoins.
    /// Usually the sender should get back their toncoins.
    response_address: TonAddress,
    /// Amount in nanotons to forward to recipient. Basically minimum amount - 1 nanoton should be used.
    forward_ton_amount: U256,
}

pub struct SigningRequest {
    /// Wallet initialized with the user's key-pair or public key.
    pub wallet: TonWallet<WalletV4>,
    pub transfer_request: TransferRequest,
}

impl SigningRequest {
    pub fn transfer_payload(&self) -> CellResult<Cell> {
        if let Some(TransferPayload::JettonTransfer(ref jetton)) = self.transfer_request.payload {
            let mut payload = JettonTransferPayload::new(jetton.dest.clone(), jetton.jetton_amount);
            payload
                .with_query_id(jetton.query_id)
                .with_response_destination(jetton.response_address.clone())
                .with_forward_ton_amount(jetton.forward_ton_amount);

            if let Some(ref comment) = self.transfer_request.comment {
                payload.with_comment(comment.clone());
            }

            return payload
                .build()
                .context("Error generating Jetton Transfer payload");
        }

        // Otherwise, this is an ordinary TON transfer with an optional comment.
        match self.transfer_request.comment {
            Some(ref comment) => CommentPayload::new(comment.clone())
                .build()
                .context("Error generating Transfer's comment payload"),
            None => EmptyPayload
                .build()
                .context("Error generating Transfer's empty payload"),
        }
    }

    pub fn create_external_message_to_sign(&self) -> CellResult<Cell> {
        let mut transfer_message = TransferInternalMessage::new(
            self.transfer_request.dest.clone(),
            self.transfer_request.ton_amount,
        );
        transfer_message.with_data(self.transfer_payload()?);

        let transfer_message_cell = transfer_message
            .build()
            .context("Error generating 'Transfer' internal message cell")?;
        let internal_message =
            InternalMessage::new(self.transfer_request.mode, transfer_message_cell);

        self.wallet
            .create_external_body(
                self.transfer_request.expire_at,
                self.transfer_request.seqno,
                vec![internal_message],
            )
            .context("Error generating an external message cell")
    }
}
