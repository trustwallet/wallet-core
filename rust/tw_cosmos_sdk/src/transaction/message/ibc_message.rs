// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::CosmosAddress;
use crate::modules::serializer::protobuf_serializer::build_coin;
use crate::proto::ibc;
use crate::transaction::message::{CosmosMessage, ProtobufMessage};
use crate::transaction::Coin;
use tw_coin_entry::error::SigningResult;
use tw_proto::to_any;

pub struct Height {
    pub revision_number: u64,
    pub revision_height: u64,
}

pub struct TransferTokensMessage<Address: CosmosAddress> {
    /// IBC port, e.g. "transfer".
    pub source_port: String,
    /// IBC connection channel, e.g. "channel-141", see apis /ibc/applications/transfer/v1beta1/denom_traces (connections) or /node_info (own channel).
    pub source_channel: String,
    pub token: Coin,
    pub sender: Address,
    pub receiver: Address,
    /// Timeout block height. Either timeout height or timestamp should be set.
    /// Recommendation is to set height, to rev. 1 and block current + 1000 (see api /blocks/latest).
    pub timeout_height: Height,
    // Timeout timestamp (in nanoseconds) relative to the current block timestamp.  Either timeout height or timestamp should be set.
    pub timeout_timestamp: u64,
}

impl<Address: CosmosAddress> CosmosMessage for TransferTokensMessage<Address> {
    fn to_proto(&self) -> SigningResult<ProtobufMessage> {
        let height = ibc::core::client::v1::Height {
            revision_number: self.timeout_height.revision_number,
            revision_height: self.timeout_height.revision_height,
        };

        let proto_msg = ibc::applications::transfer::v1::MsgTransfer {
            source_port: self.source_port.clone(),
            source_channel: self.source_channel.clone(),
            token: Some(build_coin(&self.token)),
            sender: self.sender.to_string(),
            receiver: self.receiver.to_string(),
            timeout_height: Some(height),
            timeout_timestamp: self.timeout_timestamp,
        };
        Ok(to_any(&proto_msg))
    }
}
