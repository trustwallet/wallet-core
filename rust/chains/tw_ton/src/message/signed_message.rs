// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::error::prelude::ResultContext;
use tw_hash::H512;
use tw_proto::TheOpenNetwork::Proto;
use tw_ton_sdk::cell::cell_builder::CellBuilder;
use tw_ton_sdk::cell::Cell;
use tw_ton_sdk::error::*;

pub struct SignedMessage {
    pub signature: H512,
    pub external_message: Cell,
}

impl SignedMessage {
    pub fn build(&self, wallet_version: Proto::WalletVersion) -> CellResult<Cell> {
        let mut body_builder = CellBuilder::new();

        match wallet_version {
            // In the case of WALLET_V4_R2, the signature is stored before the external message.
            Proto::WalletVersion::WALLET_V4_R2 => {
                body_builder.store_slice(self.signature.as_slice())?;
                body_builder.store_cell(&self.external_message)?;
            },
            // In the case of WALLET_V5_R1, the signature is stored after the external message.
            Proto::WalletVersion::WALLET_V5_R1 => {
                body_builder.store_cell(&self.external_message)?;
                body_builder.store_slice(self.signature.as_slice())?;
            },
            _ => {
                return CellError::err(CellErrorType::NotSupported)
                    .context("Unsupported wallet version");
            },
        }
        body_builder.build()
    }
}
