// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::SolanaAddress;
use crate::defined_addresses::*;
use crate::instruction::{AccountMeta, Instruction};
use std::mem::size_of;
use tw_memory::Data;

/// Instructions supported by the token program.
#[derive(Debug, Clone, PartialEq, Eq)]
pub enum TokenInstruction {
    /// Transfers tokens from one account to another either directly or via a
    /// delegate.  If this account is associated with the native mint then equal
    /// amounts of SOL and Tokens will be transferred to the destination
    /// account.
    ///
    /// This instruction differs from Transfer in that the token mint and
    /// decimals value is checked by the caller.  This may be useful when
    /// creating transactions offline or within a hardware wallet.
    ///
    /// Accounts expected by this instruction:
    ///
    ///   * Single owner/delegate
    ///   0. `[writable]` The source account.
    ///   1. `[]` The token mint.
    ///   2. `[writable]` The destination account.
    ///   3. `[signer]` The source account's owner/delegate.
    ///
    ///   * Multisignature owner/delegate
    ///   0. `[writable]` The source account.
    ///   1. `[]` The token mint.
    ///   2. `[writable]` The destination account.
    ///   3. `[]` The source account's multisignature owner/delegate.
    ///   4. ..4+M `[signer]` M signer accounts.
    TransferChecked {
        /// The amount of tokens to transfer.
        amount: u64,
        /// Expected number of base 10 digits to the right of the decimal place.
        decimals: u8,
    },
}

impl TokenInstruction {
    /// Packs a [TokenInstruction](enum.TokenInstruction.html) into a byte
    /// buffer.
    ///
    /// # Important
    ///
    /// This serialization method differs from [`bincode::serialize`]:
    /// It needs to serialize enum `type` as u8 (1 byte), but [`bincode::serialize`] serializes the type as u32 (4 bytes).
    pub fn pack(&self) -> Vec<u8> {
        let mut buf = Vec::with_capacity(size_of::<Self>());
        match self {
            &Self::TransferChecked { amount, decimals } => {
                // https://github.com/solana-labs/solana-program-library/blob/5418cf9b90d5c9ff5bff9f55fd17651f66c98902/token/program-2022/src/instruction.rs#L334-L339
                // https://github.com/trustwallet/wallet-core/blob/cd5a27481d2181e63362cb57e2b2160506cce163/src/Solana/Instruction.h#L37
                buf.push(12);
                buf.extend_from_slice(&amount.to_le_bytes());
                buf.push(decimals);
            },
        };
        buf
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
        token_program_id: SolanaAddress,
    ) -> Instruction {
        let account_metas = vec![
            AccountMeta::new(funding_pubkey, true),
            AccountMeta::new(token_pubkey, false),
            AccountMeta::readonly(other_main_pubkey, false),
            AccountMeta::readonly(token_mint_pubkey, false),
            AccountMeta::readonly(*SYSTEM_PROGRAM_ID_ADDRESS, false),
            AccountMeta::readonly(token_program_id, false),
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
        token_program_id: SolanaAddress,
    ) -> Instruction {
        let account_metas = vec![
            AccountMeta::new(sender_token_pubkey, false),
            AccountMeta::readonly(token_mint_pubkey, false),
            AccountMeta::new(recipient_token_pubkey, false),
            AccountMeta::new(signer, true),
        ];

        let data = TokenInstruction::TransferChecked { amount, decimals }.pack();
        Instruction::new(token_program_id, data, account_metas)
    }
}
