// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::defined_addresses::COMPUTE_BUDGET_ADDRESS;
use crate::instruction::Instruction;
use borsh::{BorshDeserialize, BorshSerialize};
use tw_encoding::{EncodingError, EncodingResult};

pub type UnitLimit = u32;
pub type UnitPrice = u64;

#[derive(BorshDeserialize, BorshSerialize)]
pub enum ComputeBudgetInstruction {
    Unused,
    RequestHeapFrame(u32),
    SetComputeUnitLimit(UnitLimit),
    SetComputeUnitPrice(UnitPrice),
    SetLoadedAccountsDataSizeLimit(u32),
}

impl ComputeBudgetInstruction {
    pub fn try_from_borsh(data: &[u8]) -> EncodingResult<Self> {
        borsh::from_slice(data).map_err(|_| EncodingError::InvalidInput)
    }
}

pub struct ComputeBudgetInstructionBuilder;

impl ComputeBudgetInstructionBuilder {
    /// Create a `ComputeBudgetInstruction::SetComputeUnitLimit` `Instruction`
    pub fn set_compute_unit_limit(units: UnitLimit) -> Instruction {
        let account_metas = vec![];
        Instruction::new_with_borsh(
            *COMPUTE_BUDGET_ADDRESS,
            &ComputeBudgetInstruction::SetComputeUnitLimit(units),
            account_metas,
        )
    }

    /// Create a `ComputeBudgetInstruction::SetComputeUnitPrice` `Instruction`
    pub fn set_compute_unit_price(micro_lamports: UnitPrice) -> Instruction {
        let account_metas = vec![];
        Instruction::new_with_borsh(
            *COMPUTE_BUDGET_ADDRESS,
            &ComputeBudgetInstruction::SetComputeUnitPrice(micro_lamports),
            account_metas,
        )
    }
}
