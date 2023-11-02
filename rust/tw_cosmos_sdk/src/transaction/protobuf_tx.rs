// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::proto::cosmos::tx::v1beta1::TxBody;
use crate::transaction::TransactionMeta;
use tw_coin_entry::error::{SigningError, SigningErrorType, SigningResult};
use tw_proto::google;

pub struct ProtobufTransaction {
    pub messages: Vec<google::protobuf::Any>,
    pub tx_meta: TransactionMeta,
}

impl ProtobufTransaction {
    pub fn new(tx_meta: TransactionMeta) -> ProtobufTransaction {
        ProtobufTransaction {
            messages: Vec::default(),
            tx_meta,
        }
    }

    pub fn push_msg<T: ProtobufTransactionMessage + Sized>(&mut self, msg: T) {
        self.messages.push(msg.into_boxed())
    }

    pub fn into_proto_tx_body(self) -> SigningResult<TxBody> {
        if self.messages.is_empty() {
            return Err(SigningError(SigningErrorType::Error_invalid_params));
        }

        Ok(TxBody {
            messages,
            memo: self.tx_meta.memo.to_string(),
            timeout_height: self.tx_meta.timeout_height,
            extension_options: Vec::default(),
            non_critical_extension_options: Vec::default(),
        })
    }
}

pub trait ProtobufTransactionMessage {
    fn into_proto_tx_msg(self) -> google::protobuf::Any;
}
