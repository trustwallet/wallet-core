// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::compiler::GreenfieldCompiler;
use crate::modules::eip712_signer::Eip712Signer;
use crate::modules::tx_builder::TxBuilder;
use std::borrow::Cow;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::SigningResult;
use tw_coin_entry::signing_output_error;
use tw_keypair::ecdsa::secp256k1;
use tw_keypair::traits::KeyPairTrait;
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
        let public_key = key_pair.public();

        // Set the public key. It will be used to construct a signer info.
        input.public_key = Cow::from(public_key.compressed().to_vec());
        let unsigned_tx = TxBuilder::unsigned_tx_from_proto(coin, &input)?;

        let signed = Eip712Signer::sign(&key_pair, unsigned_tx)?;
        let signatures = vec![signed.signature];
        let public_keys = vec![public_key.compressed().to_vec()];

        GreenfieldCompiler::compile_impl(coin, input, signatures, public_keys)
    }
}
