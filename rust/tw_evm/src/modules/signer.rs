// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::evm_context::EvmContext;
use crate::modules::tx_builder::TxBuilder;
use std::borrow::Cow;
use std::marker::PhantomData;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::signing_output_error;
use tw_keypair::ecdsa::secp256k1;
use tw_keypair::traits::SigningKeyTrait;
use tw_number::U256;
use tw_proto::Ethereum::Proto;

const SIGNATURE_V_MIN_LEN: usize = 1;

pub struct Signer<Context: EvmContext> {
    _phantom: PhantomData<Context>,
}

impl<Context: EvmContext> Signer<Context> {
    #[inline]
    pub fn sign_proto(input: Proto::SigningInput<'_>) -> Proto::SigningOutput<'static> {
        Self::sign_proto_impl(input)
            .unwrap_or_else(|e| signing_output_error!(Proto::SigningOutput, e))
    }

    fn sign_proto_impl(
        input: Proto::SigningInput<'_>,
    ) -> SigningResult<Proto::SigningOutput<'static>> {
        let chain_id = U256::from_big_endian_slice(&input.chain_id)
            .into_tw()
            .context("Invalid chain ID")?;
        let private_key = secp256k1::PrivateKey::try_from(input.private_key.as_ref())?;

        let unsigned = TxBuilder::<Context>::tx_from_proto(&input)?;

        let pre_hash = unsigned.pre_hash(chain_id);
        let signature = private_key.sign(pre_hash)?;

        let signed = unsigned.try_into_signed(signature, chain_id)?;

        let eth_signature = signed.signature();

        let v = eth_signature
            .v()
            .to_big_endian_compact_min_len(SIGNATURE_V_MIN_LEN);

        Ok(Proto::SigningOutput {
            encoded: Cow::from(signed.encode()),
            v: Cow::from(v),
            r: Cow::from(eth_signature.r().to_big_endian().to_vec()),
            s: Cow::from(eth_signature.s().to_big_endian().to_vec()),
            data: Cow::from(signed.payload()),
            pre_hash: Cow::from(pre_hash.to_vec()),
            ..Proto::SigningOutput::default()
        })
    }
}
