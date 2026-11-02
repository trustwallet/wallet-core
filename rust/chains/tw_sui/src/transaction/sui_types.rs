// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::SuiAddress;
use crate::constants::{
    SUI_FRAMEWORK_ADDRESS, SUI_MODULE_NAME, SUI_STRUCT_NAME, SUI_SYSTEM_STATE_OBJECT_ID,
    SUI_SYSTEM_STATE_OBJECT_SHARED_VERSION,
};
use move_core_types::account_address::AccountAddress;
use move_core_types::language_storage::{StructTag, TypeTag};
use serde::{Deserialize, Serialize};
use std::str::FromStr;
use tw_coin_entry::error::prelude::*;
use tw_encoding::base58::{self, Alphabet};
use tw_hash::{as_bytes, H256};
use tw_memory::Data;

pub type ObjectRef = (ObjectID, SequenceNumber, ObjectDigest);

/// `0x2::sui::SUI`, the native coin type.
pub fn sui_coin_type_tag() -> TypeTag {
    TypeTag::Struct(Box::new(StructTag {
        address: SUI_FRAMEWORK_ADDRESS,
        module: SUI_MODULE_NAME.to_owned(),
        name: SUI_STRUCT_NAME.to_owned(),
        type_params: vec![],
    }))
}
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
            .tw_err(SigningErrorType::Error_invalid_params)
            .context("Invalid Object Digest: expected valid base58 string")?;
        H256::try_from(bytes.as_slice())
            .map(ObjectDigest)
            .tw_err(SigningErrorType::Error_invalid_params)
            .context("Invalid Object Digest: expected exactly 32 bytes")
    }
}

#[derive(Debug, Deserialize, Serialize)]
pub enum CallArg {
    // contains no structs or objects
    Pure(Data),
    // an object
    Object(ObjectArg),
    /// SIP-58: a reservation to withdraw funds from an address balance. At execution time it is
    /// turned into a `0x2::funds_accumulator::Withdrawal<Balance<T>>` that `coin::redeem_funds`
    /// converts into a `Coin<T>`. Several withdrawals, even of the same type, are allowed.
    FundsWithdrawal(FundsWithdrawalArg),
}

/// SIP-58 withdrawal of `reservation` of `type_arg` from the `withdraw_from` address balance.
/// Taken from here: https://github.com/MystenLabs/sui/blob/main/crates/sui-types/src/transaction.rs
#[derive(Debug, Deserialize, Serialize)]
pub struct FundsWithdrawalArg {
    pub reservation: Reservation,
    pub type_arg: WithdrawalTypeArg,
    pub withdraw_from: WithdrawFrom,
}

impl FundsWithdrawalArg {
    /// Withdraws exactly up to `amount` of `Balance<coin_type>` from the transaction sender.
    pub fn balance_from_sender(amount: u64, coin_type: TypeTag) -> Self {
        FundsWithdrawalArg {
            reservation: Reservation::MaxAmountU64(amount),
            type_arg: WithdrawalTypeArg::Balance(coin_type),
            withdraw_from: WithdrawFrom::Sender,
        }
    }
}

#[derive(Debug, Deserialize, Serialize)]
pub enum Reservation {
    /// Reserve a specific amount of the balance.
    MaxAmountU64(u64),
}

#[derive(Debug, Deserialize, Serialize)]
pub enum WithdrawalTypeArg {
    /// `Balance<T>` of the given coin type.
    Balance(TypeTag),
}

/// Only the variant wallet-core uses. The chain defines more after `Sender` (`Sponsor`, ...);
/// BCS encodes the variant by position, so new ones must be appended in the chain's order.
#[derive(Debug, Deserialize, Serialize)]
pub enum WithdrawFrom {
    /// Withdraw from the sender of the transaction.
    Sender,
}

impl CallArg {
    pub const SUI_SYSTEM_MUT: Self = Self::Object(ObjectArg::SUI_SYSTEM_MUT);
}

// Taken from here: https://github.com/MystenLabs/sui/blob/93f02057bb55eca407f04f551e6514f123005b65/crates/sui-types/src/transaction.rs#L107
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
    // A Move object that can be received in this transaction.
    Receiving(ObjectRef),
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
            ObjectArg::Receiving((id, _, _)) => *id,
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
    /// SIP-58: required when gas is paid from an address balance (`gas_data.payment` is empty).
    /// Without an owned gas coin a transaction has no natural uniqueness or replay protection, so
    /// it declares a validity window of at most two epochs (N and N+1), the network it is valid
    /// on and a nonce. Variant index 2 must stay in this position: BCS encodes it by position.
    ValidDuring {
        /// Transaction invalid before this epoch. Must not be later than the current epoch.
        min_epoch: Option<EpochId>,
        /// Transaction expires after this epoch (at most `min_epoch + 1`).
        max_epoch: Option<EpochId>,
        /// Reserved by the chain for sub-epoch timing (not implemented there). Always `None`.
        min_timestamp: Option<u64>,
        /// Reserved by the chain for sub-epoch timing (not implemented there). Always `None`.
        max_timestamp: Option<u64>,
        /// Network identifier, prevents cross-chain replay.
        chain: ChainIdentifier,
        /// Differentiates otherwise identical transactions; ignored by the protocol except in the
        /// transaction digest.
        nonce: u32,
    },
}

/// The genesis checkpoint digest of a network (base58 in JSON and RPC).
/// BCS encodes it as length-prefixed bytes, like [`ObjectDigest`].
#[derive(Clone, Copy, Debug, Deserialize, Eq, Hash, PartialEq, Serialize)]
pub struct ChainIdentifier(#[serde(with = "as_bytes")] pub H256);

impl FromStr for ChainIdentifier {
    type Err = SigningError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let bytes = base58::decode(s, Alphabet::Bitcoin)
            .tw_err(SigningErrorType::Error_invalid_params)
            .context("Invalid chain identifier: expected valid base58 string")?;
        H256::try_from(bytes.as_slice())
            .map(ChainIdentifier)
            .tw_err(SigningErrorType::Error_invalid_params)
            .context("Invalid chain identifier: expected exactly 32 bytes")
    }
}
