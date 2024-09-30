// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::evm_context::EvmContext;
use crate::modules::tx_builder::TxBuilder;
use std::borrow::Cow;
use std::marker::PhantomData;
use tw_coin_entry::coin_entry::{PublicKeyBytes, SignatureBytes};
use tw_coin_entry::common::compile_input::SingleSignaturePubkey;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::signing_output_error;
use tw_keypair::ecdsa::secp256k1;
use tw_number::U256;
use tw_proto::Ethereum::Proto;
use tw_proto::TxCompiler::Proto as CompilerProto;

pub struct Compiler<Context: EvmContext> {
    _phantom: PhantomData<Context>,
}

impl<Context: EvmContext> Compiler<Context> {
    #[inline]
    pub fn preimage_hashes(
        input: Proto::SigningInput<'_>,
    ) -> CompilerProto::PreSigningOutput<'static> {
        Self::preimage_hashes_impl(input)
            .unwrap_or_else(|e| signing_output_error!(CompilerProto::PreSigningOutput, e))
    }

    #[inline]
    pub fn compile(
        input: Proto::SigningInput<'_>,
        signatures: Vec<SignatureBytes>,
        public_keys: Vec<PublicKeyBytes>,
    ) -> Proto::SigningOutput<'static> {
        Self::compile_impl(input, signatures, public_keys)
            .unwrap_or_else(|e| signing_output_error!(Proto::SigningOutput, e))
    }

    fn preimage_hashes_impl(
        input: Proto::SigningInput<'_>,
    ) -> SigningResult<CompilerProto::PreSigningOutput<'static>> {
        let chain_id = U256::from_big_endian_slice(&input.chain_id)
            .into_tw()
            .context("Invalid chain ID")?;

        let unsigned = TxBuilder::<Context>::tx_from_proto(&input)?;
        let prehash = unsigned.pre_hash(chain_id);
        let preimage_data = unsigned.encode(chain_id);

        Ok(CompilerProto::PreSigningOutput {
            data: Cow::from(preimage_data),
            data_hash: Cow::from(prehash.to_vec()),
            ..CompilerProto::PreSigningOutput::default()
        })
    }

    fn compile_impl(
        input: Proto::SigningInput<'_>,
        signatures: Vec<SignatureBytes>,
        _: Vec<PublicKeyBytes>,
    ) -> SigningResult<Proto::SigningOutput<'static>> {
        let SingleSignaturePubkey {
            signature,
            public_key: _,
        } = SingleSignaturePubkey::from_sign_list(signatures)?;
        let signature = secp256k1::Signature::from_bytes(&signature)?;

        let chain_id = U256::from_big_endian_slice(&input.chain_id)
            .into_tw()
            .context("Invalid chain ID")?;

        let unsigned = TxBuilder::<Context>::tx_from_proto(&input)?;

        let pre_hash = unsigned.pre_hash(chain_id);

        let signed = unsigned.try_into_signed(signature, chain_id)?;

        let eth_signature = signed.signature();

        Ok(Proto::SigningOutput {
            encoded: signed.encode().into(),
            v: eth_signature.v().to_big_endian_compact().into(),
            r: eth_signature.r().to_big_endian_compact().into(),
            s: eth_signature.s().to_big_endian_compact().into(),
            data: signed.payload().into(),
            pre_hash: pre_hash.to_vec().into(),
            ..Proto::SigningOutput::default()
        })
    }
}
