// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_proto::Polkadot::Proto;
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

    Ok(SubstrateContext {
        multi_address,
        network,
        spec_version,
        transaction_version: input.transaction_version,
    })
}
