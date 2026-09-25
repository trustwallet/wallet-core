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

pub const POLKADOT: NetworkId = NetworkId::new_unchecked(0);
pub const KUSAMA: NetworkId = NetworkId::new_unchecked(2);
pub const ACALA: NetworkId = NetworkId::new_unchecked(10);
pub const POLYMESH: NetworkId = NetworkId::new_unchecked(12);
pub const GENERIC_SUBSTRATE: NetworkId = NetworkId::new_unchecked(42);

pub const POLKADOT_MULTI_ADDRESS_SPEC: u32 = 28;
pub const KUSAMA_MULTI_ADDRESS_SPEC: u32 = 2028;

pub fn network_id_from_tw(input: &'_ Proto::SigningInput<'_>) -> EncodeResult<NetworkId> {
    Ok(NetworkId::try_from(input.network as u16).map_err(|_| EncodeError::InvalidNetworkId)?)
}

pub fn fee_asset_id_from_tw(input: &'_ Proto::SigningInput<'_>) -> Option<FeeAssetId> {
    // Special case for batches.
    if let SigningVariant::balance_call(b) = &input.message_oneof {
        match &b.message_oneof {
            BalanceVariant::asset_transfer(at) => return Some(FeeAssetId::Asset(at.fee_asset_id)),
            BalanceVariant::batch_asset_transfer(bat) => {
                return Some(FeeAssetId::Asset(bat.fee_asset_id))
            },
            _ => {},
        }
    }

    if input.charge_native_as_asset_tx_payment {
        Some(FeeAssetId::Native)
    } else {
        None
    }
}

pub fn ctx_from_tw(input: &'_ Proto::SigningInput<'_>) -> EncodeResult<SubstrateContext> {
    let network =
        NetworkId::try_from(input.network as u16).map_err(|_| EncodeError::InvalidNetworkId)?;
    let spec_version = input.spec_version;
    let multi_address = match (input.multi_address, network) {
        (true, _) => true,
        (_, POLKADOT) if spec_version >= POLKADOT_MULTI_ADDRESS_SPEC => true,
        (_, KUSAMA) if spec_version >= KUSAMA_MULTI_ADDRESS_SPEC => true,
        _ => false,
    };
    let check_metadata = match network {
        POLKADOT | KUSAMA if spec_version >= 1_002_005 => true,
        ACALA if spec_version >= 2_270 => true,
        _ => false,
    };
    let fee_asset_id = fee_asset_id_from_tw(input);

    Ok(SubstrateContext {
        multi_address,
        check_metadata,
        network,
        spec_version,
        transaction_version: input.transaction_version,
        fee_asset_id,
    })
}
