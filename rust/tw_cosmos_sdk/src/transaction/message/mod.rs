// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_proto::google;

pub mod standard_cosmos_message;

pub type ProtobufMessage = google::protobuf::Any;

pub type SerializeMessageBox = Box<dyn SerializeMessage>;

pub trait SerializeMessage {
    fn into_boxed(self) -> SerializeMessageBox
    where
        Self: 'static + Sized,
    {
        Box::new(self)
    }

    fn to_proto(&self) -> ProtobufMessage;

    // fn to_json(&self) -> Json;
}
