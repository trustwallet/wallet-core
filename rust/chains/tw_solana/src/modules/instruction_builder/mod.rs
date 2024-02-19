// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::SolanaAddress;
use crate::blockhash::Blockhash;
use crate::instruction::{AccountMeta, Instruction};
use crate::program::stake_program::StakeProgram;
use tw_coin_entry::error::SigningResult;

pub mod stake_instruction;
pub mod system_instruction;
pub mod token_instruction;

use stake_instruction::{Authorized, Lockup, StakeInstructionBuilder};
use system_instruction::SystemInstructionBuilder;
// use token_instruction::TokenInstruction;

pub struct TransferArgs {
    pub from: SolanaAddress,
    pub to: SolanaAddress,
    pub lamports: u64,
    pub recent_blockhash: Blockhash,
    pub memo: String,
    pub references: Vec<SolanaAddress>,
    pub nonce_account: Option<SolanaAddress>,
}

pub struct DepositStakeArgs {
    pub sender: SolanaAddress,
    pub validator: SolanaAddress,
    pub stake_account: Option<SolanaAddress>,
    pub recent_blockhash: Blockhash,
    pub lamports: u64,
    pub space: u64,
    pub owner_program_id: SolanaAddress,
}

pub struct InstructionBuilder;

impl InstructionBuilder {
    /// Builds all necessary instructions including optional nonce account advance and transfer memo.
    pub fn transfer(args: TransferArgs) -> SigningResult<Vec<Instruction>> {
        let mut instructions = Vec::default();
        if let Some(nonce_account) = args.nonce_account {
            instructions.push(SystemInstructionBuilder::advance_nonce_account(
                nonce_account,
                args.from,
            ));
        }

        if !args.memo.is_empty() {
            // Optional memo. Order: before transfer, as per documentation.
            instructions.push(SystemInstructionBuilder::memo(&args.memo));
        }

        instructions.push(SystemInstructionBuilder::transfer(
            args.from,
            args.to,
            args.lamports,
            args.references,
        ));
        Ok(instructions)
    }

    pub fn deposit_stake(args: DepositStakeArgs) -> SigningResult<Vec<Instruction>> {
        let stake_addr = args.stake_account.unwrap_or_else(|| {
            // no stake address specified, generate a new unique
            StakeProgram::address_from_recent_blockhash(args.sender, args.recent_blockhash)
        });
        let seed = args.recent_blockhash.to_string();

        let authorized = Authorized {
            staker: args.sender,
            withdrawer: args.sender,
        };
        let lockup = Lockup::default();

        Ok(vec![
            SystemInstructionBuilder::account_with_seed(
                args.sender,
                stake_addr,
                args.sender,
                seed,
                args.lamports,
                args.space,
                args.owner_program_id,
            ),
            StakeInstructionBuilder::stake_initialize(stake_addr, authorized, lockup),
            StakeInstructionBuilder::delegate(stake_addr, args.validator, args.sender),
        ])
    }
}

fn append_references(account_metas: &mut Vec<AccountMeta>, references: Vec<SolanaAddress>) {
    for reference in references {
        account_metas.push(AccountMeta::readonly(reference, false));
    }
}
