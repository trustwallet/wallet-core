// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::address::GreenfieldAddress;
use core::fmt;
use serde::{Serialize, Serializer};
use serde_json::Value as Json;
use std::collections::BTreeMap;
use tw_cosmos_sdk::transaction::message::JsonMessage;
use tw_cosmos_sdk::transaction::{Coin, Fee};
use tw_evm::abi::param_type::constructor::TypeConstructor;
use tw_evm::message::eip712::message_types::MessageTypesBuilder;
use tw_evm::message::eip712::property::PropertyType;
use tw_number::U256;

const DOMAIN_NAME: &str = "Greenfield Tx";
const DOMAIN_VERSION: &str = "1.0.0";
const DOMAIN_VERIFY_CONTRACT: &str = "greenfield";
const DOMAIN_SALT: &str = "0";

#[derive(Debug, Eq, Ord, PartialEq, PartialOrd)]
pub struct MsgPropertyName(pub usize);

impl fmt::Display for MsgPropertyName {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "msg{}", self.0)
    }
}

pub struct MsgPropertyType(pub usize);

impl fmt::Display for MsgPropertyType {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "Msg{}", self.0)
    }
}

impl Serialize for MsgPropertyName {
    fn serialize<S>(&self, serializer: S) -> Result<S::Ok, S::Error>
    where
        S: Serializer,
    {
        self.to_string().serialize(serializer)
    }
}

#[derive(Serialize)]
#[serde(rename_all = "camelCase")]
pub struct GreenfieldDomain {
    pub name: String,
    pub version: String,
    #[serde(serialize_with = "U256::as_decimal_str")]
    pub chain_id: U256,
    pub verifying_contract: String,
    pub salt: String,
}

impl GreenfieldDomain {
    const TYPE_NAME: &'static str = "EIP712Domain";

    /// Returns a Serializable data of the `EIP712Domain` type.
    /// https://github.com/bnb-chain/greenfield-cosmos-sdk/blob/b48770f5e210b28536f92734b6228913666d4da1/x/auth/tx/eip712.go#L35-L40
    pub fn new(chain_id: U256) -> GreenfieldDomain {
        GreenfieldDomain {
            name: DOMAIN_NAME.to_string(),
            version: DOMAIN_VERSION.to_string(),
            chain_id,
            verifying_contract: DOMAIN_VERIFY_CONTRACT.to_string(),
            salt: DOMAIN_SALT.to_string(),
        }
    }

    pub fn declare_eip712_types(&self, builder: &mut MessageTypesBuilder) {
        if let Some(mut domain_builder) = builder.add_custom_type(Self::TYPE_NAME.to_string()) {
            domain_builder
                .add_property("chainId", PropertyType::Uint)
                .add_property("name", PropertyType::String)
                .add_property("salt", PropertyType::String)
                .add_property("verifyingContract", PropertyType::String)
                .add_property("version", PropertyType::String);
        }
    }
}

pub struct GreenfieldCoin;

impl GreenfieldCoin {
    const TYPE_NAME: &'static str = "Coin";

    pub fn declare_eip712_types(builder: &mut MessageTypesBuilder) {
        if let Some(mut coin_builder) = builder.add_custom_type(Self::TYPE_NAME.to_string()) {
            coin_builder
                .add_property("amount", PropertyType::Uint)
                .add_property("denom", PropertyType::String);
        }
    }
}

#[derive(Serialize)]
pub struct GreenfieldFee {
    pub amount: Vec<Coin>,
    #[serde(serialize_with = "U256::as_decimal_str")]
    pub gas_limit: U256,
    pub payer: String,
    pub granter: String,
}

impl GreenfieldFee {
    const TYPE_NAME: &'static str = "Fee";

    pub fn declare_eip712_types(builder: &mut MessageTypesBuilder) {
        // `Tx` depends on `Coin` and `Fee` custom types.
        GreenfieldCoin::declare_eip712_types(builder);

        if let Some(mut fee_builder) = builder.add_custom_type(Self::TYPE_NAME.to_string()) {
            let amount_type = PropertyType::Custom(GreenfieldCoin::TYPE_NAME.to_string());
            fee_builder
                .add_property("amount", PropertyType::array(amount_type))
                .add_property("gas_limit", PropertyType::Uint)
                .add_property("granter", PropertyType::String)
                .add_property("payer", PropertyType::String);
        }
    }
}

/// TODO rename `GreenfieldFee` to `Eip712Fee`.
/// TODO rename `Fee<GreenfieldAddress>` to `GreenfieldFee`
impl From<Fee<GreenfieldAddress>> for GreenfieldFee {
    fn from(fee: Fee<GreenfieldAddress>) -> Self {
        let payer = fee
            .payer
            .as_ref()
            .map(|addr| addr.to_string())
            .unwrap_or_default();
        let granter = fee
            .granter
            .as_ref()
            .map(|addr| addr.to_string())
            .unwrap_or_default();

        GreenfieldFee {
            amount: fee.amounts.clone(),
            gas_limit: U256::from(fee.gas_limit),
            payer,
            granter,
        }
    }
}

#[derive(Clone, Serialize)]
pub struct GreenfieldTypedMsg {
    #[serde(rename = "type")]
    pub msg_type: String,
    #[serde(flatten)]
    pub value: Json,
}

impl From<JsonMessage> for GreenfieldTypedMsg {
    fn from(msg: JsonMessage) -> Self {
        GreenfieldTypedMsg {
            msg_type: msg.msg_type,
            value: msg.value,
        }
    }
}

#[derive(Serialize)]
pub struct GreenfieldTransaction {
    #[serde(serialize_with = "U256::as_decimal_str")]
    pub account_number: U256,
    #[serde(serialize_with = "U256::as_decimal_str")]
    pub chain_id: U256,
    pub fee: GreenfieldFee,
    pub memo: String,
    /// Will be flatten as `"msg1": { ... }, "msg2": { ... }`.
    #[serde(flatten)]
    pub msgs: BTreeMap<MsgPropertyName, GreenfieldTypedMsg>,
    #[serde(serialize_with = "U256::as_decimal_str")]
    pub sequence: U256,
    #[serde(serialize_with = "U256::as_decimal_str")]
    pub timeout_height: U256,
}

impl GreenfieldTransaction {
    /// cbindgen::ignore
    pub const TYPE_NAME: &'static str = "Tx";

    pub fn declare_eip712_types(&self, builder: &mut MessageTypesBuilder) {
        GreenfieldFee::declare_eip712_types(builder);

        let Some(mut tx_builder) = builder.add_custom_type(Self::TYPE_NAME.to_string()) else {
            return;
        };

        tx_builder
            .add_property("account_number", PropertyType::Uint)
            .add_property("chain_id", PropertyType::Uint)
            .add_property(
                "fee",
                PropertyType::Custom(GreenfieldFee::TYPE_NAME.to_string()),
            )
            .add_property("memo", PropertyType::String)
            .add_property("sequence", PropertyType::Uint)
            .add_property("timeout_height", PropertyType::Uint);

        for (msg_property_name, _msg) in self.msgs.iter() {
            let msg_property_type = MsgPropertyType(msg_property_name.0).to_string();
            let msg_property_type = PropertyType::Custom(msg_property_type.to_string());

            let msg_property_name = msg_property_name.to_string();
            tx_builder.add_property(&msg_property_name, msg_property_type);
        }

        tx_builder.sort_by_names();
    }
}
