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
pub mod compiler;
pub mod entry;
pub mod signer;
pub mod tx_builder;

pub const POLKADOT_MULTI_ADDRESS_SPEC: u32 = 28;
pub const KUSAMA_MULTI_ADDRESS_SPEC: u32 = 2028;

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
    let multi_address = match (input.multi_address, network) {
        (true, _) => true,
        (_, NetworkId::POLKADOT) if spec_version >= POLKADOT_MULTI_ADDRESS_SPEC => true,
        (_, NetworkId::KUSAMA) if spec_version >= KUSAMA_MULTI_ADDRESS_SPEC => true,
        _ => false,
    };
    let fee_asset_id = fee_asset_id_from_tw(input);

    Ok(SubstrateContext {
        multi_address,
        network,
        spec_version,
        transaction_version: input.transaction_version,
        fee_asset_id,
    })
}
