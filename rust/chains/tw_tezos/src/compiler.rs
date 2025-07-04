// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::{PublicKeyBytes, SignatureBytes};
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::signing_output_error;
use tw_hash::blake2::blake2_b;
use tw_proto::Tezos::Proto;
use tw_proto::TxCompiler::Proto as CompilerProto;

use crate::operation_list::OperationList;
use crate::signer::TezosSigner;

pub struct TezosCompiler;

impl TezosCompiler {
    #[inline]
    pub fn preimage_hashes(
        coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
    ) -> CompilerProto::PreSigningOutput<'static> {
        Self::preimage_hashes_impl(coin, input)
            .unwrap_or_else(|e| signing_output_error!(CompilerProto::PreSigningOutput, e))
    }

    fn preimage_hashes_impl(
        _coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
    ) -> SigningResult<CompilerProto::PreSigningOutput<'static>> {
        let mut operation_list = OperationList::new(
            input
                .operation_list
                .clone()
                .ok_or(SigningErrorType::Error_invalid_params)?
                .branch
                .into(),
        );
        for operation in input
            .operation_list
            .clone()
            .ok_or(SigningErrorType::Error_invalid_params)?
            .operations
        {
            operation_list.add_operation(operation);
        }

        let pre_image = TezosSigner::build_unsigned_tx(&operation_list)
            .map_err(|_| SigningErrorType::Error_internal)?;

        let mut watermarked_data = Vec::new();
        watermarked_data.push(0x03);
        watermarked_data.extend_from_slice(&pre_image);
        let pre_image_hash =
            blake2_b(&watermarked_data, 32).map_err(|_| SigningErrorType::Error_internal)?;

        let output = CompilerProto::PreSigningOutput {
            data_hash: pre_image_hash.into(),
            data: pre_image.into(),
            ..CompilerProto::PreSigningOutput::default()
        };
        Ok(output)
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
        _coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
        signatures: Vec<SignatureBytes>,
        _public_keys: Vec<PublicKeyBytes>,
    ) -> SigningResult<Proto::SigningOutput<'static>> {
        if signatures.len() != 1 {
            return TWError::err(SigningErrorType::Error_signatures_count)
                .context("Expected exactly one signature");
        }

        let signature = signatures
            .into_iter()
            .next()
            .or_tw_err(SigningErrorType::Error_signatures_count)
            .context("Expected exactly one signature")?;

        let mut operation_list = OperationList::new(
            input
                .operation_list
                .clone()
                .ok_or(SigningErrorType::Error_invalid_params)?
                .branch
                .into(),
        );
        for operation in input
            .operation_list
            .clone()
            .ok_or(SigningErrorType::Error_invalid_params)?
            .operations
        {
            operation_list.add_operation(operation);
        }
        let tx = TezosSigner::build_signed_tx(&operation_list, signature)
            .map_err(|_| SigningErrorType::Error_internal)?;

        Ok(Proto::SigningOutput {
            encoded: tx.into(),
            ..Proto::SigningOutput::default()
        })
    }
}
