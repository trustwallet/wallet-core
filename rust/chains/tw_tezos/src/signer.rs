// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::signing_output_error;
use tw_hash::blake2::blake2_b;
use tw_keypair::tw::{Curve, PrivateKey};
use tw_proto::Tezos::Proto;

use crate::{forging::ForgeError, operation_list::OperationList};

pub struct TezosSigner;

impl TezosSigner {
    pub fn sign(
        coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
    ) -> Proto::SigningOutput<'static> {
        Self::sign_impl(coin, input)
            .unwrap_or_else(|e| signing_output_error!(Proto::SigningOutput, e))
    }

    fn sign_impl(
        _coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
    ) -> SigningResult<Proto::SigningOutput<'static>> {
        let key = PrivateKey::new(input.private_key.to_vec())?;
        let encoded = if input.encoded_operations.is_empty() {
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
            Self::sign_operation_list(&key, &operation_list)
        } else {
            Self::sign_data(&key, &input.encoded_operations)
        }?;

        let output = Proto::SigningOutput {
            encoded: encoded.into(),
            ..Proto::SigningOutput::default()
        };
        Ok(output)
    }

    pub fn sign_operation_list(
        private_key: &PrivateKey,
        operation_list: &OperationList,
    ) -> SigningResult<Vec<u8>> {
        let forged = operation_list
            .forge(Some(private_key))
            .map_err(|_| SigningErrorType::Error_internal)?;
        Self::sign_data(private_key, &forged)
    }

    pub fn sign_data(private_key: &PrivateKey, data: &[u8]) -> SigningResult<Vec<u8>> {
        let mut watermarked_data = Vec::new();
        watermarked_data.push(0x03);
        watermarked_data.extend_from_slice(data);

        let hash = blake2_b(&watermarked_data, 32).map_err(|_| SigningErrorType::Error_internal)?;
        let signature = private_key.sign(&hash, Curve::Ed25519)?;

        let mut signed_data = Vec::new();
        signed_data.extend_from_slice(data);
        signed_data.extend_from_slice(&signature);
        Ok(signed_data)
    }

    pub fn build_unsigned_tx(operation_list: &OperationList) -> Result<Vec<u8>, ForgeError> {
        operation_list.forge(None)
    }

    pub fn build_signed_tx(
        operation_list: &OperationList,
        signature: Vec<u8>,
    ) -> Result<Vec<u8>, ForgeError> {
        let mut signed_data = Vec::new();

        let tx_data = operation_list.forge(None)?;

        signed_data.extend_from_slice(&tx_data);
        signed_data.extend_from_slice(&signature);

        Ok(signed_data)
    }
}
