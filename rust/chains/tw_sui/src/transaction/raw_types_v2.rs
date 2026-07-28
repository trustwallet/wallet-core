// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

//! Serialized transaction data **version 2**, as emitted by `Transaction.toJSON()` of the
//! `@mysten/sui` 2.x TypeScript SDK - the same SDK release that replaces the (now removed)
//! JSON-RPC client with `SuiGrpcClient`.
//!
//! Taken from here: https://github.com/MystenLabs/ts-sdks/blob/main/packages/sui/src/transactions/data/v2.ts
//!
//! Compared to [`super::raw_types`] (version 1), the notable differences are:
//! * `gasConfig` was renamed to `gasData`, and all of its fields became nullable;
//! * `transactions` was renamed to `commands`;
//! * inputs are positional now - the `kind`/`index`/`type` wrapper is gone, and every input is a
//!   plain `CallArg` enum;
//! * `Pure` input bytes are base64 encoded instead of being a JSON array of numbers;
//! * `MoveCall` carries `package`/`module`/`function` separately instead of a `target` string;
//! * a few variants that cannot be signed offline were added - see [`InputArgV2`] and [`CommandV2`].

use move_core_types::identifier::Identifier;
use serde::de::Error as DeError;
use serde::{Deserialize, Deserializer, Serialize, Serializer};
use std::str::FromStr;
use tw_coin_entry::error::prelude::*;
use tw_encoding::base64::{self, STANDARD};
use tw_memory::Data;

use crate::address::SuiAddress;
use crate::transaction::command::{Argument, Command};
use crate::transaction::raw_types::TypeTagWrapper;
use crate::transaction::sui_types::{
    CallArg, ObjectArg, ObjectDigest, ObjectID, ObjectRef, SequenceNumber, TransactionExpiration,
};

/// `JsonU64` of the TypeScript SDK - a `u64` encoded either as a JSON string or as a JSON number.
#[derive(Clone, Copy, Debug, PartialEq)]
pub struct JsonU64(pub u64);

impl Serialize for JsonU64 {
    fn serialize<S>(&self, serializer: S) -> Result<S::Ok, S::Error>
    where
        S: Serializer,
    {
        serializer.serialize_str(&self.0.to_string())
    }
}

impl<'de> Deserialize<'de> for JsonU64 {
    fn deserialize<D>(deserializer: D) -> Result<Self, D::Error>
    where
        D: Deserializer<'de>,
    {
        #[derive(Deserialize)]
        #[serde(untagged)]
        enum Repr {
            Str(String),
            Num(u64),
        }

        match Repr::deserialize(deserializer)? {
            Repr::Str(str) => str.parse::<u64>().map(JsonU64).map_err(DeError::custom),
            Repr::Num(num) => Ok(JsonU64(num)),
        }
    }
}

/// `BCSBytes` of the TypeScript SDK - a base64 encoded byte string.
/// A JSON array of numbers is accepted as well for compatibility with hand-crafted payloads.
#[derive(Clone, Debug, PartialEq)]
pub struct BcsBytes(pub Data);

impl Serialize for BcsBytes {
    fn serialize<S>(&self, serializer: S) -> Result<S::Ok, S::Error>
    where
        S: Serializer,
    {
        serializer.serialize_str(&base64::encode(&self.0, STANDARD))
    }
}

impl<'de> Deserialize<'de> for BcsBytes {
    fn deserialize<D>(deserializer: D) -> Result<Self, D::Error>
    where
        D: Deserializer<'de>,
    {
        #[derive(Deserialize)]
        #[serde(untagged)]
        enum Repr {
            Base64(String),
            Raw(Data),
        }

        match Repr::deserialize(deserializer)? {
            Repr::Base64(str) => base64::decode(&str, STANDARD)
                .map(BcsBytes)
                .map_err(|e| DeError::custom(format!("Invalid base64 bytes: {e:?}"))),
            Repr::Raw(bytes) => Ok(BcsBytes(bytes)),
        }
    }
}

#[derive(Debug, Deserialize, Serialize)]
#[serde(rename_all = "camelCase")]
pub struct ObjectRefV2 {
    pub object_id: String,
    pub version: JsonU64,
    pub digest: String,
}

impl TryFrom<ObjectRefV2> for ObjectRef {
    type Error = SigningError;

    fn try_from(object_ref: ObjectRefV2) -> Result<Self, Self::Error> {
        Ok((
            ObjectID::from_str(&object_ref.object_id).context("Invalid Object ID")?,
            SequenceNumber(object_ref.version.0),
            ObjectDigest::from_str(&object_ref.digest)?,
        ))
    }
}

