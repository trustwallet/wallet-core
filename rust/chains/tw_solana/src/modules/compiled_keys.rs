// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

//! Source: https://github.com/solana-labs/solana/blob/4b65cc8eef6ef79cb9b9cbc534a99b4900e58cf7/sdk/program/src/message/compiled_keys.rs

use crate::address::SolanaAddress;
use crate::instruction::Instruction;
use crate::transaction::MessageHeader;
use std::collections::BTreeMap;
use tw_coin_entry::error::{SigningError, SigningErrorType, SigningResult};

#[derive(Default, Debug, Clone, PartialEq, Eq)]
struct CompiledKeyMeta {
    is_signer: bool,
    is_writable: bool,
    is_invoked: bool,
}

#[derive(Default, Debug, Clone, PartialEq, Eq)]
pub(crate) struct CompiledKeys {
    payer: Option<SolanaAddress>,
    key_meta_map: BTreeMap<SolanaAddress, CompiledKeyMeta>,
}

impl CompiledKeys {
    pub(crate) fn compile(instructions: &[Instruction], payer: Option<SolanaAddress>) -> Self {
        let mut key_meta_map = BTreeMap::<SolanaAddress, CompiledKeyMeta>::new();
        for ix in instructions {
            let meta = key_meta_map.entry(ix.program_id).or_default();
            meta.is_invoked = true;
            for account_meta in &ix.accounts {
                let meta = key_meta_map.entry(account_meta.pubkey).or_default();
                meta.is_signer |= account_meta.is_signer;
                meta.is_writable |= account_meta.is_writable;
            }
        }
        if let Some(payer) = &payer {
            let meta = key_meta_map.entry(*payer).or_default();
            meta.is_signer = true;
            meta.is_writable = true;
        }
        Self {
            payer,
            key_meta_map,
        }
    }

    pub(crate) fn try_into_message_components(
        self,
    ) -> SigningResult<(MessageHeader, Vec<SolanaAddress>)> {
        let try_into_u8 = |num: usize| -> SigningResult<u8> {
            u8::try_from(num).map_err(|_| SigningError(SigningErrorType::Error_tx_too_big))
        };

        let Self {
            payer,
            mut key_meta_map,
        } = self;

        if let Some(payer) = &payer {
            key_meta_map.remove_entry(payer);
        }

        let writable_signer_keys: Vec<SolanaAddress> = payer
            .into_iter()
            .chain(
                key_meta_map
                    .iter()
                    .filter_map(|(key, meta)| (meta.is_signer && meta.is_writable).then_some(*key)),
            )
            .collect();
        let readonly_signer_keys: Vec<SolanaAddress> = key_meta_map
            .iter()
            .filter_map(|(key, meta)| (meta.is_signer && !meta.is_writable).then_some(*key))
            .collect();
        let writable_non_signer_keys: Vec<SolanaAddress> = key_meta_map
            .iter()
            .filter_map(|(key, meta)| (!meta.is_signer && meta.is_writable).then_some(*key))
            .collect();
        let readonly_non_signer_keys: Vec<SolanaAddress> = key_meta_map
            .iter()
            .filter_map(|(key, meta)| (!meta.is_signer && !meta.is_writable).then_some(*key))
            .collect();

        let signers_len = writable_signer_keys
            .len()
            .saturating_add(readonly_signer_keys.len());

        let header = MessageHeader {
            num_required_signatures: try_into_u8(signers_len)?,
            num_readonly_signed_accounts: try_into_u8(readonly_signer_keys.len())?,
            num_readonly_unsigned_accounts: try_into_u8(readonly_non_signer_keys.len())?,
        };

        let static_account_keys = std::iter::empty()
            .chain(writable_signer_keys)
            .chain(readonly_signer_keys)
            .chain(writable_non_signer_keys)
            .chain(readonly_non_signer_keys)
            .collect();

        Ok((header, static_account_keys))
    }
}
