// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_hash::groestl::Groestl512d;
use tw_hash::ripemd::Sha256Ripemd;
use tw_utxo::address::legacy::PrefixedBase58Address;

pub type GroestlLegacyAddress = PrefixedBase58Address<Sha256Ripemd, Groestl512d>;
