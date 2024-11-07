// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::ctx_from_tw;
use crate::tx_builder::TxBuilder;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::{PublicKeyBytes, SignatureBytes};
use tw_coin_entry::common::compile_input::SingleSignaturePubkey;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::signing_output_error;
use tw_keypair::ed25519::{sha512::PublicKey, Signature};
use tw_proto::Polkadot::Proto;
use tw_proto::TxCompiler::Proto as CompilerProto;
use tw_scale::ToScale;
use tw_ss58_address::SS58Address;
use tw_substrate::*;

pub struct PolkadotCompiler;

impl PolkadotCompiler {
    #[inline]
    pub fn preimage_hashes(
        coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
    ) -> CompilerProto::PreSigningOutput<'static> {
        Self::preimage_hashes_impl(coin, input)
            .unwrap_or_else(|e| signing_output_error!(CompilerProto::PreSigningOutput, e))
    }

    fn preimage_hashes_impl(
        coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
    ) -> SigningResult<CompilerProto::PreSigningOutput<'static>> {
        let ctx = ctx_from_tw(&input)?;
        let unsigned_tx = TxBuilder::unsigned_tx_from_proto(&ctx, coin, &input)?;
        let pre_image = unsigned_tx.encode_payload()?;

        Ok(CompilerProto::PreSigningOutput {
            // `pre_image` is already hashed if it is larger then 256 bytes.
            data_hash: pre_image.clone().into(),
            data: pre_image.into(),
            ..Default::default()
        })
    }

    #[inline]
    pub fn compile(
        coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
        signatures: Vec<SignatureBytes>,
        public_keys: Vec<PublicKeyBytes>,
    ) -> Proto::SigningOutput<'static> {
        Self::compile_impl(coin, input, signatures, public_keys)
            .unwrap_or_else(|e| signing_output_error!(Proto::SigningOutput, e))
    }

    fn compile_impl(
        coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
        signatures: Vec<SignatureBytes>,
        public_keys: Vec<PublicKeyBytes>,
    ) -> SigningResult<Proto::SigningOutput<'static>> {
        let SingleSignaturePubkey {
            signature,
            public_key,
        } = SingleSignaturePubkey::from_sign_pubkey_list(signatures, public_keys)?;
        let signature = Signature::try_from(signature.as_slice())?;
        let public_key = PublicKey::try_from(public_key.as_slice())?;
        let ctx = ctx_from_tw(&input)?;
        let account = ctx.multi_address(SubstrateAddress(SS58Address::from_public_key(
            &public_key,
            ctx.network,
        )?));

        let unsigned_tx = TxBuilder::unsigned_tx_from_proto(&ctx, coin, &input)?;
        let signed_tx = unsigned_tx.into_signed(account, signature)?;
        let encoded = signed_tx.to_scale();

        Ok(Proto::SigningOutput {
            encoded: encoded.into(),
            ..Default::default()
        })
    }
}
