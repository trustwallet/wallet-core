// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use serde_json::Value as Json;
use tw_coin_entry::error::SigningResult;
use tw_proto::google;

pub mod standard_cosmos_message;

pub type ProtobufMessage = google::protobuf::Any;

pub type SerializeMessageBox = Box<dyn SerializeMessage>;

pub trait ToJsonMessage {
    fn message_type(&self) -> String;

    fn to_json(&self) -> SigningResult<Json>;
}

pub trait ToProtobufMessage {
    fn to_proto(&self) -> ProtobufMessage;
}

pub trait SerializeMessage: ToJsonMessage + ToProtobufMessage {
    fn into_boxed(self) -> SerializeMessageBox
    where
        Self: 'static + Sized,
    {
        Box::new(self)
    }
}

impl<T> SerializeMessage for T where T: ToJsonMessage + ToProtobufMessage {}
