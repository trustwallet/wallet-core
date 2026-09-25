// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_proto::BitcoinV2::Proto;

mod groestlcoin_address;
mod groestlcoin_compile;
mod groestlcoin_sign;

pub fn groestl_info() -> Option<Proto::ChainInfo<'static>> {
    Some(Proto::ChainInfo {
        p2pkh_prefix: 36,
        p2sh_prefix: 5,
        ..Proto::ChainInfo::default()
    })
}
