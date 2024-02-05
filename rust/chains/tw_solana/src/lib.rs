// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_encoding::base58::Alphabet;

pub mod address;
pub mod compiler;
pub mod entry;
pub mod modules;
pub mod signer;
pub mod transaction;

// cbindgen:ignore
pub const SOLANA_ALPHABET: Alphabet = *Alphabet::BITCOIN;
