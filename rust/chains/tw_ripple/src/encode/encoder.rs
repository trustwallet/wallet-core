// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::encode::field_instance::FieldInstance;
use crate::encode::Encodable;
use std::io::Write;
use tw_coin_entry::error::prelude::*;
use tw_memory::Data;

const MAX_SINGLE_BYTE_LENGTH: usize = 192;
const MAX_DOUBLE_BYTE_LENGTH: usize = 12_481;
const MAX_LENGTH_VALUE: usize = 918_744;

const MAX_SECOND_BYTE_VALUE: usize = 240;

#[derive(Default)]
pub struct Encoder {
    buffer: Data,
}

impl Encoder {
    pub fn finish(self) -> Data {
        self.buffer
    }

    /// Appends raw bytes to the end of the stream.
    pub fn append_raw_slice(&mut self, bytes: &[u8]) {
        // Discard error for now, since we write simple vectors.
        self.buffer.write_all(bytes).unwrap();
    }

    pub fn push_byte(&mut self, byte: u8) {
        self.buffer.push(byte);
    }

    pub fn append_field_and_value<T: Encodable>(
        &mut self,
        field_instance: &FieldInstance,
        value: &T,
    ) -> SigningResult<()> {
        // Encode field header first.
        field_instance.field_header().encode(self)?;

        if field_instance.is_vl_encoded {
            self.append_length_encoded(value)
        } else {
            value.encode(self)
        }
    }

    fn append_length_encoded<T: Encodable>(&mut self, value: &T) -> SigningResult<()> {
        let mut encoder = Encoder::default();
        value.encode(&mut encoder)?;
        let encoded = encoder.finish();

        self.encode_variable_length(encoded.len())?;
        self.append_raw_slice(&encoded);
        Ok(())
    }

    /// The length of the prefix is 1-3 bytes depending on the length of the contents:
    /// Content length <= 192 bytes: prefix is 1 byte
    /// 192 bytes < Content length <= 12480 bytes: prefix is 2 bytes
    /// 12480 bytes < Content length <= 918744 bytes: prefix is 3 bytes
    ///
    /// See Length Prefixing https://xrpl.org/serialization.html#length-prefixing
    fn encode_variable_length(&mut self, len: usize) -> SigningResult<()> {
        fn try_into_u8(num: usize) -> SigningResult<u8> {
            u8::try_from(num)
                .tw_err(SigningErrorType::Error_internal)
                .context("Error encoding a variable length")
        }

        if len <= MAX_SINGLE_BYTE_LENGTH {
            self.push_byte(len as u8);
            return Ok(());
        }

        if len < MAX_DOUBLE_BYTE_LENGTH {
            let shifted_len = len - MAX_SINGLE_BYTE_LENGTH - 1;
            let byte_1 = (shifted_len >> 8) + MAX_SINGLE_BYTE_LENGTH + 1;
            let byte_2 = shifted_len & 0xFF;

            self.push_byte(try_into_u8(byte_1)?);
            self.push_byte(try_into_u8(byte_2)?);
            return Ok(());
        }

        if len <= MAX_LENGTH_VALUE {
            let shifted_len = len - MAX_DOUBLE_BYTE_LENGTH;
            let byte_1 = (MAX_SECOND_BYTE_VALUE + 1) + (shifted_len >> 16);
            let byte_2 = (shifted_len >> 8) & 0xFF;
            let byte_3 = shifted_len & 0xFF;

            self.push_byte(try_into_u8(byte_1)?);
            self.push_byte(try_into_u8(byte_2)?);
            self.push_byte(try_into_u8(byte_3)?);

            return Ok(());
        }

        SigningError::err(SigningErrorType::Error_tx_too_big).context(format!(
            "Invalid variable length too large: max: {MAX_LENGTH_VALUE}"
        ))
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use tw_encoding::hex::ToHex;

    fn test_encode_variable_length_impl(len: usize, expected: &str) {
        let mut encoder = Encoder::default();
        encoder.encode_variable_length(len).unwrap();
        assert_eq!(encoder.finish().to_hex(), expected);
    }

    /// Tested with [xrpl-py](https://github.com/XRPLF/xrpl-py).
    #[test]
    fn test_encode_variable_length() {
        test_encode_variable_length_impl(180, "b4");
        test_encode_variable_length_impl(1000, "c427");
        test_encode_variable_length_impl(12080, "ef6f");
        test_encode_variable_length_impl(12580, "f10063");
        test_encode_variable_length_impl(20000, "f11d5f");
        test_encode_variable_length_impl(110000, "f27cef");
        test_encode_variable_length_impl(918744, "fed417");
    }
}
