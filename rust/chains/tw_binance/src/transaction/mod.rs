// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::signature::BinanceSignature;
use crate::transaction::message::BinanceMessageBox;
use serde::Serialize;
use tw_coin_entry::error::{SigningError, SigningErrorType, SigningResult};
use tw_hash::{sha2, H256};
use tw_keypair::ecdsa::secp256k1;
use tw_memory::Data;
use tw_misc::serde::as_string;

pub mod message;

pub struct JsonTxPreimage {
    pub encoded_tx: String,
    pub tx_hash: H256,
}

#[derive(Serialize)]
pub struct UnsignedTransaction {
    #[serde(serialize_with = "as_string")]
    pub account_number: i64,
    pub chain_id: String,
    pub data: Option<Data>,
    pub memo: String,
    pub msgs: Vec<BinanceMessageBox>,
    #[serde(serialize_with = "as_string")]
    pub sequence: i64,
    #[serde(serialize_with = "as_string")]
    pub source: i64,
}

impl UnsignedTransaction {
    /// TODO move to a module.
    pub fn preimage_hash(&self) -> SigningResult<JsonTxPreimage> {
        let encoded_tx = serde_json::to_string(self)
            .map_err(|_| SigningError(SigningErrorType::Error_internal))?;
        let tx_hash = sha2::sha256(encoded_tx.as_bytes());
        let tx_hash = H256::try_from(tx_hash.as_slice()).expect("sha256 must return 32 bytes");
        Ok(JsonTxPreimage {
            encoded_tx,
            tx_hash,
        })
    }

    pub fn into_signed(self, signer: SignerInfo) -> SignedTransaction {
        SignedTransaction {
            unsigned: self,
            signer,
        }
    }
}

pub struct SignerInfo {
    pub public_key: secp256k1::PublicKey,
    pub signature: BinanceSignature,
}

pub struct SignedTransaction {
    pub unsigned: UnsignedTransaction,
    pub signer: SignerInfo,
}
