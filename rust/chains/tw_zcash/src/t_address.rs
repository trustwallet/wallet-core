// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_hash::ripemd::Sha256Ripemd;
use tw_hash::sha2::Sha256d;
use tw_utxo::address::statically_prefixed_base58_address::StaticPrefixedB58Address;

/// For now, T-prefix is a constant value.
pub const ZCASH_T_PREFIX: u8 = 0x1C;

pub type TAddress = StaticPrefixedB58Address<ZCASH_T_PREFIX, Sha256Ripemd, Sha256d>;
