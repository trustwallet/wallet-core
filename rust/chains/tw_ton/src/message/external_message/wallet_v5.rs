// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::message::out_list::build_out_list;
use crate::message::out_list::out_action::OutAction;
use tw_coin_entry::error::prelude::ResultContext;
use tw_ton_sdk::cell::cell_builder::CellBuilder;
use tw_ton_sdk::cell::Cell;
use tw_ton_sdk::error::{CellError, CellErrorType, CellResult};

pub enum V5R1OpCode {
    AuthSignedExternal,
    AuthSignedInternal,
    AuthExtension,
}

impl V5R1OpCode {
    // See https://github.com/ton-blockchain/wallet-contract-v5/blob/88557ebc33047a95207f6e47ac8aadb102dff744/contracts/wallet_v5.fc#L33
    pub fn to_ser_tag(&self) -> u32 {
        match self {
            V5R1OpCode::AuthSignedExternal => 0x0ec3c86d,
            V5R1OpCode::AuthSignedInternal => 0x73696e74,
            V5R1OpCode::AuthExtension => 0x6578746e,
        }
    }
}

pub struct ExternalMessageWalletV5 {
    pub opcode: V5R1OpCode,
    pub wallet_id: i32,
    pub expire_at: u32,
    pub seqno: u32,
    /// Currently, only basic actions are supported. Extended actions are not supported.
    pub basic_actions: Vec<OutAction>,
}

impl ExternalMessageWalletV5 {
    /// Build the external message for wallet v5.
    pub fn build(&self) -> CellResult<Cell> {
        // Check the number of basic actions
        if self.basic_actions.len() > 255 {
            return CellError::err(CellErrorType::InternalError)
                .context("Maximum number of actions in a single request is 255");
        }

        let mut builder = CellBuilder::new();
        match self.opcode {
            V5R1OpCode::AuthSignedExternal => {
                builder.store_u32(32, self.opcode.to_ser_tag())?;

                // Make sure +2 flag (ignore errors send mode) is set for all external send messages
                // See https://github.com/ton-blockchain/wallet-contract-v5/blob/88557ebc33047a95207f6e47ac8aadb102dff744/contracts/wallet_v5.fc#L82
                for action in &self.basic_actions {
                    if (action.mode & 0x02) == 0 {
                        return CellError::err(CellErrorType::InternalError)
                            .context("External send message must have ignore errors send mode");
                    }
                }
            },
            _ => {
                return CellError::err(CellErrorType::NotSupported)
                    .context("Unsupported V5R1OpCodes");
            },
        }
        builder
            .store_i32(32, self.wallet_id)?
            .store_u32(32, self.expire_at)?
            .store_u32(32, self.seqno)?;
        builder.store_bit(true)?; // true means basic actions is stored in reference

        let mut basic_actions = self.basic_actions.clone();
        basic_actions.reverse();

        builder.store_child(build_out_list(&basic_actions)?)?;
        builder.store_bit(false)?; // false means no extended actions
        builder.build()
    }
}
