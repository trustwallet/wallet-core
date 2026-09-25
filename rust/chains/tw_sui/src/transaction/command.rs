// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::transaction::sui_types::ObjectID;
use move_core_types::identifier::Identifier;
use move_core_types::language_storage::TypeTag;
use serde::{Deserialize, Serialize};

// Taken from here: https://github.com/MystenLabs/sui/blob/93f02057bb55eca407f04f551e6514f123005b65/crates/sui-types/src/transaction.rs#L660
/// A single command in a programmable transaction.
#[derive(Debug, Deserialize, Serialize)]
pub enum Command {
    /// A call to either an entry or a public Move function
    MoveCall(Box<ProgrammableMoveCall>),
    /// `(Vec<forall T:key+store. T>, address)`
    /// It sends n-objects to the specified address. These objects must have store
    /// (public transfer) and either the previous owner must be an address or the object must
    /// be newly created.
    TransferObjects(Vec<Argument>, Argument),
    /// `(&mut Coin<T>, Vec<u64>)` -> `Vec<Coin<T>>`
    /// It splits off some amounts into a new coins with those amounts
    SplitCoins(Argument, Vec<Argument>),
    /// `(&mut Coin<T>, Vec<Coin<T>>)`
    /// It merges n-coins into the first coin
    MergeCoins(Argument, Vec<Argument>),
    /// Publishes a Move package. It takes the package bytes and a list of the package's transitive
    /// dependencies to link against on-chain.
    Publish(Vec<Vec<u8>>, Vec<ObjectID>),
    /// `forall T: Vec<T> -> vector<T>`
    /// Given n-values of the same type, it constructs a vector. For non objects or an empty vector,
    /// the type tag must be specified.
    MakeMoveVec(Option<TypeTag>, Vec<Argument>),
    /// Upgrades a Move package
    /// Takes (in order):
    /// 1. A vector of serialized modules for the package.
    /// 2. A vector of object ids for the transitive dependencies of the new package.
    /// 3. The object ID of the package being upgraded.
    /// 4. An argument holding the `UpgradeTicket` that must have been produced from an earlier command in the same
    ///    programmable transaction.
    Upgrade(Vec<Vec<u8>>, Vec<ObjectID>, ObjectID, Argument),
}

impl Command {
    pub fn move_call(
        package: ObjectID,
        module: Identifier,
        function: Identifier,
        type_arguments: Vec<TypeTag>,
        arguments: Vec<Argument>,
    ) -> Self {
        Command::MoveCall(Box::new(ProgrammableMoveCall {
            package,
            module,
            function,
            type_arguments,
            arguments,
        }))
    }
}

/// The command for calling a Move function, either an entry function or a public
/// function (which cannot return references).
#[derive(Debug, Deserialize, Serialize)]
pub struct ProgrammableMoveCall {
    /// The package containing the module and function.
    pub package: ObjectID,
    /// The specific module in the package containing the function.
    pub module: Identifier,
    /// The function to be called.
    pub function: Identifier,
    /// The type arguments to the function.
    pub type_arguments: Vec<TypeTag>,
    /// The arguments to the function.
    pub arguments: Vec<Argument>,
}

/// An argument to a programmable transaction command
#[derive(Clone, Copy, Debug, Deserialize, Serialize)]
pub enum Argument {
    /// The gas coin. The gas coin can only be used by-ref, except for with
    /// `TransferObjects`, which can use it by-value.
    GasCoin,
    /// One of the input objects or primitive values (from
    /// `ProgrammableTransaction` inputs)
    Input(u16),
    /// The result of another command (from `ProgrammableTransaction` commands)
    Result(u16),
    /// Like a `Result` but it accesses a nested result. Currently, the only usage
    /// of this is to access a value from a Move call with multiple return values.
    NestedResult(u16, u16),
}
