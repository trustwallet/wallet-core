// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::SolanaAddress;
use crate::blockhash::Blockhash;
use crate::defined_addresses::*;
use tw_coin_entry::error::{AddressError, AddressResult};
use tw_hash::sha2::sha256;
use tw_hash::H256;

pub struct StakeProgram;

impl StakeProgram {
    pub fn address_from_recent_blockhash(
        from: SolanaAddress,
        recent_blockhash: Blockhash,
    ) -> SolanaAddress {
        let mut seed = from.bytes().to_vec();
        seed.extend_from_slice(recent_blockhash.to_string().as_bytes());
        seed.extend_from_slice(STAKE_PROGRAM_ID_ADDRESS.bytes().as_slice());
        let bytes =
            H256::try_from(sha256(&seed).as_slice()).expect("sha256 expected to return 32 bytes");
        SolanaAddress::with_public_key_bytes(bytes)
    }

    /// https://github.com/solana-labs/solana-program-library/blob/master/associated-token-account/program/src/lib.rs#L35
    pub fn get_associated_token_address(
        main_address: SolanaAddress,
        token_mint_address: SolanaAddress,
    ) -> AddressResult<SolanaAddress> {
        SolanaAddress::find_program_address(
            &[
                main_address.bytes().as_slice(),
                TOKEN_PROGRAM_ID_ADDRESS.bytes().as_slice(),
                token_mint_address.bytes().as_slice(),
            ],
            *ASSOCIATED_TOKEN_PROGRAM_ID_ADDRESS,
        )
        .ok_or(AddressError::InvalidInput)
    }
}
