// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::address::CosmosAddress;
use crate::modules::serializer::protobuf_serializer::build_coin;
use crate::proto::cosmos;
use crate::transaction::message::{message_to_json, CosmosMessage, JsonMessage, ProtobufMessage};
use crate::transaction::Coin;
use serde::Serialize;
use serde_json::Value as Json;
use tw_coin_entry::error::SigningResult;
use tw_proto::to_any;

// cosmos-sdk/MsgSend
#[derive(Serialize)]
pub struct SendMessage<Address: CosmosAddress> {
    pub from_address: Address,
    pub to_address: Address,
    pub amount: Vec<Coin>,
}

impl<Address: CosmosAddress> CosmosMessage for SendMessage<Address> {
    fn to_proto(&self) -> SigningResult<ProtobufMessage> {
        let proto_msg = cosmos::bank::v1beta1::MsgSend {
            from_address: self.from_address.to_string(),
            to_address: self.to_address.to_string(),
            amount: self.amount.iter().map(build_coin).collect(),
        };
        Ok(to_any(&proto_msg))
    }

    fn to_json(&self) -> SigningResult<JsonMessage> {
        message_to_json("cosmos-sdk/MsgSend", self)
    }
}

// cosmos-sdk/MsgDelegate
#[derive(Serialize)]
pub struct DelegateMessage<Address: CosmosAddress> {
    pub amount: Coin,
    pub delegator_address: Address,
    pub validator_address: Address,
}

impl<Address: CosmosAddress> CosmosMessage for DelegateMessage<Address> {
    fn to_proto(&self) -> SigningResult<ProtobufMessage> {
        let proto_msg = cosmos::staking::v1beta1::MsgDelegate {
            amount: Some(build_coin(&self.amount)),
            delegator_address: self.delegator_address.to_string(),
            validator_address: self.validator_address.to_string(),
        };
        Ok(to_any(&proto_msg))
    }

    fn to_json(&self) -> SigningResult<JsonMessage> {
        message_to_json("cosmos-sdk/MsgDelegate", self)
    }
}

// cosmos-sdk/MsgUndelegate
#[derive(Serialize)]
pub struct UndelegateMessage<Address: CosmosAddress> {
    pub amount: Coin,
    pub delegator_address: Address,
    pub validator_address: Address,
}

impl<Address: CosmosAddress> CosmosMessage for UndelegateMessage<Address> {
    fn to_proto(&self) -> SigningResult<ProtobufMessage> {
        let proto_msg = cosmos::staking::v1beta1::MsgUndelegate {
            amount: Some(build_coin(&self.amount)),
            delegator_address: self.delegator_address.to_string(),
            validator_address: self.validator_address.to_string(),
        };
        Ok(to_any(&proto_msg))
    }

    fn to_json(&self) -> SigningResult<JsonMessage> {
        message_to_json("cosmos-sdk/MsgUndelegate", self)
    }
}

// cosmos-sdk/MsgBeginRedelegate
#[derive(Serialize)]
pub struct BeginRedelegateMessage<Address: CosmosAddress> {
    pub amount: Coin,
    pub delegator_address: Address,
    pub validator_src_address: Address,
    pub validator_dst_address: Address,
}

impl<Address: CosmosAddress> CosmosMessage for BeginRedelegateMessage<Address> {
    fn to_proto(&self) -> SigningResult<ProtobufMessage> {
        let proto_msg = cosmos::staking::v1beta1::MsgBeginRedelegate {
            amount: Some(build_coin(&self.amount)),
            delegator_address: self.delegator_address.to_string(),
            validator_src_address: self.validator_src_address.to_string(),
            validator_dst_address: self.validator_dst_address.to_string(),
        };
        Ok(to_any(&proto_msg))
    }

    fn to_json(&self) -> SigningResult<JsonMessage> {
        message_to_json("cosmos-sdk/MsgBeginRedelegate", self)
    }
}

// cosmos-sdk/MsgWithdrawDelegationReward
#[derive(Serialize)]
pub struct WithdrawDelegationRewardMessage<Address: CosmosAddress> {
    pub delegator_address: Address,
    pub validator_address: Address,
}

impl<Address: CosmosAddress> CosmosMessage for WithdrawDelegationRewardMessage<Address> {
    fn to_proto(&self) -> SigningResult<ProtobufMessage> {
        let proto_msg = cosmos::distribution::v1beta1::MsgWithdrawDelegatorReward {
            delegator_address: self.delegator_address.to_string(),
            validator_address: self.validator_address.to_string(),
        };
        Ok(to_any(&proto_msg))
    }

    fn to_json(&self) -> SigningResult<JsonMessage> {
        message_to_json("cosmos-sdk/MsgWithdrawDelegationReward", self)
    }
}

// cosmos-sdk/MsgSetWithdrawAddress
#[derive(Serialize)]
pub struct SetWithdrawAddressMessage<Address: CosmosAddress> {
    pub delegator_address: Address,
    pub withdraw_address: Address,
}

impl<Address: CosmosAddress> CosmosMessage for SetWithdrawAddressMessage<Address> {
    fn to_proto(&self) -> SigningResult<ProtobufMessage> {
        let proto_msg = cosmos::distribution::v1beta1::MsgSetWithdrawAddress {
            delegator_address: self.delegator_address.to_string(),
            withdraw_address: self.withdraw_address.to_string(),
        };
        Ok(to_any(&proto_msg))
    }

    fn to_json(&self) -> SigningResult<JsonMessage> {
        message_to_json("cosmos-sdk/MsgSetWithdrawAddress", self)
    }
}

/// Any raw JSON message.
pub struct JsonRawMessage {
    pub msg_type: String,
    pub value: Json,
}

impl CosmosMessage for JsonRawMessage {
    fn to_json(&self) -> SigningResult<JsonMessage> {
        Ok(JsonMessage {
            msg_type: self.msg_type.clone(),
            value: self.value.clone(),
        })
    }
}
