// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::SolanaAddress;
use crate::modules::compiled_keys::try_into_u8;
use crate::transaction::v0::MessageAddressTableLookup;
use crate::transaction::{CompiledInstruction, MessageHeader};
use std::iter;
use tw_coin_entry::error::prelude::*;
use tw_memory::Data;

pub trait InsertInstruction {
    /// Pushes a simple instruction that doesn't have accounts.
    fn push_simple_instruction(
        &mut self,
        program_id: SolanaAddress,
        data: Data,
    ) -> SigningResult<()> {
        let insert_at = self.instructions_mut().len();
        self.insert_simple_instruction(insert_at, program_id, data)
    }

    /// Inserts a simple instruction that doesn't have accounts at the given `insert_at` index.
    fn insert_simple_instruction(
        &mut self,
        insert_at: usize,
        program_id: SolanaAddress,
        data: Data,
    ) -> SigningResult<()> {
        if insert_at > self.instructions_mut().len() {
            return SigningError::err(SigningErrorType::Error_internal)
                .context(format!("Unable to add '{program_id}' instruction at the '{insert_at}' index. Number of existing instructions: {}", self.instructions_mut().len()));
        }

        // Step 1 - find or add the `program_id` in the accounts list.
        let program_id_index = match self
            .account_keys_mut()
            .iter()
            .position(|acc| *acc == program_id)
        {
            Some(pos) => try_into_u8(pos)?,
            None => self.push_readonly_unsigned_account(program_id)?,
        };

        // Step 2 - Create a `CompiledInstruction` based on the `program_id` index and instruction `data`.
        let new_compiled_ix = CompiledInstruction {
            program_id_index,
            accounts: Vec::default(),
            data,
        };

        // Step 3 - Insert the created instruction at the given `insert_at` index.
        self.instructions_mut().insert(insert_at, new_compiled_ix);

        Ok(())
    }

    fn push_readonly_unsigned_account(&mut self, account: SolanaAddress) -> SigningResult<u8> {
        debug_assert!(
            !self.account_keys_mut().contains(&account),
            "Account must not be in the account list yet"
        );

        self.account_keys_mut().push(account);
        self.message_header_mut().num_readonly_unsigned_accounts += 1;

        let account_added_at = try_into_u8(self.account_keys_mut().len() - 1)?;

        // There is no need to update instruction account ids that point to [`Message::account_keys`] list
        // as we pushed the account to the end of the list.
        // But we must update it in case if there are instruction account ids that point to [`address_table_lookups`].

        match self.address_table_lookups() {
            Some(lookups) if !lookups.is_empty() => (),
            // No address table lookups, no need to update the indexes.
            _ => return Ok(account_added_at),
        }

        self.instructions_mut()
            .iter_mut()
            .flat_map(|ix| {
                ix.accounts
                    .iter_mut()
                    .chain(iter::once(&mut ix.program_id_index))
            })
            // Update every instruction account id that points to the address table lookups.
            .filter(|ix_account_id| **ix_account_id >= account_added_at)
            .for_each(|ix_account_id| *ix_account_id += 1);

        Ok(account_added_at)
    }

    /// Returns ALT (Address Lookup Tables) if supported by the message version.
    fn address_table_lookups(&self) -> Option<&[MessageAddressTableLookup]>;

    fn account_keys_mut(&mut self) -> &mut Vec<SolanaAddress>;

    fn message_header_mut(&mut self) -> &mut MessageHeader;

    fn instructions_mut(&mut self) -> &mut Vec<CompiledInstruction>;
}
