// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::SolanaAddress;
use crate::transaction::versioned::VersionedMessage;
use crate::transaction::CompiledInstruction;
use tw_coin_entry::error::prelude::*;
use tw_memory::Data;

/// [`Instruction`] without `accounts` field.
pub struct InstructionWithoutAccounts {
    /// Pubkey of the program that executes this instruction.
    pub program_id: SolanaAddress,
    /// Opaque data passed to the program for its own interpretation.
    pub data: Data,
}

pub struct MessageDecompiler;

impl MessageDecompiler {
    pub fn decompile_partly(
        message: &VersionedMessage,
    ) -> SigningResult<Vec<InstructionWithoutAccounts>> {
        match message {
            VersionedMessage::Legacy(legacy) => {
                Self::decompile_partly_impl(&legacy.instructions, &legacy.account_keys)
            },
            VersionedMessage::V0(v0) => {
                Self::decompile_partly_impl(&v0.instructions, &v0.account_keys)
            },
        }
    }

    fn decompile_partly_impl(
        instructions: &[CompiledInstruction],
        account_keys: &[SolanaAddress],
    ) -> SigningResult<Vec<InstructionWithoutAccounts>> {
        instructions
            .iter()
            .map(|ix| Self::decompile_instruction_partly(ix, account_keys))
            .collect()
    }

    fn decompile_instruction_partly(
        ix: &CompiledInstruction,
        account_keys: &[SolanaAddress],
    ) -> SigningResult<InstructionWithoutAccounts> {
        // Program ID should always be in the transaction's accounts list even if AddressLookupTable is used:
        // https://solana.stackexchange.com/questions/16122/using-program-ids-in-address-lookup-tables-missing-documentation-about-luts
        let program_id = *account_keys
            .get(ix.program_id_index as usize)
            .or_tw_err(SigningErrorType::Error_invalid_params)
            .context("Program ID not found in the accounts list")?;
        Ok(InstructionWithoutAccounts {
            program_id,
            data: ix.data.clone(),
        })
    }
}
