// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_ton_sdk::cell::{Cell, CellArc};

pub mod transfer;

pub struct InternalMessage {
    /// https://docs.ton.org/develop/smart-contracts/messages#message-modes
    pub mode: u8,
    pub message: CellArc,
}

impl InternalMessage {
    pub fn new(mode: u8, message: Cell) -> InternalMessage {
        InternalMessage {
            mode,
            message: message.into_arc(),
        }
    }
}
