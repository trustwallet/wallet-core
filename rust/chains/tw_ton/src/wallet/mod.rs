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

    pub fn sign(&self, msg_to_sign: Vec<u8>) -> SigningResult<Signature> {
        let sig = match self {
            Self::V4R2(wallet_v4r2) => wallet_v4r2.private_key.as_ref(),
            Self::V5R1(wallet_v5r1) => wallet_v5r1.private_key.as_ref(),
        }
        .or_tw_err(SigningErrorType::Error_internal)
        .context("'TonWallet' should be initialized with a key-pair to be able to sign a message")?
        .sign(msg_to_sign)?;

        Ok(sig)
    }

    pub fn compile_signed_transaction(
        &self,
        external_message: Cell,
        state_init: bool,
        signature: Signature,
    ) -> SigningResult<SignedTransaction> {
        let state_init = if state_init {
            match self {
                Self::V4R2(wallet_v4r2) => {
                    Some(wallet_v4r2.state_init().map_err(cell_to_signing_error)?)
                },
                Self::V5R1(wallet_v5r1) => {
                    Some(wallet_v5r1.state_init().map_err(cell_to_signing_error)?)
                },
            }
        } else {
            None
        };

        let signed_body = match self {
            Self::V4R2(_) => SignedMessageV4 {
                signature: signature.to_bytes(),
                external_message,
            }
            .build()
            .map_err(cell_to_signing_error)?,

            Self::V5R1(_) => SignedMessageV5 {
                signature: signature.to_bytes(),
                external_message,
            }
            .build()
            .map_err(cell_to_signing_error)?,
        };

        Ok(SignedTransaction {
            src_address: TonAddress::null(),
            // The wallet contract address.
            dest_address: self.address().clone(),
            import_fee: U256::zero(),
            state_init,
            signed_body,
        })
    }
}
