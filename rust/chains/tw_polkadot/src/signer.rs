// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::tx_builder::TxBuilder;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::signing_output_error;
use tw_keypair::{ed25519::sha512::KeyPair, traits::KeyPairTrait};
use tw_proto::Polkadot::Proto;
use tw_scale::ToScale;
use tw_ss58_address::{NetworkId, SS58Address};
use tw_substrate::*;

pub fn network_id_from_tw(input: &'_ Proto::SigningInput<'_>) -> EncodeResult<NetworkId> {
    Ok(NetworkId::try_from(input.network as u16).map_err(|_| EncodeError::InvalidNetworkId)?)
}

pub struct PolkadotSigner;

impl PolkadotSigner {
    pub fn sign(
        coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
    ) -> Proto::SigningOutput<'static> {
        Self::sign_impl(coin, input)
            .unwrap_or_else(|e| signing_output_error!(Proto::SigningOutput, e))
    }

    fn sign_impl(
        coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
    ) -> SigningResult<Proto::SigningOutput<'static>> {
        let key_pair = KeyPair::try_from(input.private_key.as_ref())?;
        let network_id = network_id_from_tw(&input)?;
        let account =
            SubstrateAddress(SS58Address::from_public_key(key_pair.public(), network_id)?);
        let unsigned_tx = TxBuilder::unsigned_tx_from_proto(coin, &input)?;
        let signed_tx = unsigned_tx.sign(account, &key_pair)?;
        let encoded = signed_tx.to_scale();

        Ok(Proto::SigningOutput {
            encoded: encoded.into(),
            ..Default::default()
        })
    }
}
