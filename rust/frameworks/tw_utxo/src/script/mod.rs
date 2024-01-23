// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::encode::stream::Stream;
use crate::encode::Encodable;
use tw_memory::Data;

#[derive(Clone, Debug)]
pub struct ScriptPubkey {
    bytes: Data,
}

impl Encodable for ScriptPubkey {
    fn encode(&self, stream: &mut Stream) {
        stream.append(&self.bytes);
    }
}
