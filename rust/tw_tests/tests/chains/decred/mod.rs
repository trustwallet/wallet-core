// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::common::bitcoin::ChainSpecific;
use tw_proto::BitcoinV2::Proto;
use tw_proto::DecredV2::Proto as DecredProto;

mod decred_address;
mod decred_compile;
mod decred_sign;

pub const DECRED_P2PKH_PREFIX: u8 = 63;
pub const DECRED_P2PSH_PREFIX: u8 = 26;

pub fn decred_info() -> Option<Proto::ChainInfo<'static>> {
    Some(Proto::ChainInfo {
        p2pkh_prefix: DECRED_P2PKH_PREFIX as u32,
        p2sh_prefix: DECRED_P2PSH_PREFIX as u32,
        ..Proto::ChainInfo::default()
    })
}

pub fn decred_extra_data(expiry_height: u32) -> ChainSpecific<'static> {
    ChainSpecific::decred_extra_data(DecredProto::TransactionBuilderExtraData { expiry_height })
}
