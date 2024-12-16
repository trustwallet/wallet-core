// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::borrow::Cow;
use tw_coin_entry::error::prelude::*;
use tw_keypair::schnorr;
use tw_utxo::context::AddressPrefixes;

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

impl BitcoinChainInfo {
    pub fn to_address_prefixes(&self) -> AddressPrefixes {
        AddressPrefixes {
            p2pkh_prefix: self.p2pkh_prefix,
            p2sh_prefix: self.p2sh_prefix,
        }
    }
}

fn parse_schnorr_pk(bytes: &Cow<[u8]>) -> SigningResult<schnorr::XOnlyPublicKey> {
    schnorr::XOnlyPublicKey::try_from(bytes.as_ref()).into_tw()
}

fn parse_schnorr_pks(pks: &[Cow<[u8]>]) -> SigningResult<Vec<schnorr::XOnlyPublicKey>> {
    pks.iter().map(parse_schnorr_pk).collect()
}
