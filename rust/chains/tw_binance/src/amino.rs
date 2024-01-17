// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use quick_protobuf::MessageWrite;
use tw_encoding::{EncodingError, EncodingResult};
use tw_memory::Data;
use tw_proto::serialize;

pub struct AminoEncoder {
    /// The Amino content starts with a prefix.
    content: Data,
}

impl AminoEncoder {
    pub fn new(prefix: &[u8]) -> AminoEncoder {
        AminoEncoder {
            content: prefix.to_vec(),
        }
    }

    pub fn extend_content(mut self, content: &[u8]) -> AminoEncoder {
        self.content.extend_from_slice(content);
        self
    }

    pub fn extend_with_msg<M: MessageWrite>(mut self, msg: &M) -> EncodingResult<AminoEncoder> {
        let msg_data = serialize(msg).map_err(|_| EncodingError::Internal)?;
        self.content.extend_from_slice(&msg_data);
        Ok(self)
    }

    pub fn encode(self) -> Data {
        self.content
    }

    pub fn encode_size_prefixed(self) -> EncodingResult<Data> {
        const CONTENT_SIZE_CAPACITY: usize = 10;

        let content_len = self.content.len();
        let capacity = content_len + CONTENT_SIZE_CAPACITY;

        let mut buffer = Vec::with_capacity(capacity);

        Self::write_varint(&mut buffer, content_len as u64)?;
        buffer.extend_from_slice(&self.content);

        Ok(buffer)
    }

    /// This method takes `&mut Data` instead of `&mut [u8]` because the given `buffer` can be extended (become longer).
    fn write_varint(buffer: &mut Data, num: u64) -> EncodingResult<()> {
        let mut writer = quick_protobuf::Writer::new(buffer);
        writer
            .write_varint(num)
            .map_err(|_| EncodingError::Internal)
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use tw_encoding::hex::DecodeHex;

    struct TestInput {
        prefix: &'static str,
        content: &'static str,
        content_size_prefixed: bool,
        expected: &'static str,
    }

    fn amino_encode_impl(input: TestInput) {
        let prefix = input.prefix.decode_hex().unwrap();
        let content = input.content.decode_hex().unwrap();

        let encoder = AminoEncoder::new(&prefix).extend_content(&content);

        let actual = if input.content_size_prefixed {
            encoder
                .encode_size_prefixed()
                .expect("Error on Amino encoding with content size prefix")
        } else {
            encoder.encode()
        };

        let expected = input.expected.decode_hex().unwrap();
        assert_eq!(actual, expected);
    }

    #[test]
    fn test_amino_encode() {
        let content_size_prefixed = false;

        amino_encode_impl(TestInput {
            prefix: "0b0c0d0e",
            content: "0102030405060708",
            content_size_prefixed,
            expected: "0b0c0d0e0102030405060708",
        });
        amino_encode_impl(TestInput {
            prefix: "0b0c0d0e",
            content: "01020304050607080102030405060708010203040506070801020304050607080102030405060708",
            content_size_prefixed,
            expected: "0b0c0d0e01020304050607080102030405060708010203040506070801020304050607080102030405060708",
        });
    }

    #[test]
    fn test_amino_encode_with_content_size_prefix() {
        let content_size_prefixed = true;

        amino_encode_impl(TestInput {
            prefix: "0b0c0d0e",
            content: "0102030405060708",
            content_size_prefixed,
            expected: "0c0b0c0d0e0102030405060708",
        });
        amino_encode_impl(TestInput {
            prefix: "0b0c0d0e0f101112131415161718191a1b1c1d1e0b0c0d0e0f101112131415161718191a1b1c1d1e0b0c0d0e0f101112131415161718191a1b1c1d1e0b0c0d0e0f101112131415161718191a1b1c1d1e0b0c0d0e0f101112131415161718191a1b1c1d1e0b0c0d0e0f101112131415161718191a1b1c1d1e0b0c0d0e0f101112131415161718191a1b1c1d1e0b0c0d0e0f101112131415161718191a1b1c1d1e0b0c0d0e0f101112131415161718191a1b1c1d1e0b0c0d0e0f101112131415161718191a1b1c1d1e0b0c0d0e0f101112131415161718191a1b1c1d1e0b0c0d0e0f101112131415161718191a1b1c1d1e0b0c0d0e0f101112131415161718191a1b1c1d1e0b0c0d0e0f101112131415161718191a1b1c1d1e0b0c0d0e0f101112131415161718191a1b1c1d1e0b0c0d0e0f101112131415161718191a1b1c1d1e0b0c0d0e0f101112131415161718191a1b1c1d1e",
            content: "0102030405060708",
            content_size_prefixed,
            expected: "dc020b0c0d0e0f101112131415161718191a1b1c1d1e0b0c0d0e0f101112131415161718191a1b1c1d1e0b0c0d0e0f101112131415161718191a1b1c1d1e0b0c0d0e0f101112131415161718191a1b1c1d1e0b0c0d0e0f101112131415161718191a1b1c1d1e0b0c0d0e0f101112131415161718191a1b1c1d1e0b0c0d0e0f101112131415161718191a1b1c1d1e0b0c0d0e0f101112131415161718191a1b1c1d1e0b0c0d0e0f101112131415161718191a1b1c1d1e0b0c0d0e0f101112131415161718191a1b1c1d1e0b0c0d0e0f101112131415161718191a1b1c1d1e0b0c0d0e0f101112131415161718191a1b1c1d1e0b0c0d0e0f101112131415161718191a1b1c1d1e0b0c0d0e0f101112131415161718191a1b1c1d1e0b0c0d0e0f101112131415161718191a1b1c1d1e0b0c0d0e0f101112131415161718191a1b1c1d1e0b0c0d0e0f101112131415161718191a1b1c1d1e0102030405060708",
        });
    }
}
