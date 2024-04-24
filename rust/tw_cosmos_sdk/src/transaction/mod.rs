// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::context::CosmosContext;
use crate::private_key::SignatureData;
use serde::Serialize;
use tw_number::U256;

pub mod message;

use message::CosmosMessageBox;

/// At this moment, TW only supports the Direct signing mode.
#[derive(Clone, Copy)]
pub enum SignMode {
    Direct,
    Other(i32),
}

#[derive(Clone, Serialize)]
pub struct Fee<Address> {
    pub amounts: Vec<Coin>,
    pub gas_limit: u64,
    pub payer: Option<Address>,
    pub granter: Option<Address>,
}

#[derive(Clone, Serialize)]
pub struct Coin {
    #[serde(serialize_with = "U256::as_decimal_str")]
    pub amount: U256,
    pub denom: String,
}

pub struct SignerInfo<PublicKey> {
    pub public_key: PublicKey,
    pub sequence: u64,
    pub sign_mode: SignMode,
}

pub struct TxBody {
    pub messages: Vec<CosmosMessageBox>,
    pub memo: String,
    pub timeout_height: u64,
}

pub struct UnsignedTransaction<Context: CosmosContext> {
    pub signer: SignerInfo<Context::PublicKey>,
    pub fee: Fee<Context::Address>,
    pub chain_id: String,
    pub account_number: u64,
    pub tx_body: TxBody,
}

impl<Context: CosmosContext> UnsignedTransaction<Context> {
    pub fn into_signed(self, signature: SignatureData) -> SignedTransaction<Context> {
        SignedTransaction {
            signer: self.signer,
            fee: self.fee,
            tx_body: self.tx_body,
            signature,
        }
    }
}

pub struct SignedTransaction<Context: CosmosContext> {
    pub signer: SignerInfo<Context::PublicKey>,
    pub fee: Fee<Context::Address>,
    pub tx_body: TxBody,
    pub signature: SignatureData,
}
