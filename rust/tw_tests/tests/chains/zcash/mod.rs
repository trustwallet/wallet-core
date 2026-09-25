// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::common::bitcoin::ChainSpecific;
use tw_proto::BitcoinV2::Proto;
use tw_proto::Zcash::Proto as ZcashProto;

mod zcash_address;
mod zcash_compile;
mod zcash_plan;
mod zcash_sign;

/// See https://github.com/zcash/zips/blob/master/zips/zip-0205.rst#sapling-deployment BRANCH_ID section
pub const SAPLING_BRANCH_ID: &[u8] = &[0xbb, 0x09, 0xb8, 0x76];
/// See https://github.com/zcash/zips/blob/master/zips/zip-0206.rst#blossom-deployment BRANCH_ID section
pub const BLOSSOM_BRANCH_ID: &[u8] = &[0x60, 0x0e, 0xb4, 0x2b];
/// See https://github.com/zcash/zips/blob/main/zips/zip-0253.md#nu6-deployment CONSENSUS_BRANCH_ID section
pub const NU6_BRANCH_ID: &[u8] = &[0x55, 0x10, 0xe7, 0xc8];
pub const ZCASH_P2PKH_PREFIX: u8 = 184;
pub const ZCASH_P2SH_PREFIX: u8 = 189;

pub fn zec_info() -> Option<Proto::ChainInfo<'static>> {
    Some(Proto::ChainInfo {
        p2pkh_prefix: ZCASH_P2PKH_PREFIX as u32,
        p2sh_prefix: ZCASH_P2SH_PREFIX as u32,
        ..Proto::ChainInfo::default()
    })
}

pub fn zcash_extra_data(data: ZcashProto::TransactionBuilderExtraData) -> ChainSpecific {
    ChainSpecific::zcash_extra_data(data)
}
