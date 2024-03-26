// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::SuiAddress;
use crate::constants::{SUI_SYSTEM_STATE_OBJECT_ID, SUI_SYSTEM_STATE_OBJECT_SHARED_VERSION};
use move_core_types::account_address::AccountAddress;
use tw_hash::H256;
use tw_memory::Data;

pub type ObjectDigest = H256;
pub type ObjectRef = (ObjectID, SequenceNumber, ObjectDigest);
pub type EpochId = u64;

pub struct Object1Ref {
    pub id: ObjectID,
    pub version: SequenceNumber,
    pub digest: ObjectDigest,
}

#[derive(Eq, PartialEq, Ord, PartialOrd, Copy, Clone, Hash, Default, Debug)]
pub struct SequenceNumber(pub u64);

#[derive(Debug, Eq, PartialEq, Clone, Copy, PartialOrd, Ord, Hash)]
pub struct ObjectID(pub AccountAddress);

#[derive(Debug, PartialEq, Eq, Hash, Clone)]
pub enum CallArg {
    // contains no structs or objects
    Pure(Data),
    // an object
    Object(ObjectArg),
}

impl CallArg {
    pub const SUI_SYSTEM_MUT: Self = Self::Object(ObjectArg::SUI_SYSTEM_MUT);
}

#[derive(Debug, PartialEq, Eq, Hash, Clone, Copy)]
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

#[derive(Debug, PartialEq, Eq, Hash, Clone)]
pub struct GasData {
    pub payment: Vec<ObjectRef>,
    pub owner: SuiAddress,
    pub price: u64,
    pub budget: u64,
}

#[derive(Debug, PartialEq, Eq, Hash, Clone, Copy)]
pub enum TransactionExpiration {
    /// The transaction has no expiration
    None,
    /// Validators wont sign a transaction unless the expiration Epoch
    /// is greater than or equal to the current epoch
    Epoch(EpochId),
}
