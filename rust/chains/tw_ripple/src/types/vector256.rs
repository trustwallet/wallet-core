// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::encode::encoder::Encoder;
use crate::encode::Encodable;
use serde::{Deserialize, Serialize};
use tw_coin_entry::error::prelude::SigningResult;
use tw_encoding::hex::as_hex::AsHex;
use tw_hash::H256;

#[derive(Clone, Debug, Deserialize, Eq, PartialEq, Serialize)]
pub struct Vector256(pub Vec<AsHex<H256>>);

impl Vector256 {
    pub fn new<I>(items: I) -> Self
    where
        I: IntoIterator<Item = H256>,
    {
        Vector256(items.into_iter().map(AsHex).collect())
    }
}

impl Encodable for Vector256 {
    fn encode(&self, dst: &mut Encoder) -> SigningResult<()> {
        for item in self.0.iter() {
            dst.append_raw_slice(item.0.as_slice());
        }
        Ok(())
    }
}
