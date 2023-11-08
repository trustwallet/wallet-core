// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::address::CosmosAddress;
use crate::proto::cosmos;
use crate::transaction::message::{CosmosMessage, ProtobufMessage};
use tw_coin_entry::error::SigningResult;
use tw_proto::{google, to_any};

/// Any raw JSON message.
/// Supports JSON serialization only.
pub struct AuthGrantMessage<Address: CosmosAddress> {
    pub granter: Address,
    pub grantee: Address,
    pub grant_msg: google::protobuf::Any,
    pub expiration_secs: i64,
}

impl<Address: CosmosAddress> CosmosMessage for AuthGrantMessage<Address> {
    fn to_proto(&self) -> SigningResult<ProtobufMessage> {
        let expiration = google::protobuf::Timestamp {
            seconds: self.expiration_secs,
            ..google::protobuf::Timestamp::default()
        };
        let grant = cosmos::authz::v1beta1::Grant {
            authorization: Some(self.grant_msg.clone()),
            expiration: Some(expiration),
        };

        let proto_msg = cosmos::authz::v1beta1::MsgGrant {
            granter: self.granter.to_string(),
            grantee: self.grantee.to_string(),
            grant: Some(grant),
        };
        Ok(to_any(&proto_msg))
    }
}
