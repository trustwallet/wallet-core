// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::signature::BinanceSignature;
use crate::transaction::message::BinanceMessageEnum;
use serde::{Deserialize, Serialize};
use tw_cosmos_sdk::public_key::secp256k1::Secp256PublicKey;
use tw_memory::Data;
use tw_misc::serde::as_string;

pub mod message;

#[derive(Deserialize, Serialize)]
pub struct UnsignedTransaction {
    #[serde(with = "as_string")]
    pub account_number: i64,
    pub chain_id: String,
    pub data: Option<Data>,
    pub memo: String,
    pub msgs: Vec<BinanceMessageEnum>,
    #[serde(with = "as_string")]
    pub sequence: i64,
    #[serde(with = "as_string")]
    pub source: i64,
}

impl UnsignedTransaction {
    pub fn into_signed(self, signer: SignerInfo) -> SignedTransaction {
        SignedTransaction {
            unsigned: self,
            signer,
        }
    }
}

pub struct SignerInfo {
    pub public_key: Secp256PublicKey,
    pub signature: BinanceSignature,
}

pub struct SignedTransaction {
    pub unsigned: UnsignedTransaction,
    pub signer: SignerInfo,
}
