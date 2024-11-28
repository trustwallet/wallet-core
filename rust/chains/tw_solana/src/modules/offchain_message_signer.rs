// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::SOLANA_ALPHABET;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::modules::message_signer::MessageSigner;
use tw_coin_entry::signing_output_error;
use tw_encoding::base58;
use tw_keypair::ed25519;
use tw_keypair::traits::{SigningKeyTrait, VerifyingKeyTrait};
use tw_misc::try_or_false;
use tw_proto::Solana::Proto;
use tw_proto::TxCompiler::Proto as CompilerProto;

/// Currently, supports https://solana.com/developers/cookbook/wallets/sign-message only.
pub struct OffchainMessageSigner;

impl OffchainMessageSigner {
    pub fn sign_message_impl(
        _coin: &dyn CoinContext,
        input: Proto::MessageSigningInput,
    ) -> SigningResult<Proto::MessageSigningOutput<'static>> {
        let private_key = ed25519::sha512::PrivateKey::try_from(input.private_key.as_ref())?;
        let sign = private_key.sign(input.message.as_bytes().to_vec())?;
        let base58_sign = base58::encode(sign.to_bytes().as_slice(), SOLANA_ALPHABET);
        Ok(Proto::MessageSigningOutput {
            signature: base58_sign.into(),
            ..Proto::MessageSigningOutput::default()
        })
    }
}

impl MessageSigner for OffchainMessageSigner {
    type MessageSigningInput<'a> = Proto::MessageSigningInput<'a>;
    type MessagePreSigningOutput = CompilerProto::PreSigningOutput<'static>;
    type MessageSigningOutput = Proto::MessageSigningOutput<'static>;
    type MessageVerifyingInput<'a> = Proto::MessageVerifyingInput<'a>;

    fn message_preimage_hashes(
        &self,
        _coin: &dyn CoinContext,
        input: Self::MessageSigningInput<'_>,
    ) -> Self::MessagePreSigningOutput {
        CompilerProto::PreSigningOutput {
            data: input.message.as_bytes().to_vec().into(),
            ..CompilerProto::PreSigningOutput::default()
        }
    }

    fn sign_message(
        &self,
        coin: &dyn CoinContext,
        input: Self::MessageSigningInput<'_>,
    ) -> Self::MessageSigningOutput {
        Self::sign_message_impl(coin, input)
            .unwrap_or_else(|e| signing_output_error!(Proto::MessageSigningOutput, e))
    }

    fn verify_message(
        &self,
        _coin: &dyn CoinContext,
        input: Self::MessageVerifyingInput<'_>,
    ) -> bool {
        let sign = try_or_false!(base58::decode(&input.signature, SOLANA_ALPHABET));
        let sign = try_or_false!(ed25519::Signature::try_from(sign.as_slice()));
        let public_key = try_or_false!(ed25519::sha512::PublicKey::try_from(
            input.public_key.as_ref()
        ));
        let message_utf8 = input.message.as_bytes().to_vec();
        public_key.verify(sign, message_utf8)
    }
}
