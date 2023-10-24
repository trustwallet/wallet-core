// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::proto::cosmos;
use tw_proto::Cosmos::Proto::SigningInput;

/// `ProtobufSerializer` serializes a signing input into Cosmos specific Protobuf messages.
pub struct ProtobufSerializer;

// pub struct ProtobufBody

impl ProtobufSerializer {
    pub fn build_tx_body(input: SigningInput<'_>) -> cosmos::tx::v1beta1::TxBody {
        todo!()
    }
}
