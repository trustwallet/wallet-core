// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::TonAddress;
use crate::message::internal_message::InternalMessage;
use crate::message::signed_message::signed_message_v4::SignedMessageV4;
use crate::message::signed_message::signed_message_v5::SignedMessageV5;
use crate::transaction::SignedTransaction;
use tw_coin_entry::error::prelude::*;
use tw_keypair::ed25519::Signature;
use tw_keypair::traits::SigningKeyTrait;
use tw_number::U256;
use tw_ton_sdk::cell::Cell;
use tw_ton_sdk::error::{cell_to_signing_error, CellResult};
use tw_ton_sdk::message::state_init::StateInit;

pub mod wallet_v4;
pub mod wallet_v5;

/// Notes: Rust specialization for generic code is not completed. See https://github.com/rust-lang/rust/issues/31844
/// Currently, we use a workaround to implement the versioned TonWallet struct.
/// After the Rust specialization feature is finished, maybe we can remove this workaround.
pub enum VersionedTonWallet {
    V4R2(wallet_v4::WalletV4R2),
    V5R1(wallet_v5::WalletV5R1),
}

impl VersionedTonWallet {
    pub fn address(&self) -> &TonAddress {
        match self {
            Self::V4R2(wallet_v4r2) => &wallet_v4r2.address,
            Self::V5R1(wallet_v5r1) => &wallet_v5r1.address,
        }
    }

    pub fn state_init(&self) -> CellResult<StateInit> {
        match self {
            Self::V4R2(wallet_v4r2) => wallet_v4r2.state_init(),
            Self::V5R1(wallet_v5r1) => wallet_v5r1.state_init(),
        }
    }

    pub fn create_external_body(
        &self,
        expire_at: u32,
        seqno: u32,
        internal_messages: Vec<InternalMessage>,
    ) -> CellResult<Cell> {
        match self {
            Self::V4R2(wallet_v4r2) => {
                wallet_v4r2.create_external_body(expire_at, seqno, internal_messages)
            },
            Self::V5R1(wallet_v5r1) => {
                wallet_v5r1.create_external_body(expire_at, seqno, internal_messages)
            },
        }
    }

    pub fn sign_external_message(&self, external_message: Cell) -> SigningResult<Cell> {
        let message_hash = external_message.cell_hash();
        let sig = match self {
            Self::V4R2(wallet_v4r2) => wallet_v4r2.private_key.as_ref(),
            Self::V5R1(wallet_v5r1) => wallet_v5r1.private_key.as_ref(),
        }
        .or_tw_err(SigningErrorType::Error_internal)
        .context("'TonWallet' should be initialized with a key-pair to be able to sign a message")?
        .sign(message_hash.to_vec())?;

        self.compile_signed_external_message(external_message, sig)
    }

    pub fn compile_signed_external_message(
        &self,
        external_message: Cell,
        sig: Signature,
    ) -> SigningResult<Cell> {
        match self {
            Self::V4R2(_) => Ok(SignedMessageV4 {
                signature: sig.to_bytes(),
                external_message,
            }
            .build()
            .map_err(cell_to_signing_error)?),

            Self::V5R1(_) => Ok(SignedMessageV5 {
                signature: sig.to_bytes(),
                external_message,
            }
            .build()
            .map_err(cell_to_signing_error)?),
        }
    }

    pub fn sign_transaction(
        &self,
        external_message: Cell,
        state_init: bool,
    ) -> SigningResult<SignedTransaction> {
        let signed_external_message = self.sign_external_message(external_message.clone())?;
        self.compile_transaction(signed_external_message, state_init)
    }

    pub fn compile_transaction(
        &self,
        signed_external_message: Cell,
        state_init: bool,
    ) -> SigningResult<SignedTransaction> {
        let state_init = if state_init {
            let state_init = self.state_init().map_err(cell_to_signing_error)?;
            Some(state_init)
        } else {
            None
        };

        Ok(SignedTransaction {
            src_address: TonAddress::null(),
            // The wallet contract address.
            dest_address: self.address().clone(),
            import_fee: U256::zero(),
            state_init,
            signed_body: signed_external_message,
        })
    }
}
