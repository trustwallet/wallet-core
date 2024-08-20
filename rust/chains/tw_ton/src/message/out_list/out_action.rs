// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_ton_sdk::cell::cell_builder::CellBuilder;
use tw_ton_sdk::cell::{Cell, CellArc};
use tw_ton_sdk::error::CellResult;

#[derive(Clone)]
pub enum OutActionType {
    // Currently, only SendMsg is supported. SetCode is not supported.
    SendMsg,
}

impl OutActionType {
    // See https://ton.org/tblkch.pdf 4.4.11. Serialization of output actions
    pub fn to_ser_tag(&self) -> u32 {
        match self {
            OutActionType::SendMsg => 0x0ec3c86d,
        }
    }
}

#[derive(Clone)]
pub struct OutAction {
    pub typ: OutActionType,
    pub mode: u8,
    pub data: CellArc, // out msg (SendMsg) or new code (SetCode)
}

impl OutAction {
    pub fn new(typ: OutActionType, mode: u8, out_msg: CellArc) -> Self {
        OutAction {
            typ,
            mode,
            data: out_msg,
        }
    }

    pub fn build(&self) -> CellResult<Cell> {
        match self.typ {
            OutActionType::SendMsg => self.build_out_action_send_msg(),
        }
    }

    fn build_out_action_send_msg(&self) -> CellResult<Cell> {
        let mut builder = CellBuilder::new();
        builder
            .store_u32(32, self.typ.to_ser_tag())?
            .store_u8(8, self.mode)?
            .store_reference(&self.data)?;
        builder.build()
    }
}
