// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_hash::blake::Blake256d;
use tw_hash::ripemd::Blake256Ripemd;
use tw_utxo::address::statically_prefixed_base58_address::StaticPrefixedB58Address;

pub const DECRED_PREFIX: u8 = 7;

pub type DecredAddress = StaticPrefixedB58Address<DECRED_PREFIX, Blake256Ripemd, Blake256d>;