#[derive(Debug, Deserialize, Serialize)]
pub enum InputObjectArgV2 {
    ImmOrOwnedObject(ObjectRefV2),
    #[serde(rename_all = "camelCase")]
    SharedObject {
        object_id: String,
        initial_shared_version: JsonU64,
        mutable: bool,
    },
    Receiving(ObjectRefV2),
}

impl TryFrom<InputObjectArgV2> for ObjectArg {
    type Error = SigningError;

    fn try_from(arg: InputObjectArgV2) -> Result<Self, Self::Error> {
        match arg {
            InputObjectArgV2::ImmOrOwnedObject(object_ref) => {
                Ok(ObjectArg::ImmOrOwnedObject(object_ref.try_into()?))
            },
            InputObjectArgV2::SharedObject {
                object_id,
                initial_shared_version,
                mutable,
            } => Ok(ObjectArg::SharedObject {
                id: ObjectID::from_str(&object_id).context("Invalid Object ID")?,
                initial_shared_version: SequenceNumber(initial_shared_version.0),
                mutable,
            }),
            InputObjectArgV2::Receiving(object_ref) => {
                Ok(ObjectArg::Receiving(object_ref.try_into()?))
            },
        }
    }
}

#[derive(Debug, Deserialize, Serialize)]
pub enum InputArgV2 {
    Object(InputObjectArgV2),
    Pure {
        bytes: BcsBytes,
    },
    /// A value that has not been BCS-encoded yet. Resolving it requires knowing the expected Move
    /// type, which in turn requires querying the network - impossible to do offline.
    UnresolvedPure(serde_json::Value),
    /// An object reference that is missing its version and/or digest. Resolving it requires
    /// querying the network - impossible to do offline.
    UnresolvedObject(serde_json::Value),
    /// Reserved for the upcoming funds withdrawal feature, not representable in `TransactionData::V1`.
    FundsWithdrawal(serde_json::Value),
}

impl TryFrom<InputArgV2> for CallArg {
    type Error = SigningError;

    fn try_from(arg: InputArgV2) -> Result<Self, Self::Error> {
        match arg {
            InputArgV2::Pure { bytes } => Ok(CallArg::Pure(bytes.0)),
            InputArgV2::Object(object) => Ok(CallArg::Object(object.try_into()?)),
            InputArgV2::UnresolvedPure(_) => {
                SigningError::err(SigningErrorType::Error_invalid_params).context(
                    "'UnresolvedPure' input requires a network lookup to be BCS-encoded. \
                     Call `Transaction.build()` or `Transaction.prepareForSerialization()` before signing",
                )
            },
            InputArgV2::UnresolvedObject(_) => {
                SigningError::err(SigningErrorType::Error_invalid_params).context(
                    "'UnresolvedObject' input is missing its version and/or digest. \
                     Call `Transaction.build()` or `Transaction.prepareForSerialization()` before signing",
                )
            },
            InputArgV2::FundsWithdrawal(_) => {
                SigningError::err(SigningErrorType::Error_not_supported)
                    .context("'FundsWithdrawal' input is not supported")
            },
        }
    }
}

#[derive(Clone, Copy, Debug, Deserialize, Serialize)]
pub enum ArgumentV2 {
    GasCoin(bool),
    Input(u16),
    Result(u16),
    NestedResult(u16, u16),
}

impl From<ArgumentV2> for Argument {
    fn from(arg: ArgumentV2) -> Self {
        match arg {
            ArgumentV2::GasCoin(_) => Argument::GasCoin,
            ArgumentV2::Input(index) => Argument::Input(index),
            ArgumentV2::Result(index) => Argument::Result(index),
            ArgumentV2::NestedResult(index, result_index) => {
                Argument::NestedResult(index, result_index)
            },
        }
    }
}

#[derive(Debug, Deserialize, Serialize)]
pub enum CommandV2 {
    #[serde(rename_all = "camelCase")]
    MoveCall {
        package: String,
        module: String,
        function: String,
        type_arguments: Vec<TypeTagWrapper>,
        arguments: Vec<ArgumentV2>,
    },
    TransferObjects {
        objects: Vec<ArgumentV2>,
        address: ArgumentV2,
    },
    SplitCoins {
        coin: ArgumentV2,
        amounts: Vec<ArgumentV2>,
    },
    MergeCoins {
        destination: ArgumentV2,
        sources: Vec<ArgumentV2>,
    },
    Publish {
        modules: Vec<BcsBytes>,
        dependencies: Vec<String>,
    },
    MakeMoveVec {
        #[serde(rename = "type")]
        type_tag: Option<TypeTagWrapper>,
        elements: Vec<ArgumentV2>,
    },
    Upgrade {
        modules: Vec<BcsBytes>,
        dependencies: Vec<String>,
        package: String,
        ticket: ArgumentV2,
    },
    /// A transaction intent that has to be expanded into real commands by an SDK plugin
    /// before the transaction can be signed.
    #[serde(rename = "$Intent")]
    Intent(serde_json::Value),
}

