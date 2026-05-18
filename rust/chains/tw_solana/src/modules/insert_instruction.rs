// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::SolanaAddress;
use crate::instruction::AccountMeta;
use crate::modules::compiled_keys::try_into_u8;
use crate::transaction::v0::MessageAddressTableLookup;
use crate::transaction::{CompiledInstruction, MessageHeader};
use std::iter;
use tw_coin_entry::error::prelude::*;
use tw_memory::Data;

fn increment_counter(counter: &mut u8) -> SigningResult<()> {
    *counter = counter
        .checked_add(1)
        .ok_or(SigningErrorType::Error_tx_too_big)?;
    Ok(())
}

pub trait InsertInstruction: Clone + Sized {
    // =========================================================================
    // Public API
    // Each method clones `self`, runs all mutations on the clone, and returns
    // the updated clone on success.  On error the clone is dropped and `self`
    // is left completely untouched.
    // =========================================================================

    fn with_instruction(
        &self,
        program_id: SolanaAddress,
        accounts: Vec<AccountMeta>,
        data: Data,
    ) -> SigningResult<Self> {
        let mut draft = self.clone();
        let insert_at = draft.instructions_mut().len();
        draft.insert_instruction_mut(insert_at, program_id, accounts, data)?;
        Ok(draft)
    }

    fn with_instruction_at(
        &self,
        insert_at: usize,
        program_id: SolanaAddress,
        accounts: Vec<AccountMeta>,
        data: Data,
    ) -> SigningResult<Self> {
        let mut draft = self.clone();
        draft.insert_instruction_mut(insert_at, program_id, accounts, data)?;
        Ok(draft)
    }

    fn with_simple_instruction(
        &self,
        program_id: SolanaAddress,
        data: Data,
    ) -> SigningResult<Self> {
        let mut draft = self.clone();
        let insert_at = draft.instructions_mut().len();
        draft.insert_simple_instruction_mut(insert_at, program_id, data)?;
        Ok(draft)
    }

    fn with_simple_instruction_at(
        &self,
        insert_at: usize,
        program_id: SolanaAddress,
        data: Data,
    ) -> SigningResult<Self> {
        let mut draft = self.clone();
        draft.insert_simple_instruction_mut(insert_at, program_id, data)?;
        Ok(draft)
    }

    fn with_fee_payer(&self, account: SolanaAddress) -> SigningResult<Self> {
        let mut draft = self.clone();
        draft.set_fee_payer_mut(account)?;
        Ok(draft)
    }

    // =========================================================================
    // Internal mutation helpers
    // These operate on `&mut self` and may leave the message in a partially
    // mutated state when they return `Err`.  They must only be called on a
    // draft that will be discarded on error (see the public wrappers above).
    // =========================================================================

    fn insert_instruction_mut(
        &mut self,
        insert_at: usize,
        program_id: SolanaAddress,
        accounts: Vec<AccountMeta>,
        data: Data,
    ) -> SigningResult<()> {
        if insert_at > self.instructions_mut().len() {
            return SigningError::err(SigningErrorType::Error_internal)
            .context(format!("Unable to add '{program_id}' instruction at the '{insert_at}' index. Number of existing instructions: {}", self.instructions_mut().len()));
        }

        // Step 1 - add the `account` in the accounts list.
        let accounts: Vec<u8> = accounts
            .iter()
            .map(|account_meta| self.push_account_mut(account_meta))
            .collect::<Result<Vec<u8>, _>>()?;

        // Step 2 - find or add the `program_id` in the accounts list.
        let program_id_index = match self
            .account_keys_mut()
            .iter()
            .position(|acc| *acc == program_id)
        {
            Some(pos) => try_into_u8(pos)?,
            None => self.push_readonly_unsigned_account_mut(program_id)?,
        };

        // Step 3 - Create a `CompiledInstruction` based on the `program_id` index and instruction `accounts` and `data`.
        let new_compiled_ix = CompiledInstruction {
            program_id_index,
            accounts,
            data,
        };

        // Step 4 - Insert the created instruction at the given `insert_at` index.
        self.instructions_mut().insert(insert_at, new_compiled_ix);

        Ok(())
    }

