// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::prefixed_b58_address::PrefixedB58Address;
use tw_hash::ripemd::Sha256Ripemd;
use tw_hash::sha2::Sha256d;

pub type LegacyAddress = PrefixedB58Address<Sha256Ripemd, Sha256d>;