fn parse_object_ids(ids: Vec<String>) -> SigningResult<Vec<ObjectID>> {
    ids.into_iter()
        .map(|id| ObjectID::from_str(&id).context("Failed to parse object ID"))
        .collect()
}

fn into_arguments(arguments: Vec<ArgumentV2>) -> Vec<Argument> {
    arguments.into_iter().map(Argument::from).collect()
}

fn into_modules(modules: Vec<BcsBytes>) -> Vec<Data> {
    modules.into_iter().map(|module| module.0).collect()
}

impl TryFrom<CommandV2> for Command {
    type Error = SigningError;

    fn try_from(command: CommandV2) -> Result<Self, Self::Error> {
        match command {
            CommandV2::MoveCall {
                package,
                module,
                function,
                type_arguments,
                arguments,
            } => {
                let package = ObjectID::from_str(&package).context("Failed to parse package ID")?;
                let module = Identifier::from_str(&module)
                    .tw_err(SigningErrorType::Error_invalid_params)
                    .context("Failed to parse module")?;
                let function = Identifier::from_str(&function)
                    .tw_err(SigningErrorType::Error_invalid_params)
                    .context("Failed to parse function")?;
                Ok(Command::move_call(
                    package,
                    module,
                    function,
                    type_arguments.into_iter().map(|tag| tag.into()).collect(),
                    into_arguments(arguments),
                ))
            },
            CommandV2::TransferObjects { objects, address } => Ok(Command::TransferObjects(
                into_arguments(objects),
                address.into(),
            )),
            CommandV2::SplitCoins { coin, amounts } => {
                Ok(Command::SplitCoins(coin.into(), into_arguments(amounts)))
            },
            CommandV2::MergeCoins {
                destination,
                sources,
            } => Ok(Command::MergeCoins(
                destination.into(),
                into_arguments(sources),
            )),
            CommandV2::Publish {
                modules,
                dependencies,
            } => Ok(Command::Publish(
                into_modules(modules),
                parse_object_ids(dependencies)?,
            )),
            CommandV2::MakeMoveVec { type_tag, elements } => Ok(Command::MakeMoveVec(
                type_tag.map(|tag| tag.into()),
                into_arguments(elements),
            )),
            CommandV2::Upgrade {
                modules,
                dependencies,
                package,
                ticket,
            } => Ok(Command::Upgrade(
                into_modules(modules),
                parse_object_ids(dependencies)?,
                ObjectID::from_str(&package).context("Failed to parse package ID")?,
                ticket.into(),
            )),
            CommandV2::Intent(_) => SigningError::err(SigningErrorType::Error_invalid_params)
                .context(
                    "'$Intent' command must be resolved by the SDK before signing. \
                     Call `Transaction.build()` or `Transaction.prepareForSerialization()` first",
                ),
        }
    }
}

#[derive(Debug, Deserialize, Serialize)]
pub enum ExpirationV2 {
    None(bool),
    Epoch(JsonU64),
    /// Introduced together with the gRPC API, not representable in `TransactionData::V1`.
    ValidDuring(serde_json::Value),
}

impl TryFrom<ExpirationV2> for TransactionExpiration {
    type Error = SigningError;

    fn try_from(expiration: ExpirationV2) -> Result<Self, Self::Error> {
        match expiration {
            ExpirationV2::None(_) => Ok(TransactionExpiration::None),
            ExpirationV2::Epoch(epoch) => Ok(TransactionExpiration::Epoch(epoch.0)),
            ExpirationV2::ValidDuring(_) => {
                SigningError::err(SigningErrorType::Error_not_supported)
                    .context("'ValidDuring' transaction expiration is not supported")
            },
        }
    }
}

#[derive(Debug, Default, Deserialize, Serialize)]
#[serde(rename_all = "camelCase")]
pub struct GasDataV2 {
    #[serde(default)]
    pub budget: Option<JsonU64>,
    #[serde(default)]
    pub price: Option<JsonU64>,
    /// The gas sponsor. Defaults to the `sender` when not set.
    #[serde(default)]
    pub owner: Option<SuiAddress>,
    #[serde(default)]
    pub payment: Option<Vec<ObjectRefV2>>,
}

#[derive(Debug, Deserialize, Serialize)]
#[serde(rename_all = "camelCase")]
pub struct RawTransactionV2 {
    pub version: u32,
    #[serde(default)]
    pub sender: Option<SuiAddress>,
    #[serde(default)]
    pub expiration: Option<ExpirationV2>,
    #[serde(default)]
    pub gas_data: GasDataV2,
    pub inputs: Vec<InputArgV2>,
    pub commands: Vec<CommandV2>,
    #[serde(default)]
    pub digest: Option<String>,
}
