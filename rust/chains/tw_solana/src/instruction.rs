// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::SolanaAddress;
use serde::{Deserialize, Serialize};
use tw_memory::Data;

pub struct Instruction {
    /// Pubkey of the program that executes this instruction.
    pub program_id: SolanaAddress,
    /// Metadata describing accounts that should be passed to the program.
    pub accounts: Vec<AccountMeta>,
    /// Opaque data passed to the program for its own interpretation.
    pub data: Data,
}

impl Instruction {
    /// Create a new instruction from a value, encoded with [`bincode`].
    ///
    /// [`bincode`]: https://docs.rs/bincode/latest/bincode/
    ///
    /// `program_id` is the address of the program that will execute the instruction.
    /// `accounts` contains a description of all accounts that may be accessed by the program.
    pub fn new_with_bincode<T: Serialize>(
        program_id: SolanaAddress,
        data: T,
        accounts: Vec<AccountMeta>,
    ) -> Self {
        let data = bincode::serialize(&data).expect("Error serializing bincode");
        Self {
            program_id,
            accounts,
            data,
        }
    }

    pub fn new(program_id: SolanaAddress, data: Data, accounts: Vec<AccountMeta>) -> Self {
        Self {
            program_id,
            accounts,
            data,
        }
    }
}

#[derive(Debug, Default, PartialEq, Eq, Clone, Serialize, Deserialize)]
pub struct AccountMeta {
    /// An account's public key.
    pub pubkey: SolanaAddress,
    /// True if an `Instruction` requires a `Transaction` signature matching `pubkey`.
    pub is_signer: bool,
    /// True if the account data or metadata may be mutated during program execution.
    pub is_writable: bool,
}

impl AccountMeta {
    /// Construct metadata for a writable account.
    pub fn new(pubkey: SolanaAddress, is_signer: bool) -> Self {
        Self {
            pubkey,
            is_signer,
            is_writable: true,
        }
    }

    /// Construct metadata for a read-only account.
    pub fn readonly(pubkey: SolanaAddress, is_signer: bool) -> Self {
        Self {
            pubkey,
            is_signer,
            is_writable: false,
        }
    }
}