    fn insert_simple_instruction_mut(
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
            None => self.push_readonly_unsigned_account_mut(program_id)?,
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

    /// Pushes an account to the message.
    /// If the account already exists, it must match the `is_signer` and `is_writable` attributes.
    /// Returns the index of the account in the account list.
    fn push_account_mut(&mut self, account: &AccountMeta) -> SigningResult<u8> {
        // The layout of the account keys is as follows:
        // +-------------------------------------+
        // | Writable and required signature     |                                     \
        // +-------------------------------------+                                      |->  num_required_signatures
        // | Readonly and required signature     | --> num_readonly_signed_accounts    /
        // +-------------------------------------+
        // | Writable and not required signature |
        // +-------------------------------------+
        // | Readonly and not required signature | --> num_readonly_unsigned_accounts
        // +-------------------------------------+

        // Check if the account already exists in `account_keys`,
        // if it does, validate `is_signer` and `is_writable` match
        if let Some(existing_index) = self
            .account_keys_mut()
            .iter()
            .position(|key| *key == account.pubkey)
        {
            let existing_is_signer =
                existing_index < self.message_header_mut().num_required_signatures as usize;

            let existing_is_writable = if existing_is_signer {
                existing_index
                    < (self.message_header_mut().num_required_signatures
                        - self.message_header_mut().num_readonly_signed_accounts)
                        as usize
            } else {
                existing_index
                    < (self.account_keys_mut().len()
                        - self.message_header_mut().num_readonly_unsigned_accounts as usize)
            };

            match (existing_is_signer, account.is_signer) {
                // If new account requires weaker or the same signing permissions, it's ok.
                (true, false) | (false, false) | (true, true) => (),
                // If new account requires stronger signing permissions than we have already, then would need to reorder accounts.
                // TODO: Implement reordering accounts if needed.
                (false, true) => {
                    return SigningError::err(SigningErrorType::Error_internal).context(
                        "Account already exists but the `is_signer` attribute does not match",
                    );
                },
            }

            match (existing_is_writable, account.is_writable) {
                // If new account requires weaker or the same writable permissions, it's ok.
                (true, false) | (false, false) | (true, true) => (),
                // If new account requires stronger writable permissions than we have already, then would need to reorder accounts.
                // TODO: Implement reordering accounts if needed.
                (false, true) => {
                    return SigningError::err(SigningErrorType::Error_internal).context(
                        "Account already exists but the `is_writable` attribute does not match",
                    );
                },
            }

            // Return the existing index if validation passes
            return try_into_u8(existing_index);
        }

        let accounts_number_before = self.account_keys_mut().len();

        // 255u8 + 1 wraps to 0 in release builds; reject before header state is mutated.
        if accounts_number_before > u8::MAX as usize {
            return SigningError::err(SigningErrorType::Error_tx_too_big)
                .context("There are too many accounts in the transaction");
        }

        // Determine the insertion position based on is_signer and is_writable
        let insert_at = match (account.is_signer, account.is_writable) {
            (true, true) => {
                increment_counter(&mut self.message_header_mut().num_required_signatures)?;
                // The account is added at the end of the writable and signer accounts
                (self.message_header_mut().num_required_signatures
                    - self.message_header_mut().num_readonly_signed_accounts)
                    as usize
                    - 1
            },
            (true, false) => {
                increment_counter(&mut self.message_header_mut().num_required_signatures)?;
                increment_counter(&mut self.message_header_mut().num_readonly_signed_accounts)?;
                // The account is added at the end of the read-only and signer accounts
                self.message_header_mut().num_required_signatures as usize - 1
            },
            (false, true) => {
                // The account is added at the end of the writable and non-signer accounts
                accounts_number_before
                    .checked_sub(self.message_header_mut().num_readonly_unsigned_accounts as usize)
                    .ok_or(SigningErrorType::Error_internal)?
            },
            (false, false) => {
                increment_counter(&mut self.message_header_mut().num_readonly_unsigned_accounts)?;
                // The account is added at the end of the list
                accounts_number_before
            },
        };

        // Insert the account at the determined position
        self.account_keys_mut().insert(insert_at, account.pubkey);

        let account_added_at = try_into_u8(insert_at)?;

        // Update program ID and account indexes if the new account was added before its position
        let instructions = self.instructions_mut();
        instructions
            .iter_mut()
            .try_for_each(|ix| -> SigningResult<()> {
                // Update program ID index
                if ix.program_id_index >= account_added_at {
                    ix.program_id_index = ix
                        .program_id_index
                        .checked_add(1)
                        .ok_or(SigningErrorType::Error_tx_too_big)?;
                }

                // Update account indexes
                ix.accounts
                    .iter_mut()
                    .filter(|ix_account_id| **ix_account_id >= account_added_at)
                    .try_for_each(|ix_account_id| -> SigningResult<()> {
                        *ix_account_id = ix_account_id
                            .checked_add(1)
                            .ok_or(SigningErrorType::Error_tx_too_big)?;
                        Ok(())
                    })
            })?;

        Ok(account_added_at)
    }

    fn push_readonly_unsigned_account_mut(&mut self, account: SolanaAddress) -> SigningResult<u8> {
        debug_assert!(
            !self.account_keys_mut().contains(&account),
            "Account must not be in the account list yet"
        );

        self.account_keys_mut().push(account);
        increment_counter(&mut self.message_header_mut().num_readonly_unsigned_accounts)?;

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
            .try_for_each(|ix_account_id| -> SigningResult<()> {
                *ix_account_id = ix_account_id
                    .checked_add(1)
                    .ok_or(SigningErrorType::Error_tx_too_big)?;
                Ok(())
            })?;

        Ok(account_added_at)
    }

    /// Adds a fee payer account to the message.
    /// Note: The fee payer must NOT be in the account list yet.
    fn set_fee_payer_mut(&mut self, account: SolanaAddress) -> SigningResult<()> {
        if self.account_keys_mut().contains(&account) {
            // For security reasons, we don't allow adding a fee payer if it's already in the account list.
            //
            // If the fee payer is already in the transaction and there is a malicious instruction to
            // transfer tokens from the fee payer to another account, The fee payer may have inadvertently
            // signed off on such transactions, which is not what they would expect.
            //
            // Such examples may be difficult to exploit, but we still took precautionary measures to prohibit
            // the new fee payer from appearing in the account list of the transaction out of caution
            return SigningError::err(SigningErrorType::Error_internal)
                .context("Fee payer account is already in the account list");
        }

        if self.account_keys_mut().len() > u8::MAX as usize {
            return SigningError::err(SigningErrorType::Error_tx_too_big)
                .context("There are too many accounts in the transaction");
        }

        // Insert the fee payer account at the beginning of the account list.
        self.account_keys_mut().insert(0, account);
        increment_counter(&mut self.message_header_mut().num_required_signatures)?;

        // Update `program id indexes` and `account id indexes` in every instruction as we inserted the account at the beginning of the list.
        self.instructions_mut()
            .iter_mut()
            .try_for_each(|ix| -> SigningResult<()> {
                ix.program_id_index = ix
                    .program_id_index
                    .checked_add(1)
                    .ok_or(SigningErrorType::Error_tx_too_big)?;
                ix.accounts
                    .iter_mut()
                    .try_for_each(|account_id| -> SigningResult<()> {
                        *account_id = account_id
                            .checked_add(1)
                            .ok_or(SigningErrorType::Error_tx_too_big)?;
                        Ok(())
                    })
            })?;

        Ok(())
    }

    // =========================================================================
    // Abstract methods — implementors must provide these.
    // =========================================================================

    /// Returns ALT (Address Lookup Tables) if supported by the message version.
    fn address_table_lookups(&self) -> Option<&[MessageAddressTableLookup]>;

    fn account_keys_mut(&mut self) -> &mut Vec<SolanaAddress>;

    fn message_header_mut(&mut self) -> &mut MessageHeader;

    fn instructions_mut(&mut self) -> &mut Vec<CompiledInstruction>;
}
