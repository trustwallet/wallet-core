// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.
use std::ops::RangeInclusive;
use tw_hash::{H160, H256};

pub mod derivation;
pub mod legacy;
pub mod segwit;
pub mod standard_bitcoin;
pub mod taproot;

const MAX_WITNESS_VERSION: u8 = 16;
const WITNESS_VERSIONS: RangeInclusive<u8> = WITNESS_V0..=MAX_WITNESS_VERSION;
const WITNESS_V0: u8 = 0;
const WITNESS_V1: u8 = 1;
/// Witness program sizes valid for V0 (Segwit).
const WITNESS_V0_VALID_PROGRAM_SIZES: [usize; 2] = [H160::LEN, H256::LEN];
/// Witness program sizes valid for V1 (Taproot).
const WITNESS_V1_VALID_PROGRAM_SIZES: [usize; 1] = [H256::LEN];
/// Witness program sizes valid for most of the witness versions.
/// Please note that V0 is more constraint.
const WITNESS_VALID_PROGRAM_SIZES: RangeInclusive<usize> = 2..=40;

type Bech32Prefix = tw_bech32_address::bech32_prefix::Bech32Prefix;
