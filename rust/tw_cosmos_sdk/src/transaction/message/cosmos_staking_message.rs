// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::CosmosAddress;
use crate::modules::serializer::protobuf_serializer::build_coin;
use crate::proto::cosmos;
use crate::transaction::message::{message_to_json, CosmosMessage, JsonMessage, ProtobufMessage};
use crate::transaction::Coin;
use serde::Serialize;
use tw_coin_entry::error::prelude::*;
use tw_proto::to_any;

const DEFAULT_JSON_SET_WITHDRAW_ADDRESS_TYPE: &str = "cosmos-sdk/MsgSetWithdrawAddress";
const DEFAULT_JSON_WITHDRAW_REWARDS_TYPE: &str = "cosmos-sdk/MsgWithdrawDelegationReward";
const DEFAULT_JSON_BEGIN_REDELEGATE_TYPE: &str = "cosmos-sdk/MsgBeginRedelegate";
const DEFAULT_JSON_UNDELEGATE_TYPE: &str = "cosmos-sdk/MsgUndelegate";
const DEFAULT_JSON_DELEGATE_TYPE: &str = "cosmos-sdk/MsgDelegate";

/// cosmos-sdk/MsgDelegate
#[derive(Serialize)]
pub struct DelegateMessage<Address: CosmosAddress> {
    #[serde(skip)]
    pub custom_type_prefix: Option<String>,
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
        let msg_type = self
            .custom_type_prefix
            .as_deref()
            .unwrap_or(DEFAULT_JSON_DELEGATE_TYPE);
        message_to_json(msg_type, self)
    }
}

/// cosmos-sdk/MsgUndelegate
#[derive(Serialize)]
pub struct UndelegateMessage<Address: CosmosAddress> {
    #[serde(skip)]
    pub custom_type_prefix: Option<String>,
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
        let msg_type = self
            .custom_type_prefix
            .as_deref()
            .unwrap_or(DEFAULT_JSON_UNDELEGATE_TYPE);
        message_to_json(msg_type, self)
    }
}

/// cosmos-sdk/MsgBeginRedelegate
#[derive(Serialize)]
pub struct BeginRedelegateMessage<Address: CosmosAddress> {
    #[serde(skip)]
    pub custom_type_prefix: Option<String>,
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
        let msg_type = self
            .custom_type_prefix
            .as_deref()
            .unwrap_or(DEFAULT_JSON_BEGIN_REDELEGATE_TYPE);
        message_to_json(msg_type, self)
    }
}

/// cosmos-sdk/MsgWithdrawDelegationReward
#[derive(Serialize)]
pub struct WithdrawDelegationRewardMessage<Address: CosmosAddress> {
    #[serde(skip)]
    pub custom_type_prefix: Option<String>,
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
        let msg_type = self
            .custom_type_prefix
            .as_deref()
            .unwrap_or(DEFAULT_JSON_WITHDRAW_REWARDS_TYPE);
        message_to_json(msg_type, self)
    }
}

/// cosmos-sdk/MsgSetWithdrawAddress
#[derive(Serialize)]
pub struct SetWithdrawAddressMessage<Address: CosmosAddress> {
    #[serde(skip)]
    pub custom_type_prefix: Option<String>,
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
        let msg_type = self
            .custom_type_prefix
            .as_deref()
            .unwrap_or(DEFAULT_JSON_SET_WITHDRAW_ADDRESS_TYPE);
        message_to_json(msg_type, self)
    }
}
