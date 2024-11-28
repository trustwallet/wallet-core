// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_proto::Polkadot::Proto::{
    self, mod_Balance::OneOfmessage_oneof as BalanceVariant,
    mod_SigningInput::OneOfmessage_oneof as SigningVariant,
};
use tw_ss58_address::NetworkId;
use tw_substrate::*;

pub mod call_encoder;
pub mod entry;

pub const POLYMESH_PREFIX: u16 = 12;
pub const POLYMESH: NetworkId = NetworkId::new_unchecked(POLYMESH_PREFIX);

pub fn network_id_from_tw(input: &'_ Proto::SigningInput<'_>) -> EncodeResult<NetworkId> {
    Ok(NetworkId::try_from(input.network as u16).map_err(|_| EncodeError::InvalidNetworkId)?)
}

pub fn fee_asset_id_from_tw(input: &'_ Proto::SigningInput<'_>) -> Option<u32> {
    // Special case for batches.
    match &input.message_oneof {
        SigningVariant::balance_call(b) => match &b.message_oneof {
            BalanceVariant::asset_transfer(at) => Some(at.fee_asset_id),
            BalanceVariant::batch_asset_transfer(bat) => Some(bat.fee_asset_id),
            _ => None,
        },
        _ => None,
    }
}

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
    })
}
