// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::SolanaAddress;
use crate::blockhash::Blockhash;
use tw_hash::sha2::sha256;
use tw_hash::H256;
use crate::defined_addresses::STAKE_PROGRAM_ID_ADDRESS;

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
}
