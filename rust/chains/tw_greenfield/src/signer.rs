// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::compiler::GreenfieldCompiler;
use crate::modules::eip712_signer::{Eip712Signer, Eip712TxPreimage};
use crate::modules::tx_builder::TxBuilder;
use std::borrow::Cow;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::SigningResult;
use tw_coin_entry::signing_output_error;
use tw_keypair::ecdsa::secp256k1;
use tw_keypair::traits::{KeyPairTrait, SigningKeyTrait};
use tw_misc::traits::ToBytesVec;
use tw_proto::Greenfield::Proto;

pub struct GreenfieldSigner;

impl GreenfieldSigner {
    pub fn sign(
        coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
    ) -> Proto::SigningOutput<'static> {
        Self::sign_impl(coin, input)
            .unwrap_or_else(|e| signing_output_error!(Proto::SigningOutput, e))
    }

    fn sign_impl(
        coin: &dyn CoinContext,
        mut input: Proto::SigningInput<'_>,
    ) -> SigningResult<Proto::SigningOutput<'static>> {
        let key_pair = secp256k1::KeyPair::try_from(input.private_key.as_ref())?;
        let public_key = key_pair.public().compressed().to_vec();

        // Set the public key. It will be used to construct a signer info.
        input.public_key = Cow::from(public_key.clone());
        let unsigned_tx = TxBuilder::unsigned_tx_from_proto(coin, &input)?;

        let Eip712TxPreimage { tx_hash, .. } = Eip712Signer::preimage_hash(&unsigned_tx)?;
        // Get the standard secp256k1 signature. It will be EIP155 protected at the `GreenfieldCompiler::compile_impl`.
        let signature = key_pair.sign(tx_hash)?;

        let signatures = vec![signature.to_vec()];
        let public_keys = vec![public_key];

        GreenfieldCompiler::compile_impl(coin, input, signatures, public_keys)
    }
}
