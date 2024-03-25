// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::SolanaAddress;
use crate::modules::message_builder::MessageBuilder;
use crate::modules::proto_builder::ProtoBuilder;
use crate::modules::tx_signer::TxSigner;
use crate::SOLANA_ALPHABET;
use std::borrow::Cow;
use std::collections::HashMap;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::{PublicKeyBytes, SignatureBytes};
use tw_coin_entry::error::{SigningError, SigningErrorType, SigningResult};
use tw_coin_entry::signing_output_error;
use tw_encoding::{base58, base64};
use tw_keypair::ed25519;
use tw_keypair::traits::VerifyingKeyTrait;
use tw_proto::Solana::Proto;

pub struct SolanaCompiler;

impl SolanaCompiler {
    #[inline]
    pub fn preimage_hashes(
        coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
    ) -> Proto::PreSigningOutput<'static> {
        Self::preimage_hashes_impl(coin, input)
            .unwrap_or_else(|e| signing_output_error!(Proto::PreSigningOutput, e))
    }

    fn preimage_hashes_impl(
        _coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
    ) -> SigningResult<Proto::PreSigningOutput<'static>> {
        let builder = MessageBuilder::new(input);
        let unsigned_msg = builder.build()?;
        let data_to_sign = TxSigner::preimage_versioned(&unsigned_msg)?;

        let signers: Vec<_> = unsigned_msg
            .signers()
            .map(|addr| Cow::from(addr.to_string().into_bytes()))
            .collect();

        Ok(Proto::PreSigningOutput {
            signers,
            data: Cow::from(data_to_sign),
            ..Proto::PreSigningOutput::default()
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
        _coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
        signatures: Vec<SignatureBytes>,
        public_keys: Vec<PublicKeyBytes>,
    ) -> SigningResult<Proto::SigningOutput<'static>> {
        let encode = move |data| match input.tx_encoding {
            Proto::Encoding::Base58 => base58::encode(data, &SOLANA_ALPHABET),
            Proto::Encoding::Base64 => base64::encode(data, false),
        };

        if signatures.len() != public_keys.len() {
            return Err(SigningError(SigningErrorType::Error_signatures_count));
        }

        let builder = MessageBuilder::new(input);
        let unsigned_msg = builder.build()?;
        let data_to_sign = TxSigner::preimage_versioned(&unsigned_msg)?;

        // Verify the given signatures and collect the key-signature map.
        let mut key_signs = HashMap::default();
        for (sign, pubkey) in signatures.iter().zip(public_keys.iter()) {
            let signature = ed25519::Signature::try_from(sign.as_slice())?;
            let pubkey = ed25519::sha512::PublicKey::try_from(pubkey.as_slice())?;

            if !pubkey.verify(signature.clone(), data_to_sign.clone())
                && !signature.to_bytes().is_zero()
            {
                return Err(SigningError(SigningErrorType::Error_signing));
            }

            key_signs.insert(SolanaAddress::with_public_key_ed25519(&pubkey), signature);
        }

        let signed_tx = TxSigner::compile_versioned(unsigned_msg, key_signs)?;

        let signed_encoded = bincode::serialize(&signed_tx)
            .map_err(|_| SigningError(SigningErrorType::Error_internal))?;
        let signed_encoded = encode(&signed_encoded);
        let unsigned_encoded = encode(&data_to_sign);

        Ok(Proto::SigningOutput {
            encoded: Cow::from(signed_encoded),
            unsigned_tx: Cow::from(unsigned_encoded),
            signatures: ProtoBuilder::build_signatures(&signed_tx),
            ..Proto::SigningOutput::default()
        })
    }
}
