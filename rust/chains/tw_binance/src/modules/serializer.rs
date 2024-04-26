// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::amino::AminoEncoder;
use crate::transaction::SignedTransaction;
use std::borrow::Cow;
use tw_coin_entry::error::prelude::*;
use tw_cosmos_sdk::public_key::CosmosPublicKey;
use tw_memory::Data;
use tw_misc::traits::ToBytesVec;
use tw_proto::serialize;
use tw_proto::Binance::Proto;

/// cbindgen:ignore
pub const TRANSACTION_AMINO_PREFIX: [u8; 4] = [0xF0, 0x62, 0x5D, 0xEE];
/// cbindgen:ignore
pub const PUBLIC_KEY_PREFIX: [u8; 4] = [0xEB, 0x5A, 0xE9, 0x87];

pub struct BinanceAminoSerializer;

impl BinanceAminoSerializer {
    pub fn serialize_signed_tx(tx: &SignedTransaction) -> SigningResult<Data> {
        let msgs = tx
            .unsigned
            .msgs
            .iter()
            .map(|msg| msg.as_ref().to_amino_protobuf().map(Cow::from))
            .collect::<SigningResult<Vec<_>>>()?;

        let signature = Self::serialize_signature(tx)?;
        let tx = Proto::Transaction {
            msgs,
            signatures: vec![signature.into()],
            memo: tx.unsigned.memo.clone().into(),
            source: tx.unsigned.source,
            data: tx.unsigned.data.clone().unwrap_or_default().into(),
        };
        Ok(AminoEncoder::new(&TRANSACTION_AMINO_PREFIX)
            .extend_with_msg(&tx)?
            .encode_size_prefixed()?)
    }

    pub fn serialize_public_key(public_key: Data) -> Data {
        let public_key_len = public_key.len() as u8;
        AminoEncoder::new(&PUBLIC_KEY_PREFIX)
            // Push the length of the public key.
            .extend_content(&[public_key_len])
            .extend_content(public_key.as_slice())
            .encode()
    }

    pub fn serialize_signature(signed: &SignedTransaction) -> SigningResult<Data> {
        let sign_msg = Proto::Signature {
            pub_key: Self::serialize_public_key(signed.signer.public_key.to_bytes()).into(),
            signature: signed.signer.signature.to_vec().into(),
            account_number: signed.unsigned.account_number,
            sequence: signed.unsigned.sequence,
        };
        // There is no need to use Amino encoding here as the prefix is empty.
        serialize(&sign_msg).tw_err(|_| SigningErrorType::Error_internal)
    }
}
