// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

pub mod protobuf_signer;

pub struct TransactionSigner;

// TODO move to a module.

// pub trait BroadcastMode: ToString {}
//
// pub struct BroadcastJson {
//     tx_bytes: String,
//     mode: String,
// }
//
// pub fn encode_broadcast_json<Mode>(&self, broadcast_mode: Mode) -> BroadcastJson
// where
//     Mode: BroadcastMode,
// {
//     BroadcastJson {
//         tx_bytes: self.encode(),
//         mode: broadcast_mode.to_string(),
//     }
// }
