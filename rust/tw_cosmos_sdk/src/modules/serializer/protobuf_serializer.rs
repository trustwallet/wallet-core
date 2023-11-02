// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::proto::cosmos;
use crate::proto::cosmos::signing::v1beta1::SignMode;
use crate::proto::cosmos::tx::v1beta1::{SignDoc, TxBody};
use crate::public_key::{CosmosPubkey, ProtobufPublicKey};
use crate::transaction::protobuf_tx::ProtobufTransaction;
use crate::transaction::TransactionMeta;
use tw_coin_entry::error::{SigningError, SigningErrorType, SigningResult};
use tw_proto::Cosmos::Proto;
use tw_proto::Cosmos::Proto::SigningMode;
use tw_proto::{google, serialize, to_any};

const TIMEOUT_HEIGHT: u64 = 0;

pub struct SignerInfo {
    mode: SigningMode,
    sequence: u64,
    public_key: google::protobuf::Any,
}

impl SignerInfo {
    pub fn new<PublicKey>(mode: SigningMode, sequence: u64, public_key: PublicKey) -> SignerInfo
    where
        PublicKey: ProtobufPublicKey,
    {
        SignerInfo {
            mode,
            sequence,
            public_key: public_key.into_proto(),
        }
    }

    pub fn into_auth_info
}

/// `ProtobufSerializer` serializes Cosmos specific Protobuf messages.
pub struct ProtobufSerializer;

impl ProtobufSerializer {
    pub fn build_sign_doc(
        tx: ProtobufTransaction,
        signer_info: SignerInfo,
    ) -> SigningResult<SignDoc> {
        let tx_body = tx.into_proto_tx_body()?;
        SignDoc {
            body_bytes: serialize(&tx_body)?,
            auth_info_bytes:
        }
    }
}
