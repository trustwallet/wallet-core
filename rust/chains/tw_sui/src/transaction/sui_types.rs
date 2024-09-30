// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::SuiAddress;
use crate::constants::{SUI_SYSTEM_STATE_OBJECT_ID, SUI_SYSTEM_STATE_OBJECT_SHARED_VERSION};
use move_core_types::account_address::AccountAddress;
use serde::{Deserialize, Serialize};
use std::str::FromStr;
use tw_coin_entry::error::prelude::*;
use tw_encoding::base58::{self, Alphabet};
use tw_hash::{as_bytes, H256};
use tw_memory::Data;

pub type ObjectRef = (ObjectID, SequenceNumber, ObjectDigest);
pub type EpochId = u64;

#[derive(Clone, Copy, Debug, Deserialize, PartialEq, Serialize)]
pub struct SequenceNumber(pub u64);

#[derive(Clone, Copy, Debug, Deserialize, Eq, Hash, PartialEq, Serialize)]
pub struct ObjectID(pub AccountAddress);

impl FromStr for ObjectID {
    type Err = SigningError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let addr = SuiAddress::from_str(s)
            .into_tw()
            .context("Invalid Object ID")?;
        Ok(ObjectID(addr.into_inner()))
    }
}

#[derive(Clone, Copy, Debug, Deserialize, Eq, Hash, PartialEq, Serialize)]
pub struct ObjectDigest(#[serde(with = "as_bytes")] pub H256);

impl FromStr for ObjectDigest {
    type Err = SigningError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let bytes = base58::decode(s, Alphabet::Bitcoin)
            .tw_err(|_| SigningErrorType::Error_invalid_params)
            .context("Invalid Object Digest: expected valid base58 string")?;
        H256::try_from(bytes.as_slice())
            .map(ObjectDigest)
            .tw_err(|_| SigningErrorType::Error_invalid_params)
            .context("Invalid Object Digest: expected exactly 32 bytes")
    }
}

#[derive(Debug, Deserialize, Serialize)]
pub enum CallArg {
    // contains no structs or objects
    Pure(Data),
    // an object
    Object(ObjectArg),
}

impl CallArg {
    pub const SUI_SYSTEM_MUT: Self = Self::Object(ObjectArg::SUI_SYSTEM_MUT);
}

#[derive(Debug, Deserialize, PartialEq, Serialize)]
pub enum ObjectArg {
    // A Move object, either immutable, or owned mutable.
    ImmOrOwnedObject(ObjectRef),
    // A Move object that's shared.
    // SharedObject::mutable controls whether caller asks for a mutable reference to shared object.
    SharedObject {
        id: ObjectID,
        initial_shared_version: SequenceNumber,
        mutable: bool,
    },
}

impl ObjectArg {
    pub const SUI_SYSTEM_MUT: Self = Self::SharedObject {
        id: SUI_SYSTEM_STATE_OBJECT_ID,
        initial_shared_version: SUI_SYSTEM_STATE_OBJECT_SHARED_VERSION,
        mutable: true,
    };

    pub fn id(&self) -> ObjectID {
        match self {
            ObjectArg::ImmOrOwnedObject((id, _, _)) | ObjectArg::SharedObject { id, .. } => *id,
        }
    }
}

#[derive(Debug, Deserialize, Serialize)]
pub struct GasData {
    pub payment: Vec<ObjectRef>,
    pub owner: SuiAddress,
    pub price: u64,
    pub budget: u64,
}

#[derive(Debug, Deserialize, Serialize)]
pub enum TransactionExpiration {
    /// The transaction has no expiration
    None,
    /// Validators wont sign a transaction unless the expiration Epoch
    /// is greater than or equal to the current epoch
    Epoch(EpochId),
}
