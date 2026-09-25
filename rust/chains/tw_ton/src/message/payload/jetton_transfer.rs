// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::TonAddress;
use tw_coin_entry::error::prelude::ResultContext;
use tw_number::U256;
use tw_ton_sdk::cell::cell_builder::CellBuilder;
use tw_ton_sdk::cell::{Cell, CellArc};
use tw_ton_sdk::error::{CellError, CellErrorType, CellResult};

pub const JETTON_TRANSFER: u32 = 0x0f8a7ea5;

/// Jetton transfer message payload with an optional comment.
#[derive(Debug)]
pub struct JettonTransferPayload {
    /// Arbitrary request number.
    query_id: u64,
    /// Amount of transferred jettons in elementary units.
    jetton_amount: U256,
    /// Address of the new owner of the jettons.
    destination: TonAddress,
    /// Address where to send a response with confirmation of a successful transfer and the rest of the incoming message Toncoins.
    response_destination: TonAddress,
    /// Optional custom data (which is used by either sender or receiver jetton wallet for inner logic).
    custom_payload: Option<CellArc>,
    /// Amount of nanotons to be sent to the destination address.
    forward_ton_amount: U256,
    /// Optional custom data that should be sent to the destination address.
    /// At WalletCore, we do not use `forward_payload` at the moment.
    #[allow(dead_code)]
    forward_payload: Option<CellArc>,
    /// Optional transfer comment.
    comment: Option<String>,
}

impl JettonTransferPayload {
    pub fn new(destination: TonAddress, jetton_amount: U256) -> Self {
        JettonTransferPayload {
            query_id: 0,
            jetton_amount,
            destination,
            response_destination: TonAddress::null(),
            custom_payload: None,
            forward_ton_amount: U256::zero(),
            forward_payload: None,
            comment: None,
        }
    }

    pub fn with_query_id(&mut self, query_id: u64) -> &mut Self {
        self.query_id = query_id;
        self
    }

    pub fn with_response_destination(&mut self, response_destination: TonAddress) -> &mut Self {
        self.response_destination = response_destination;
        self
    }

    pub fn with_comment(&mut self, comment: String) -> &mut Self {
        self.comment = Some(comment);
        self
    }

    pub fn with_custom_payload(&mut self, custom_payload: Option<CellArc>) -> &mut Self {
        self.custom_payload = custom_payload;
        self
    }

    pub fn with_forward_ton_amount(&mut self, forward_ton_amount: U256) -> &mut Self {
        self.forward_ton_amount = forward_ton_amount;
        self
    }

    pub fn build(&self) -> CellResult<Cell> {
        if self.forward_ton_amount.is_zero() && self.forward_payload.is_some() {
            return CellError::err(CellErrorType::CellBuilderError)
                .context("Forward_ton_amount must be positive when specifying forward_payload");
        }

        let mut message = CellBuilder::new();
        message.store_u32(32, JETTON_TRANSFER)?;
        message.store_u64(64, self.query_id)?;
        message.store_coins(&self.jetton_amount)?;
        message.store_address(&self.destination)?;
        message.store_address(&self.response_destination)?;

        if let Some(ref cp) = self.custom_payload {
            message.store_bit(true)?;
            message.store_reference(cp)?;
        } else {
            message.store_bit(false)?;
        }

        message.store_coins(&self.forward_ton_amount)?;

        if let Some(ref fp) = self.forward_payload {
            message.store_bit(true)?;
            message.store_reference(fp)?;
        } else {
            message.store_bit(false)?;
        }

        if let Some(ref comment) = self.comment {
            message.store_u32(32, 0)?;
            message.store_string(comment)?;
        }

        message.build()
    }
}
