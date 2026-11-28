// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::SolanaAddress;
use crate::defined_addresses::MPL_CORE_PROGRAM_ID_ADDRESS;
use crate::instruction::{AccountMeta, Instruction};

/// `TransferV1` discriminator.
/// https://github.com/metaplex-foundation/mpl-core/blob/main/clients/rust/src/generated/instructions/transfer_v1.rs
const TRANSFER_V1: u8 = 14;
/// Borsh-encoded `None` of the `compression_proof` argument. Compressed assets are not supported.
const NO_COMPRESSION_PROOF: u8 = 0;

pub struct MplCoreInstructionBuilder;

impl MplCoreInstructionBuilder {
    /// `TransferV1` changes the owner of the Core asset.
    ///
    /// Accounts: asset (writable), collection (optional), payer (writable signer),
    /// authority (optional signer), new owner, system program (optional), log wrapper (optional).
    /// Omitted optional accounts are replaced with the Core program ID, as the official client does.
    pub fn transfer_v1(
        asset: SolanaAddress,
        collection: Option<SolanaAddress>,
        payer: SolanaAddress,
        authority: SolanaAddress,
        new_owner: SolanaAddress,
    ) -> Instruction {
        let account_metas = vec![
            AccountMeta::new(asset, false),
            optional_account(collection),
            AccountMeta::new(payer, true),
            AccountMeta::readonly(authority, true),
            AccountMeta::readonly(new_owner, false),
            // System program.
            optional_account(None),
            // Log wrapper.
            optional_account(None),
        ];
        let data = vec![TRANSFER_V1, NO_COMPRESSION_PROOF];
        Instruction::new(*MPL_CORE_PROGRAM_ID_ADDRESS, data, account_metas)
    }
}

/// An omitted optional account is encoded as the program ID itself.
fn optional_account(address: Option<SolanaAddress>) -> AccountMeta {
    AccountMeta::readonly(address.unwrap_or(*MPL_CORE_PROGRAM_ID_ADDRESS), false)
}

#[cfg(test)]
mod tests {
    use super::*;

    fn addr(s: &'static str) -> SolanaAddress {
        SolanaAddress::from(s)
    }

    #[test]
    fn test_transfer_v1() {
        let asset = addr("DdC5i5AFrHCvcaEUEcCktFBiytHoGbMgUQ7JBwfRGihL");
        let collection = addr("CCryptUfeFSZ3Fgc9FLeKrhLVAP67FSqi1GuVoj9CRac");
        let payer = addr("HRqBqsfppNgzcb5EfCKoFWXXz9Y5tru9ZMg1R1WtJCcw");
        let authority = addr("ArgPLJSF7ujjPZgooHGpRcnFDmftpDSzvhLRNsH94zmo");
        let new_owner = addr("EN2sCsJ1WDV8UFqsiTXHcUPUxQ4juE71eCknHYYMifkd");
        let core = *MPL_CORE_PROGRAM_ID_ADDRESS;

        let with_collection = MplCoreInstructionBuilder::transfer_v1(
            asset,
            Some(collection),
            payer,
            authority,
            new_owner,
        );
        assert_eq!(with_collection.program_id, core);
        assert_eq!(with_collection.data, vec![14, 0]);
        let expected = [
            (asset, true, false),
            (collection, false, false),
            (payer, true, true),
            (authority, false, true),
            (new_owner, false, false),
            (core, false, false),
            (core, false, false),
        ];
        let actual: Vec<_> = with_collection
            .accounts
            .iter()
            .map(|a| (a.pubkey, a.is_writable, a.is_signer))
            .collect();
        assert_eq!(actual, expected);

        // A missing collection is replaced with the Core program ID.
        let no_collection =
            MplCoreInstructionBuilder::transfer_v1(asset, None, payer, authority, new_owner);
        assert_eq!(no_collection.accounts[1].pubkey, core);
        assert!(!no_collection.accounts[1].is_writable);
    }
}
