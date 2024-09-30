// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::message::out_list::build_out_list;
use crate::message::out_list::out_action::OutAction;
use tw_coin_entry::error::prelude::ResultContext;
use tw_ton_sdk::cell::cell_builder::CellBuilder;
use tw_ton_sdk::cell::Cell;
use tw_ton_sdk::error::{CellError, CellErrorType, CellResult};

const SEND_MODE_IGNORE_ACTION_PHASE_ERRORS: u8 = 0x02;

pub enum V5R1OpCode {
    // Currently, only AuthSignedExternal is supported. AuthSignedInternal/AuthExtension are not supported.
    AuthSignedExternal,
}

impl V5R1OpCode {
    // See https://github.com/ton-blockchain/wallet-contract-v5/blob/88557ebc33047a95207f6e47ac8aadb102dff744/contracts/wallet_v5.fc#L33
    pub fn to_ser_tag(&self) -> u32 {
        match self {
            V5R1OpCode::AuthSignedExternal => 0x7369676e,
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
                    if (action.mode & SEND_MODE_IGNORE_ACTION_PHASE_ERRORS) == 0 {
                        return CellError::err(CellErrorType::InternalError)
                            .context("External send message must have ignore errors send mode");
                    }
                }
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
