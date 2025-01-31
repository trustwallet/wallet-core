// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::encode::serializer::{Encodable, Encoder};
use std::str::FromStr;
use tw_coin_entry::error::prelude::{
    MapTWError, ResultContext, SigningError, SigningErrorType, SigningResult,
};
use tw_encoding::hex::DecodeHex;
use tw_memory::Data;

pub struct Blob(Data);

impl Encodable for Blob {
    fn encode(&self, dst: &mut Encoder) -> SigningResult<()> {
        dst.append_raw_slice(&self.0);
        Ok(())
    }
}

impl FromStr for Blob {
    type Err = SigningError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        s.decode_hex()
            .map(Blob)
            .tw_err(SigningErrorType::Error_input_parse)
            .context("Error parsing Blob")
    }
}
