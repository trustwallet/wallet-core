use move_core_types::language_storage::TypeTag;
use serde::{Deserialize, Serialize};
use std::str::FromStr;
use tw_coin_entry::error::prelude::SigningError;
use tw_memory::Data;
use tw_misc::serde::as_string;

use crate::address::SuiAddress;

use super::{
    command::Argument,
    sui_types::{ObjectArg, ObjectID, SequenceNumber},
};

#[derive(Debug, Deserialize, Serialize)]
#[serde(rename_all = "camelCase")]
pub struct PaymentConfig {
    pub object_id: String,
    #[serde(with = "as_string")]
    pub version: u64,
    pub digest: String,
}

#[derive(Debug, Deserialize, Serialize)]
pub struct GasConfig {
    #[serde(with = "as_string")]
    pub budget: u64,
    #[serde(with = "as_string")]
    pub price: u64,
    pub payment: Vec<PaymentConfig>,
}

#[derive(Debug, Deserialize, Serialize)]
pub enum InputObjectArg {
    #[serde(rename_all = "camelCase")]
    Shared {
        mutable: bool,
        #[serde(with = "as_string")]
        initial_shared_version: u64,
        object_id: String,
    },
}

impl TryFrom<InputObjectArg> for ObjectArg {
    type Error = SigningError;

    fn try_from(arg: InputObjectArg) -> Result<Self, Self::Error> {
        match arg {
            InputObjectArg::Shared {
                mutable,
                initial_shared_version,
                object_id,
            } => Ok(ObjectArg::SharedObject {
                id: ObjectID::from_str(&object_id)?,
                initial_shared_version: SequenceNumber(initial_shared_version),
                mutable,
            }),
        }
    }
}

#[derive(Debug, Deserialize, Serialize)]
pub enum InputArg {
    Pure(Data),
    Object(InputObjectArg),
}

#[derive(Debug, Deserialize, Serialize)]
#[serde(rename_all = "camelCase")]
pub struct Input {
    pub kind: String,
    pub index: u16,
    #[serde(rename = "type")]
    pub input_type: String,
    pub value: InputArg,
}

#[derive(Debug, Deserialize, Serialize)]
#[serde(tag = "kind")]
pub enum TransactionArg {
    GasCoin,
    Input { index: u16 },
    Result { index: u16 },
}

impl From<TransactionArg> for Argument {
    fn from(arg: TransactionArg) -> Self {
        match arg {
            TransactionArg::GasCoin => Argument::GasCoin,
            TransactionArg::Input { index } => Argument::Input(index),
            TransactionArg::Result { index } => Argument::Result(index),
        }
    }
}

#[derive(Debug, Deserialize, Serialize, Clone)]
pub struct TypeTagWrapper(#[serde(with = "as_string")] TypeTag);

impl From<TypeTagWrapper> for TypeTag {
    fn from(value: TypeTagWrapper) -> Self {
        value.0
    }
}

#[derive(Debug, Deserialize, Serialize)]
#[serde(tag = "kind")]
pub enum Transaction {
    SplitCoins {
        coin: TransactionArg,
        amounts: Vec<TransactionArg>,
    },
    #[serde(rename_all = "camelCase")]
    MoveCall {
        target: String,
        type_arguments: Vec<TypeTagWrapper>,
        arguments: Vec<TransactionArg>,
    },
    TransferObjects {
        objects: Vec<TransactionArg>,
        address: TransactionArg,
    },
}

#[derive(Debug, Deserialize, Serialize)]
#[serde(rename_all = "camelCase")]
pub struct RawTransaction {
    pub version: u8,
    pub sender: SuiAddress,
    pub expiration: Option<u64>,
    pub gas_config: GasConfig,
    pub inputs: Vec<Input>,
    pub transactions: Vec<Transaction>,
}
