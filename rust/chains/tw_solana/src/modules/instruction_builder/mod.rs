// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::SolanaAddress;
use crate::defined_addresses::*;
use crate::instruction::{AccountMeta, Instruction};
use tw_memory::Data;

pub mod stake_instruction;
pub mod system_instruction;
pub mod token_instruction;

use stake_instruction::{Authorized, Lockup, StakeInstruction};
use system_instruction::SystemInstruction;
use token_instruction::TokenInstruction;

pub struct SystemInstructionBuilder;

impl SystemInstructionBuilder {
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

    // TODO add references
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
    pub fn account_with_seed(
        from_pubkey: SolanaAddress,
        to_pubkey: SolanaAddress,
        base: SolanaAddress,
        seed: String,
        lamports: u64,
        space: u64,
        owner: SolanaAddress,
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
                owner,
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
}

pub struct StakeInstructionBuilder;

impl StakeInstructionBuilder {
    /// Creates an Initialize Stake instruction.
    pub fn stake_initialize(
        stake_pubkey: SolanaAddress,
        authorized: Authorized,
        lockup: Lockup,
    ) -> Instruction {
        Instruction::new_with_bincode(
            *STAKE_PROGRAM_ID_ADDRESS,
            StakeInstruction::Initialize(authorized, lockup),
            vec![
                AccountMeta::new(stake_pubkey, false),
                AccountMeta::readonly(*SYSVAR_RENT_ID_ADDRESS, false),
            ],
        )
    }

    pub fn withdraw(
        stake_pubkey: SolanaAddress,
        withdrawer_pubkey: SolanaAddress,
        to_pubkey: SolanaAddress,
        lamports: u64,
        custodian_pubkey: Option<SolanaAddress>,
    ) -> Instruction {
        let mut account_metas = vec![
            AccountMeta::new(stake_pubkey, false),
            AccountMeta::new(to_pubkey, false),
            AccountMeta::readonly(*SYSVAR_CLOCK_ID_ADDRESS, false),
            AccountMeta::readonly(*SYSVAR_STAKE_HISTORY_ID_ADDRESS, false),
            AccountMeta::readonly(withdrawer_pubkey, true),
        ];

        if let Some(custodian_pubkey) = custodian_pubkey {
            account_metas.push(AccountMeta::readonly(custodian_pubkey, true));
        }

        Instruction::new_with_bincode(
            *STAKE_PROGRAM_ID_ADDRESS,
            StakeInstruction::Withdraw(lamports),
            account_metas,
        )
    }

    pub fn delegate(
        stake_pubkey: SolanaAddress,
        vote_pubkey: SolanaAddress,
        authorized_pubkey: SolanaAddress,
    ) -> Instruction {
        let account_metas = vec![
            AccountMeta::new(stake_pubkey, false),
            AccountMeta::readonly(vote_pubkey, false),
            AccountMeta::readonly(*SYSVAR_CLOCK_ID_ADDRESS, false),
            AccountMeta::readonly(*SYSVAR_STAKE_HISTORY_ID_ADDRESS, false),
            AccountMeta::readonly(*STAKE_CONFIG_ID_ADDRESS, false),
            AccountMeta::readonly(authorized_pubkey, true),
        ];
        Instruction::new_with_bincode(
            *STAKE_PROGRAM_ID_ADDRESS,
            StakeInstruction::DelegateStake,
            account_metas,
        )
    }
}

pub struct TokenInstructionBuilder;

impl TokenInstructionBuilder {
    // `create_associated_token_account()` solana-program-library/associated-token-account/program/src/lib.rs
    pub fn create_account(
        funding_pubkey: SolanaAddress,
        other_main_pubkey: SolanaAddress,
        token_mint_pubkey: SolanaAddress,
        token_pubkey: SolanaAddress,
    ) -> Instruction {
        let account_metas = vec![
            AccountMeta::new(funding_pubkey, true),
            AccountMeta::new(token_pubkey, false),
            AccountMeta::readonly(other_main_pubkey, false),
            AccountMeta::readonly(token_mint_pubkey, false),
            AccountMeta::readonly(*SYSTEM_PROGRAM_ID_ADDRESS, false),
            AccountMeta::readonly(*TOKEN_PROGRAM_ID_ADDRESS, false),
            AccountMeta::readonly(*SYSVAR_RENT_ID_ADDRESS, false),
        ];
        let data = Data::default();
        Instruction::new(*ASSOCIATED_TOKEN_PROGRAM_ID_ADDRESS, data, account_metas)
    }

    /// transfer_checked() solana-program-library/token/program/src/instruction.rs
    pub fn transfer_checked(
        sender_token_pubkey: SolanaAddress,
        token_mint_pubkey: SolanaAddress,
        recipient_token_pubkey: SolanaAddress,
        signer: SolanaAddress,
        amount: u64,
        decimals: u8,
        references: Vec<SolanaAddress>,
    ) -> Instruction {
        let mut account_metas = vec![
            AccountMeta::new(sender_token_pubkey, false),
            AccountMeta::readonly(token_mint_pubkey, false),
            AccountMeta::new(recipient_token_pubkey, false),
            AccountMeta::new(signer, true),
        ];
        append_references(&mut account_metas, references);
        Instruction::new_with_bincode(
            *TOKEN_PROGRAM_ID_ADDRESS,
            TokenInstruction::TransferChecked { amount, decimals },
            account_metas,
        )
    }
}

fn append_references(account_metas: &mut Vec<AccountMeta>, references: Vec<SolanaAddress>) {
    for reference in references {
        account_metas.push(AccountMeta::readonly(reference, false));
    }
}
