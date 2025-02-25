use move_core_types::identifier::Identifier;
use move_core_types::language_storage::TypeTag;
use serde::{Deserialize, Serialize};
use std::str::FromStr;
use tw_coin_entry::error::prelude::*;
use tw_memory::Data;
use tw_misc::serde::as_string;

use crate::address::SuiAddress;

use super::sui_types::TransactionExpiration;
use super::{
    command::{Argument, Command},
    sui_types::{CallArg, ObjectArg, ObjectDigest, ObjectID, ObjectRef, SequenceNumber},
};

#[derive(Debug, Deserialize, Serialize)]
#[serde(rename_all = "camelCase")]
pub struct PaymentConfig {
    pub object_id: String,
    #[serde(with = "as_string")]
    pub version: u64,
    pub digest: String,
}

impl TryFrom<PaymentConfig> for ObjectRef {
    type Error = SigningError;

    fn try_from(config: PaymentConfig) -> Result<Self, Self::Error> {
        Ok((
            ObjectID::from_str(&config.object_id)?,
            SequenceNumber(config.version),
            ObjectDigest::from_str(&config.digest)?,
        ))
    }
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
    ImmOrOwned {
        object_id: String,
        #[serde(with = "as_string")]
        version: u64,
        digest: String,
    },
    #[serde(rename_all = "camelCase")]
    Shared {
        mutable: bool,
        #[serde(with = "as_string")]
        initial_shared_version: u64,
        object_id: String,
    },
    #[serde(rename_all = "camelCase")]
    Receiving {
        digest: String,
        #[serde(with = "as_string")]
        version: u64,
        object_id: String,
    },
}

impl TryFrom<InputObjectArg> for ObjectArg {
    type Error = SigningError;

    fn try_from(arg: InputObjectArg) -> Result<Self, Self::Error> {
        match arg {
            InputObjectArg::ImmOrOwned {
                object_id,
                version,
                digest,
            } => Ok(ObjectArg::ImmOrOwnedObject((
                ObjectID::from_str(&object_id)?,
                SequenceNumber(version),
                ObjectDigest::from_str(&digest)?,
            ))),
            InputObjectArg::Shared {
                mutable,
                initial_shared_version,
                object_id,
            } => Ok(ObjectArg::SharedObject {
                id: ObjectID::from_str(&object_id)?,
                initial_shared_version: SequenceNumber(initial_shared_version),
                mutable,
            }),
            InputObjectArg::Receiving {
                digest,
                version,
                object_id,
            } => Ok(ObjectArg::Receiving((
                ObjectID::from_str(&object_id)?,
                SequenceNumber(version),
                ObjectDigest::from_str(&digest)?,
            ))),
        }
    }
}

#[derive(Debug, Deserialize, Serialize)]
pub enum InputArg {
    Pure(Data),
    Object(InputObjectArg),
}

impl TryFrom<InputArg> for CallArg {
    type Error = SigningError;

    fn try_from(arg: InputArg) -> Result<Self, Self::Error> {
        match arg {
            InputArg::Pure(data) => Ok(CallArg::Pure(data)),
            InputArg::Object(object) => Ok(CallArg::Object(object.try_into()?)),
        }
    }
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
    Input {
        index: u16,
    },
    Result {
        index: u16,
    },
    #[serde(rename_all = "camelCase")]
    NestedResult {
        index: u16,
        result_index: u16,
    },
}

