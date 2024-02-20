// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::SolanaAddress;
use crate::instruction::Instruction;

pub mod stake_instruction;
pub mod system_instruction;
pub mod token_instruction;

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
