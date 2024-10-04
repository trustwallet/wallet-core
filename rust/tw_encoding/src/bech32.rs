// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use bech32::{FromBase32, ToBase32, Variant};
use tw_memory::Data;

pub use bech32::{convert_bits, CheckBase32, Error as Bech32Error};

pub type Bech32Result<T> = Result<T, Bech32Error>;

pub struct Decoded {
    pub hrp: String,
    pub bytes: Data,
}

pub fn encode(hrp: &str, data: &[u8]) -> Bech32Result<String> {
    bech32::encode(hrp, data.to_base32(), Variant::Bech32)
}

pub fn decode(s: &str) -> Bech32Result<Decoded> {
    let (hrp, base32_bytes, variant) = bech32::decode(s)?;
    let bytes = Data::from_base32(&base32_bytes)?;

    if matches!(variant, Variant::Bech32) {
        return Ok(Decoded { hrp, bytes });
    }
    Err(Bech32Error::InvalidChecksum)
}
