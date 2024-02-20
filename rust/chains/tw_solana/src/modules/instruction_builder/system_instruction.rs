// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::SolanaAddress;
use crate::defined_addresses::*;
use crate::instruction::{AccountMeta, Instruction};
use serde::{Deserialize, Serialize};

/// An instruction to the system program.
#[derive(Serialize, Deserialize, Debug, Clone, PartialEq, Eq)]
pub enum SystemInstruction {
    /// Create a new account
    ///
    /// # Account references
    ///   0. `[WRITE, SIGNER]` Funding account
    ///   1. `[WRITE, SIGNER]` New account
    CreateAccount {
        /// Number of lamports to transfer to the new account
        lamports: u64,

        /// Number of bytes of memory to allocate
        space: u64,

        /// Address of program that will own the new account
        owner: SolanaAddress,
    },

    /// Assign account to a program
    ///
    /// # Account references
    ///   0. `[WRITE, SIGNER]` Assigned account public key
    Assign {
        /// Owner program account
        owner: SolanaAddress,
    },

    /// Transfer lamports
    ///
    /// # Account references
    ///   0. `[WRITE, SIGNER]` Funding account
    ///   1. `[WRITE]` Recipient account
    Transfer { lamports: u64 },

    /// Create a new account at an address derived from a base pubkey and a seed
    ///
    /// # Account references
    ///   0. `[WRITE, SIGNER]` Funding account
    ///   1. `[WRITE]` Created account
    ///   2. `[SIGNER]` (optional) Base account; the account matching the base SolanaAddress below must be
    ///                          provided as a signer, but may be the same as the funding account
    ///                          and provided as account 0
    CreateAccountWithSeed {
        /// Base public key
        base: SolanaAddress,

        /// String of ASCII chars, no longer than `SolanaAddress::MAX_SEED_LEN`
        seed: String,

        /// Number of lamports to transfer to the new account
        lamports: u64,

        /// Number of bytes of memory to allocate
        space: u64,

        /// Owner program account address
        owner: SolanaAddress,
    },

    /// Consumes a stored nonce, replacing it with a successor
    ///
    /// # Account references
    ///   0. `[WRITE]` Nonce account
    ///   1. `[]` RecentBlockhashes sysvar
    ///   2. `[SIGNER]` Nonce authority
    AdvanceNonceAccount,

    /// Withdraw funds from a nonce account
    ///
    /// # Account references
    ///   0. `[WRITE]` Nonce account
    ///   1. `[WRITE]` Recipient account
    ///   2. `[]` RecentBlockhashes sysvar
    ///   3. `[]` Rent sysvar
    ///   4. `[SIGNER]` Nonce authority
    ///
    /// The `u64` parameter is the lamports to withdraw, which must leave the
    /// account balance above the rent exempt reserve or at zero.
    WithdrawNonceAccount(u64),

    /// Drive state of Uninitialized nonce account to Initialized, setting the nonce value
    ///
    /// # Account references
    ///   0. `[WRITE]` Nonce account
    ///   1. `[]` RecentBlockhashes sysvar
    ///   2. `[]` Rent sysvar
    ///
    /// The `SolanaAddress` parameter specifies the entity authorized to execute nonce
    /// instruction on the account
    ///
    /// No signatures are required to execute this instruction, enabling derived
    /// nonce account addresses
    InitializeNonceAccount(SolanaAddress),

    /// Change the entity authorized to execute nonce instructions on the account
    ///
    /// # Account references
    ///   0. `[WRITE]` Nonce account
    ///   1. `[SIGNER]` Nonce authority
    ///
    /// The `SolanaAddress` parameter identifies the entity to authorize
    AuthorizeNonceAccount(SolanaAddress),

    /// Allocate space in a (possibly new) account without funding
    ///
    /// # Account references
    ///   0. `[WRITE, SIGNER]` New account
    Allocate {
        /// Number of bytes of memory to allocate
        space: u64,
    },

    /// Allocate space for and assign an account at an address
    ///    derived from a base public key and a seed
    ///
    /// # Account references
    ///   0. `[WRITE]` Allocated account
    ///   1. `[SIGNER]` Base account
    AllocateWithSeed {
        /// Base public key
        base: SolanaAddress,

        /// String of ASCII chars, no longer than `pubkey::MAX_SEED_LEN`
        seed: String,

        /// Number of bytes of memory to allocate
        space: u64,

        /// Owner program account
        owner: SolanaAddress,
    },

    /// Assign account to a program based on a seed
    ///
    /// # Account references
    ///   0. `[WRITE]` Assigned account
    ///   1. `[SIGNER]` Base account
    AssignWithSeed {
        /// Base public key
        base: SolanaAddress,

        /// String of ASCII chars, no longer than `pubkey::MAX_SEED_LEN`
        seed: String,

        /// Owner program account
        owner: SolanaAddress,
    },

    /// Transfer lamports from a derived address
    ///
    /// # Account references
    ///   0. `[WRITE]` Funding account
    ///   1. `[SIGNER]` Base for funding account
    ///   2. `[WRITE]` Recipient account
    TransferWithSeed {
        /// Amount to transfer
        lamports: u64,

        /// Seed to use to derive the funding account address
        from_seed: String,

        /// Owner to use to derive the funding account address
        from_owner: SolanaAddress,
    },