impl From<TransactionArg> for Argument {
    fn from(arg: TransactionArg) -> Self {
        match arg {
            TransactionArg::GasCoin => Argument::GasCoin,
            TransactionArg::Input { index } => Argument::Input(index),
            TransactionArg::Result { index } => Argument::Result(index),
            TransactionArg::NestedResult {
                index,
                result_index,
            } => Argument::NestedResult(index, result_index),
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
    SplitCoins {
        coin: TransactionArg,
        amounts: Vec<TransactionArg>,
    },
    MergeCoins {
        destination: TransactionArg,
        sources: Vec<TransactionArg>,
    },
    Publish {
        modules: Vec<Data>,
        dependencies: Vec<String>,
    },
    #[serde(rename_all = "camelCase")]
    MakeMoveVec {
        type_tag: Option<TypeTagWrapper>,
        arguments: Vec<TransactionArg>,
    },
    #[serde(rename_all = "camelCase")]
    Upgrade {
        modules: Vec<Data>,
        dependencies: Vec<String>,
        package_id: String,
        ticket: TransactionArg,
    },
}

impl TryFrom<Transaction> for Command {
    type Error = SigningError;

    fn try_from(transaction: Transaction) -> Result<Self, Self::Error> {
        match transaction {
            Transaction::MoveCall {
                target,
                type_arguments,
                arguments,
            } => {
                let parts: Vec<&str> = target.split("::").collect();
                if parts.len() != 3 {
                    return SigningError::err(SigningErrorType::Error_invalid_params)
                        .context("Invalid target format for MoveCall command");
                }
                let package = ObjectID::from_str(parts[0]).context("Failed to parse package ID")?;
                let module = Identifier::from_str(parts[1])
                    .tw_err(SigningErrorType::Error_invalid_params)
                    .context("Failed to parse module")?;
                let function = Identifier::from_str(parts[2])
                    .tw_err(SigningErrorType::Error_invalid_params)
                    .context("Failed to parse function")?;
                Ok(Command::move_call(
                    package,
                    module,
                    function,
                    type_arguments.into_iter().map(|tag| tag.into()).collect(),
                    arguments
                        .into_iter()
                        .map(|argument| argument.into())
                        .collect(),
                ))
            },
            Transaction::TransferObjects { objects, address } => Ok(Command::TransferObjects(
                objects.into_iter().map(|object| object.into()).collect(),
                address.into(),
            )),
            Transaction::SplitCoins { coin, amounts } => Ok(Command::SplitCoins(
                coin.into(),
                amounts.into_iter().map(|amount| amount.into()).collect(),
            )),
            Transaction::MergeCoins {
                destination,
                sources,
            } => Ok(Command::MergeCoins(
                destination.into(),
                sources.into_iter().map(|source| source.into()).collect(),
            )),
            Transaction::Publish {
                modules,
                dependencies,
            } => Ok(Command::Publish(
                modules,
                dependencies
                    .into_iter()
                    .map(|dependency| {
                        ObjectID::from_str(&dependency).context("Failed to parse object ID")
                    })
                    .collect::<SigningResult<Vec<_>>>()?,
            )),
            Transaction::MakeMoveVec {
                type_tag,
                arguments,
            } => Ok(Command::MakeMoveVec(
                type_tag.map(|tag| tag.into()),
                arguments
                    .into_iter()
                    .map(|argument| argument.into())
                    .collect(),
            )),
            Transaction::Upgrade {
                modules,
                dependencies,
                package_id,
                ticket,
            } => Ok(Command::Upgrade(
                modules,
                dependencies
                    .into_iter()
                    .map(|dependency| {
                        ObjectID::from_str(&dependency).context("Failed to parse object ID")
                    })
                    .collect::<SigningResult<Vec<_>>>()?,
                ObjectID::from_str(&package_id).context("Failed to parse object ID")?,
                ticket.into(),
            )),
        }
    }
}

// Taken from here: https://github.com/MystenLabs/ts-sdks/blob/68e1b649c125f031b72ff7816d1ff653ef47cb53/packages/typescript/src/transactions/data/v1.ts#L271
#[derive(Debug, Deserialize, Serialize)]
pub enum Expiration {
    None(bool),
    Epoch(u64),
}

impl From<Expiration> for TransactionExpiration {
    fn from(expiration: Expiration) -> Self {
        match expiration {
            Expiration::None(_) => TransactionExpiration::None,
            Expiration::Epoch(epoch) => TransactionExpiration::Epoch(epoch),
        }
    }
}

#[derive(Debug, Deserialize, Serialize)]
#[serde(rename_all = "camelCase")]
pub struct RawTransaction {
    pub version: u8,
    pub sender: SuiAddress,
    pub expiration: Option<Expiration>,
    pub gas_config: GasConfig,
    pub inputs: Vec<Input>,
    pub transactions: Vec<Transaction>,
}
