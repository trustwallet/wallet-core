// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::encode::encoder::Encoder;
use crate::encode::st_object::STObject;
use crate::encode::Encodable;
use serde::{Deserialize, Serialize};
use serde_json::Value as Json;
use tw_coin_entry::error::prelude::*;

const ARRAY_END_MARKER: u8 = 0xF1;

/// Class for serializing and deserializing Lists of objects.
///
/// See Array Fields:
/// `<https://xrpl.org/serialization.html#array-fields>`
#[derive(Debug, Deserialize, Serialize)]
pub struct STArray(Vec<Json>);

impl Encodable for STArray {
    fn encode(&self, dst: &mut Encoder) -> SigningResult<()> {
        let whether_all_objects = self.0.iter().all(|v| v.is_object());

        if !self.0.is_empty() && !whether_all_objects {
            return SigningError::err(SigningErrorType::Error_input_parse)
                .context("STArray is expected to be an array of objects");
        }

        for object in self.0.iter() {
            let signing_only = false;
            let serialized_object = STObject::try_from_value(object.clone(), signing_only)
                .context("Error parsing/serializing STArray")?;

            dst.append_raw_slice(&serialized_object.0);
        }

        dst.push_byte(ARRAY_END_MARKER);
        Ok(())
    }
}
