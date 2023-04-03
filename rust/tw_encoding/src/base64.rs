// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::{EncodingError, EncodingResult};
use base64::{engine::general_purpose, DecodeError, Engine as _};

impl From<DecodeError> for EncodingError {
    fn from(_: DecodeError) -> Self {
        EncodingError::InvalidInput
    }
}

pub fn encode(data: &[u8], is_url: bool) -> String {
    if is_url {
        general_purpose::URL_SAFE.encode(data)
    } else {
        general_purpose::STANDARD.encode(data)
    }
}

pub fn decode(data: &str, is_url: bool) -> EncodingResult<Vec<u8>> {
    if is_url {
        general_purpose::URL_SAFE.decode(data)
    } else {
        general_purpose::STANDARD.decode(data)
    }
    .map_err(EncodingError::from)
}
