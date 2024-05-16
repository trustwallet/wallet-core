// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

pub mod derivation;
pub mod legacy;
pub mod segwit;
pub mod standard_bitcoin;
pub mod taproot;
pub mod witness_program;

type Bech32Prefix = tw_bech32_address::bech32_prefix::Bech32Prefix;
