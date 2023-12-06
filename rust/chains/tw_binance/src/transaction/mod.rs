// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::amino::Amino;
use crate::transaction::message::BinanceMessageBox;
use serde::Serialize;
use std::borrow::Cow;
use tw_coin_entry::error::{SigningError, SigningErrorType, SigningResult};
use tw_hash::{sha2, H256};
use tw_memory::Data;
use tw_proto::Binance::Proto;

const TRANSACTION_AMINO_PREFIX: [u8; 4] = [0xF0, 0x62, 0x5D, 0xEE];

pub mod message;

pub struct JsonTxPreimage {
    pub encoded_tx: String,
    pub tx_hash: H256,
}

#[derive(Serialize)]
pub struct Transaction {
    pub account_number: u64,
    pub chain_id: String,
    pub data: Data,
    pub memo: String,
    pub msgs: Vec<BinanceMessageBox>,
    pub sequence: u64,
    pub source: i64,
}

impl Transaction {
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
}

pub struct SignedTransaction {
    unsigned: Transaction,
    signatures: Vec<H256>,
}

impl SignedTransaction {
    pub fn encode(&self) -> SigningResult<Data> {
        let msgs = self
            .unsigned
            .msgs
            .iter()
            .map(|msg| msg.to_amino_protobuf().map(Cow::from))
            .collect::<SigningResult<Vec<_>>>()?;
        let signatures = self
            .signatures
            .iter()
            .map(|sign| sign.to_vec().into())
            .collect();

        let tx = Proto::Transaction {
            msgs,
            signatures,
            memo: self.unsigned.memo.clone().into(),
            source: self.unsigned.source,
            data: self.unsigned.data.clone().into(),
        };
        Amino::encode_proto_with_content_size_prefix(&TRANSACTION_AMINO_PREFIX, &tx)
            .map_err(SigningError::from)
    }
}
