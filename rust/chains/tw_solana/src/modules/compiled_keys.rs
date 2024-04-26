// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

//! Original source code: https://github.com/solana-labs/solana/blob/4b65cc8eef6ef79cb9b9cbc534a99b4900e58cf7/sdk/program/src/message/compiled_keys.rs

use crate::address::SolanaAddress;
use crate::instruction::Instruction;
use crate::transaction::MessageHeader;
use std::collections::hash_map::Entry;
use std::collections::HashMap;
use tw_coin_entry::error::prelude::*;

#[derive(Default, Debug, Clone, Copy, PartialEq, Eq)]
struct CompiledKeyMeta {
    is_signer: bool,
    is_writable: bool,
}

#[derive(Debug, Default, Clone, PartialEq, Eq)]
pub(crate) struct CompiledKeys {
    ordered_keys: Vec<SolanaAddress>,
    key_meta_map: HashMap<SolanaAddress, CompiledKeyMeta>,
}

impl CompiledKeys {
    pub fn with_fee_payer(fee_payer: SolanaAddress) -> Self {
        let mut selfi = Self::default();

        selfi.key_meta_map.insert(
            fee_payer,
            CompiledKeyMeta {
                is_signer: true,
                is_writable: true,
            },
        );
        // Fee payer must be the first account in the keys list.
        selfi.ordered_keys.push(fee_payer);

        selfi
    }

    pub fn compile(mut self, instructions: &[Instruction]) -> Self {
        for ix in instructions {
            for account_meta in &ix.accounts {
                let meta_entry = self.key_meta_map.entry(account_meta.pubkey);
                if matches!(meta_entry, Entry::Vacant(_)) {
                    self.ordered_keys.push(account_meta.pubkey);
                }

                let meta = meta_entry.or_default();
                meta.is_signer |= account_meta.is_signer;
                meta.is_writable |= account_meta.is_writable;
            }
        }

        // add programIds (read-only, at end)
        for ix in instructions {
            let meta_entry = self.key_meta_map.entry(ix.program_id);
            if matches!(meta_entry, Entry::Vacant(_)) {
                self.ordered_keys.push(ix.program_id);
            }
            meta_entry.or_default();
        }

        self
    }

    pub fn try_into_message_components(self) -> SigningResult<(MessageHeader, Vec<SolanaAddress>)> {
        let try_into_u8 = |num: usize| -> SigningResult<u8> {
            u8::try_from(num).tw_err(|_| SigningErrorType::Error_tx_too_big)
        };

        let Self {
            ordered_keys,
            key_meta_map,
        } = self;

        let filter = |account, is_signer: bool, is_writable: bool| -> Option<SolanaAddress> {
            let meta = key_meta_map.get(account).copied().unwrap_or_default();
            (meta.is_signer == is_signer && meta.is_writable == is_writable).then_some(*account)
        };

        let writable_signer_keys: Vec<SolanaAddress> = ordered_keys
            .iter()
            .filter_map(|key| filter(key, true, true))
            .collect();
        let readonly_signer_keys: Vec<SolanaAddress> = ordered_keys
            .iter()
            .filter_map(|key| filter(key, true, false))
            .collect();
        let writable_non_signer_keys: Vec<SolanaAddress> = ordered_keys
            .iter()
            .filter_map(|key| filter(key, false, true))
            .collect();
        let readonly_non_signer_keys: Vec<SolanaAddress> = ordered_keys
            .iter()
            .filter_map(|key| filter(key, false, false))
            .collect();

        let signers_len = writable_signer_keys
            .len()
            .saturating_add(readonly_signer_keys.len());

        let header = MessageHeader {
            num_required_signatures: try_into_u8(signers_len)
                .context("Too many signatures required")?,
            num_readonly_signed_accounts: try_into_u8(readonly_signer_keys.len())
                .context("Too many accounts in the transaction")?,
            num_readonly_unsigned_accounts: try_into_u8(readonly_non_signer_keys.len())
                .context("Too many accounts in the transaction")?,
        };

        let static_account_keys: Vec<_> = std::iter::empty()
            .chain(writable_signer_keys)
            .chain(readonly_signer_keys)
            .chain(writable_non_signer_keys)
            .chain(readonly_non_signer_keys)
            .collect();

        Ok((header, static_account_keys))
    }
}
