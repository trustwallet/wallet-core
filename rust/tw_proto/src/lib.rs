// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use quick_protobuf::{BytesReader, MessageInfo, Writer};

#[allow(non_snake_case)]
#[rustfmt::skip]
mod common;

#[allow(non_snake_case)]
#[rustfmt::skip]
mod generated {
    include!(concat!(env!("OUT_DIR"), "/proto/mod.rs"));
}

pub use common::google;
pub use generated::TW::*;
pub use quick_protobuf::{
    deserialize_from_slice as deserialize_prefixed, serialize_into_vec as serialize_prefixed,
    Error as ProtoError, MessageRead, MessageWrite, Result as ProtoResult,
};

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

pub fn to_any<T>(message: &T) -> google::protobuf::Any
where
    T: MessageInfo + MessageWrite,
{
    let value = serialize(message).expect("Protobuf serialization should never fail");
    let type_url = type_url::<T>();
    google::protobuf::Any { type_url, value }
}

pub fn to_any_with_type_url<T>(message: &T, type_url: String) -> google::protobuf::Any
where
    T: MessageInfo + MessageWrite,
{
    let value = serialize(message).expect("Protobuf serialization should never fail");
    google::protobuf::Any { type_url, value }
}

pub fn type_url<T: MessageInfo>() -> String {
    format!("/{}", T::PATH)
}

/// There is no way to create an instance of the `NoMessage` enum as it doesn't has variants.
pub enum NoMessage {}

impl MessageWrite for NoMessage {}

/// `DummyMessage` has no effect on `MessageWrite` and `MessageRead`.
pub struct DummyMessage;

impl MessageWrite for DummyMessage {}

impl<'a> MessageRead<'a> for DummyMessage {
    fn from_reader(_r: &mut BytesReader, _bytes: &'a [u8]) -> ProtoResult<Self> {
        Ok(DummyMessage)
    }
}
