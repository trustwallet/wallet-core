// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::transaction::protobuf_tx::ProtobufTransaction;

pub struct ProtobufSigningInput {
    tx: ProtobufTransaction
}

pub struct ProtobufSigner;

impl ProtobufSigner {
    // pub fn sign(tx: ProtobufTransaction) ->
}
