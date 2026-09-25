// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_proto::BitcoinV2::Proto;

mod komodo_address;
mod komodo_compile;
mod komodo_sign;

pub fn komodo_info() -> Option<Proto::ChainInfo<'static>> {
    Some(Proto::ChainInfo {
        p2pkh_prefix: 60,
        p2sh_prefix: 85,
        ..Proto::ChainInfo::default()
    })
}
