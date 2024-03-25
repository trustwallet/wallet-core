// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::SolanaAddress;
use crate::instruction::Instruction;

pub mod compute_budget_instruction;
pub mod stake_instruction;
pub mod system_instruction;
pub mod token_instruction;

use compute_budget_instruction::ComputeBudgetInstructionBuilder;
use system_instruction::SystemInstructionBuilder;

#[derive(Default)]
pub struct InstructionBuilder {
    instructions: Vec<Instruction>,
}

impl InstructionBuilder {
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

    /// Adds a `priority_fee` instruction if it is provided.
    pub fn maybe_priority_fee_price(&mut self, micro_lamports: Option<u64>) -> &mut Self {
        if let Some(micro_lamports) = micro_lamports {
            let ix = ComputeBudgetInstructionBuilder::set_compute_unit_price(micro_lamports);
            self.instructions.push(ix);
        }
        self
    }

    /// Adds a `fee_limit` instruction if it is provided.
    pub fn maybe_priority_fee_limit(&mut self, units: Option<u32>) -> &mut Self {
        if let Some(units) = units {
            let ix = ComputeBudgetInstructionBuilder::set_compute_unit_limit(units);
            self.instructions.push(ix);
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