    /// One-time idempotent upgrade of legacy nonce versions in order to bump
    /// them out of chain blockhash domain.
    ///
    /// # Account references
    ///   0. `[WRITE]` Nonce account
    UpgradeNonceAccount,
}

pub struct SystemInstructionBuilder;

impl SystemInstructionBuilder {
    pub fn memo(memo: &str) -> Instruction {
        let account_metas = Vec::default();
        let data = memo.as_bytes().to_vec();
        Instruction::new(*MEMO_PROGRAM_ID_ADDRESS, data, account_metas)
    }

    pub fn create_account(
        from_pubkey: SolanaAddress,
        to_pubkey: SolanaAddress,
        lamports: u64,
        space: u64,
        owner: SolanaAddress,
    ) -> Instruction {
        let account_metas = vec![
            AccountMeta::new(from_pubkey, true),
            AccountMeta::new(to_pubkey, true),
        ];
        Instruction::new_with_bincode(
            *SYSTEM_PROGRAM_ID_ADDRESS,
            SystemInstruction::CreateAccount {
                lamports,
                space,
                owner,
            },
            account_metas,
        )
    }

    pub fn transfer(
        from_pubkey: SolanaAddress,
        to_pubkey: SolanaAddress,
        lamports: u64,
    ) -> Instruction {
        let account_metas = vec![
            AccountMeta::new(from_pubkey, true),
            AccountMeta::new(to_pubkey, false),
        ];
        Instruction::new_with_bincode(
            *SYSTEM_PROGRAM_ID_ADDRESS,
            SystemInstruction::Transfer { lamports },
            account_metas,
        )
    }

    /// Please note `to_pubkey` must match `create_with_seed(base, seed, owner)`.
    pub fn create_account_with_seed(
        from_pubkey: SolanaAddress,
        to_pubkey: SolanaAddress,
        base: SolanaAddress,
        seed: String,
        lamports: u64,
        space: u64,
    ) -> Instruction {
        let account_metas = vec![
            AccountMeta::new(from_pubkey, true),
            AccountMeta::new(to_pubkey, false),
            AccountMeta::readonly(base, true),
        ];

        Instruction::new_with_bincode(
            *SYSTEM_PROGRAM_ID_ADDRESS,
            SystemInstruction::CreateAccountWithSeed {
                base,
                seed,
                lamports,
                space,
                owner: *STAKE_PROGRAM_ID_ADDRESS,
            },
            account_metas,
        )
    }

    /// Advance the value of a durable transaction nonce.
    ///
    /// # Required signers
    ///
    /// The `authorized_pubkey` signer must sign the transaction.
    pub fn advance_nonce_account(
        nonce_pubkey: SolanaAddress,
        authorized_pubkey: SolanaAddress,
    ) -> Instruction {
        let account_metas = vec![
            AccountMeta::new(nonce_pubkey, false),
            AccountMeta::readonly(*SYSVAR_RECENT_BLOCKHASHS_ADDRESS, false),
            AccountMeta::readonly(authorized_pubkey, true),
        ];
        Instruction::new_with_bincode(
            *SYSTEM_PROGRAM_ID_ADDRESS,
            SystemInstruction::AdvanceNonceAccount,
            account_metas,
        )
    }

    /// Create an account containing a durable transaction nonce.
    pub fn init_nonce_account(
        nonce_pubkey: SolanaAddress,
        authority: SolanaAddress,
    ) -> Instruction {
        let account_metas = vec![
            AccountMeta::new(nonce_pubkey, false),
            AccountMeta::readonly(*SYSVAR_RECENT_BLOCKHASHS_ADDRESS, false),
            AccountMeta::readonly(*SYSVAR_RENT_ID_ADDRESS, false),
        ];
        Instruction::new_with_bincode(
            *SYSTEM_PROGRAM_ID_ADDRESS,
            SystemInstruction::InitializeNonceAccount(authority),
            account_metas,
        )
    }

    /// Withdraw lamports from a durable transaction nonce account.
    pub fn withdraw_nonce_account(
        nonce_pubkey: SolanaAddress,
        authorized_pubkey: SolanaAddress,
        to_pubkey: SolanaAddress,
        lamports: u64,
    ) -> Instruction {
        let account_metas = vec![
            AccountMeta::new(nonce_pubkey, false),
            AccountMeta::new(to_pubkey, false),
            AccountMeta::readonly(*SYSVAR_RECENT_BLOCKHASHS_ADDRESS, false),
            AccountMeta::readonly(*SYSVAR_RENT_ID_ADDRESS, false),
            AccountMeta::readonly(authorized_pubkey, true),
        ];
        Instruction::new_with_bincode(
            *SYSTEM_PROGRAM_ID_ADDRESS,
            SystemInstruction::WithdrawNonceAccount(lamports),
            account_metas,
        )
    }

    /// The function represents "create nonce account" operation that consists of several small instructions.
    pub fn create_nonce_account(
        signer: SolanaAddress,
        new_nonce_account: SolanaAddress,
        rent: u64,
        space: u64,
    ) -> Vec<Instruction> {
        let program_id = *SYSTEM_PROGRAM_ID_ADDRESS;
        vec![
            SystemInstructionBuilder::create_account(
                signer,
                new_nonce_account,
                rent,
                space,
                program_id,
            ),
            SystemInstructionBuilder::init_nonce_account(new_nonce_account, signer),
        ]
    }
}
