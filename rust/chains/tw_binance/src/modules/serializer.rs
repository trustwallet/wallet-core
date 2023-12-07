// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::amino::AminoEncoder;
use crate::transaction::SignedTransaction;
use std::borrow::Cow;
use tw_coin_entry::error::{SigningError, SigningErrorType, SigningResult};
use tw_hash::H264;
use tw_memory::Data;
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
            .map(|msg| msg.to_amino_protobuf().map(Cow::from))
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

    pub fn serialize_public_key(public_key: H264) -> Data {
        let public_key_len = public_key.len() as u8;
        AminoEncoder::new(&PUBLIC_KEY_PREFIX)
            // Push the length of the public key.
            .extend_content(&[public_key_len])
            .extend_content(public_key.as_slice())
            .encode()
    }

    pub fn serialize_signature(signed: &SignedTransaction) -> SigningResult<Data> {
        let sign_msg = Proto::Signature {
            pub_key: Self::serialize_public_key(signed.signer.public_key).into(),
            signature: signed.signer.signature.to_vec().into(),
            account_number: signed.unsigned.account_number,
            sequence: signed.unsigned.sequence,
        };
        // There is no need to use Amino encoding here as the prefix is empty.
        serialize(&sign_msg).map_err(|_| SigningError(SigningErrorType::Error_internal))
    }
}
