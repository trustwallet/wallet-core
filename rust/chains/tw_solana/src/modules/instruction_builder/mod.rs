// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::SolanaAddress;
use crate::blockhash::Blockhash;
use crate::instruction::Instruction;
use crate::program::stake_program::StakeProgram;
use tw_coin_entry::error::SigningResult;

pub mod stake_instruction;
pub mod system_instruction;
pub mod token_instruction;

use stake_instruction::{Authorized, Lockup, StakeInstructionBuilder};
use system_instruction::SystemInstructionBuilder;

// TODO rename to `InstructionBuilder`.
#[derive(Default)]
pub struct InstructionBuilder1 {
    instructions: Vec<Instruction>,
}

impl InstructionBuilder1 {
    /// Adds an `advance_nonce` instruction if the nonce account provided.
    pub fn maybe_advance_nonce(
        &mut self,
        nonce_account: Option<SolanaAddress>,
        authorized_account: SolanaAddress,
    ) -> &mut Self {
        if let Some(nonce_account) = nonce_account {
            self.instructions
                .push(SystemInstructionBuilder::advance_nonce_account(
                    nonce_account,
                    authorized_account,
                ));
        }
        self
    }

    /// Adds a `memo` instruction if it is not empty.
    pub fn maybe_memo(&mut self, memo: &str) -> &mut Self {
        if !memo.is_empty() {
            self.instructions.push(SystemInstructionBuilder::memo(memo));
        }
        self
    }

    pub fn add_instruction(&mut self, instruction: Instruction) -> &mut Self {
        self.instructions.push(instruction);
        self
    }

    pub fn add_instructions<I>(&mut self, instructions: I) -> &mut Self
    where
        I: IntoIterator<Item = Instruction>,
    {
        self.instructions.extend(instructions);
        self
    }

    pub fn output(self) -> Vec<Instruction> {
        self.instructions
    }
}

pub struct DepositStakeArgs {
    pub sender: SolanaAddress,
    pub validator: SolanaAddress,
    pub stake_account: Option<SolanaAddress>,
    pub recent_blockhash: Blockhash,
    pub lamports: u64,
    pub space: u64,
}

/// Consists of complex instruction builders.
pub struct InstructionBuilder;

impl InstructionBuilder {
    // TODO move to `StakeInstructionBuilder`.
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
            ),
            StakeInstructionBuilder::stake_initialize(stake_addr, authorized, lockup),
            StakeInstructionBuilder::delegate(stake_addr, args.validator, args.sender),
        ])
    }
}
