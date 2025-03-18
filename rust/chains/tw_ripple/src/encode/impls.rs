// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::encode::encoder::Encoder;
use crate::encode::Encodable;
use tw_coin_entry::error::prelude::SigningResult;
use tw_hash::Hash;

impl<const N: usize> Encodable for Hash<N> {
    fn encode(&self, dst: &mut Encoder) -> SigningResult<()> {
        dst.append_raw_slice(self.as_slice());
        Ok(())
    }
}

macro_rules! encodable_int {
    ($int:ty) => {
        impl Encodable for $int {
            fn encode(&self, dst: &mut Encoder) -> SigningResult<()> {
                dst.append_raw_slice(&self.to_be_bytes());
                Ok(())
            }
        }
    };
}

encodable_int!(u8);
encodable_int!(u16);
encodable_int!(u32);
encodable_int!(u64);
