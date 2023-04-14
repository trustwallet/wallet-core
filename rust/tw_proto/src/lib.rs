// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use quick_protobuf::{BytesReader, MessageRead, MessageWrite, Writer};

#[allow(non_snake_case)]
#[rustfmt::skip]
mod generated {
    include!(concat!(env!("OUT_DIR"), "/proto/mod.rs"));
}

pub use generated::TW::*;
pub use quick_protobuf::{
    deserialize_from_slice as deserialize_prefixed, serialize_into_vec as serialize_prefixed,
    Error as ProtoError, Result as ProtoResult,
};

pub mod ffi;

/// Serializes a Protobuf message without the length prefix.
/// Please note that [`quick_protobuf::serialize_into_vec`] appends a `varint32` length prefix.
pub fn serialize<T: MessageWrite>(message: &T) -> ProtoResult<Vec<u8>> {
    let len = message.get_size();
    let mut v = Vec::with_capacity(quick_protobuf::sizeofs::sizeof_len(len));
    let mut writer = Writer::new(&mut v);
    message.write_message(&mut writer)?;
    Ok(v)
}

/// Deserializes a Protobuf message from the given `data` bytes without the length prefix.
/// Please note that [`quick_protobuf::deserialize_from_slice`] requires the data
/// starts from a `varint32` length prefix.
pub fn deserialize<'a, T: MessageRead<'a>>(data: &'a [u8]) -> ProtoResult<T> {
    let mut reader = BytesReader::from_bytes(data);
    T::from_reader(&mut reader, data)
}
