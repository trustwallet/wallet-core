// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

pub mod derivation;
pub mod legacy;
pub mod prefixed_b58_address;
pub mod segwit;
pub mod standard_bitcoin;
pub mod statically_prefixed_base58_address;
pub mod taproot;
pub mod witness_program;

type Bech32Prefix = tw_bech32_address::bech32_prefix::Bech32Prefix;
