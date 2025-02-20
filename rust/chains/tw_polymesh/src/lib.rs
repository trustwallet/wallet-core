// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_proto::Polymesh::Proto;
use tw_ss58_address::NetworkId;
use tw_substrate::*;

pub mod call_encoder;
pub mod entry;
pub mod types;

pub const POLYMESH_PREFIX: u16 = 12;
pub const POLYMESH: NetworkId = NetworkId::new_unchecked(POLYMESH_PREFIX);

pub fn ctx_from_tw(input: &'_ Proto::SigningInput<'_>) -> EncodeResult<SubstrateContext> {
    let network =
        NetworkId::try_from(input.network as u16).map_err(|_| EncodeError::InvalidNetworkId)?;
    let spec_version = input.spec_version;

    Ok(SubstrateContext {
        multi_address: true,
        network,
        spec_version,
        transaction_version: input.transaction_version,
        fee_asset_id: None,
        check_metadata: false,
    })
}
