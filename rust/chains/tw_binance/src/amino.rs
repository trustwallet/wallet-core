// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use quick_protobuf::MessageWrite;
use tw_encoding::{EncodingError, EncodingResult};
use tw_memory::Data;
use tw_proto::serialize;

pub struct Amino;

impl Amino {
    pub fn encode(prefix: &[u8], content: &[u8]) -> Data {
        let capacity = prefix.len() + content.len();
        let mut buffer = Vec::with_capacity(capacity);

        buffer.extend_from_slice(prefix);
        buffer.extend_from_slice(content);

        buffer
    }

    pub fn encode_proto<T: MessageWrite>(prefix: &[u8], msg: &T) -> EncodingResult<Data> {
        let content = serialize(msg).map_err(|_| EncodingError::Internal)?;
        Ok(Amino::encode(prefix, &content))
    }

    pub fn encode_with_content_size_prefix(prefix: &[u8], content: &[u8]) -> EncodingResult<Data> {
        const CONTENT_SIZE_CAPACITY: usize = 10;

        let content_len = prefix.len() + content.len();
        let capacity = content_len + CONTENT_SIZE_CAPACITY;

        let mut buffer = Vec::with_capacity(capacity);

        Self::write_varint(&mut buffer, content_len as u64)?;
        buffer.extend_from_slice(prefix);
        buffer.extend_from_slice(content);

        Ok(buffer)
    }

    pub fn encode_proto_with_content_size_prefix<T: MessageWrite>(
        prefix: &[u8],
        msg: &T,
    ) -> EncodingResult<Data> {
        let content = serialize(msg).map_err(|_| EncodingError::Internal)?;
        Amino::encode_with_content_size_prefix(prefix, &content)
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

        let actual = if input.content_size_prefixed {
            Amino::encode_with_content_size_prefix(&prefix, &content)
                .expect("Error on Amino encoding with content size prefix")
        } else {
            Amino::encode(&prefix, &content)
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
