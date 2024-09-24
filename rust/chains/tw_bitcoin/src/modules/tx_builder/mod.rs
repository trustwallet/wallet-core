// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

pub mod output_protobuf;
pub mod public_keys;
pub mod script_parser;
pub mod utxo_protobuf;

pub struct BitcoinChainInfo {
    pub p2pkh_prefix: u8,
    pub p2sh_prefix: u8,
    /// Note that not all Bitcoin forks support HRP (segwit addresses).
    pub hrp: Option<String>,
}
