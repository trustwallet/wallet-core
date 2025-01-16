// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::CosmosAddress;
use crate::proto::cosmos;
use crate::transaction::message::{CosmosMessage, ProtobufMessage};
use tw_coin_entry::error::prelude::*;
use tw_proto::{google, to_any};

/// Supports Protobuf serialization only.
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

/// Supports Protobuf serialization only.
pub struct AuthRevokeMessage<Address: CosmosAddress> {
    pub granter: Address,
    pub grantee: Address,
    pub msg_type_url: String,
}

impl<Address: CosmosAddress> CosmosMessage for AuthRevokeMessage<Address> {
    fn to_proto(&self) -> SigningResult<ProtobufMessage> {
        let proto_msg = cosmos::authz::v1beta1::MsgRevoke {
            granter: self.granter.to_string(),
            grantee: self.grantee.to_string(),
            msg_type_url: self.msg_type_url.clone(),
        };
        Ok(to_any(&proto_msg))
    }
}
