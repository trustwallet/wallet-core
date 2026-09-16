// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::SolanaAddress;
use crate::modules::offchain_message::OffchainMessage;
use crate::modules::structured_message::StructuredMessageRenderer;
use crate::SOLANA_ALPHABET;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::modules::message_signer::MessageSigner;
use tw_coin_entry::signing_output_error;
use tw_encoding::base58;
use tw_keypair::ed25519;
use tw_keypair::traits::{SigningKeyTrait, VerifyingKeyTrait};
use tw_memory::Data;
use tw_misc::try_or_false;
use tw_proto::Solana::Proto;
use tw_proto::TxCompiler::Proto as CompilerProto;

use Proto::mod_MessageSigningInput::OneOfmessage_payload as SigningPayload;
use Proto::mod_MessageVerifyingInput::OneOfmessage_payload as VerifyingPayload;

/// Signs and verifies the message types in [`Proto::MessageType`]:
///
/// - [`Proto::MessageType::MessageType_raw`] signs the UTF-8 bytes as they are, which is the
///   [cookbook recipe](https://solana.com/developers/cookbook/wallets/sign-message) and what
///   this coin has always done.
/// - [`Proto::MessageType::MessageType_offchain_v0`] wraps them in a Solana off-chain message,
///   header version 0.
///
/// The body itself comes either as a UTF-8 string or as [`Proto::StructuredMessage`], which this
/// library renders so that every platform produces the same bytes from the same fields. The two
/// choices are independent: the envelope decides what surrounds the body, the payload decides
/// what the body is.
pub struct SolanaMessageSigner;

impl SolanaMessageSigner {
    pub fn sign_message_impl(
        _coin: &dyn CoinContext,
        input: Proto::MessageSigningInput,
    ) -> SigningResult<Proto::MessageSigningOutput<'static>> {
        let private_key = ed25519::sha512::PrivateKey::try_from(input.private_key.as_ref())?;
        let signer = SolanaAddress::with_public_key_ed25519(&private_key.public());

        let to_sign = Self::signed_bytes(
            input.message_type,
            &input.application_domain,
            signer,
            Self::signing_body(&input.message_payload)?,
        )?;

        let sign = private_key.sign(to_sign)?;
        let base58_sign = base58::encode(sign.to_bytes().as_slice(), SOLANA_ALPHABET);

        Ok(Proto::MessageSigningOutput {
            signature: base58_sign.into(),
            ..Proto::MessageSigningOutput::default()
        })
    }

    pub fn message_preimage_hashes_impl(
        _coin: &dyn CoinContext,
        input: Proto::MessageSigningInput,
    ) -> SigningResult<CompilerProto::PreSigningOutput<'static>> {
        let private_key = ed25519::sha512::PrivateKey::try_from(input.private_key.as_ref())?;
        let signer = SolanaAddress::with_public_key_ed25519(&private_key.public());

        let to_sign = Self::signed_bytes(
            input.message_type,
            &input.application_domain,
            signer,
            Self::signing_body(&input.message_payload)?,
        )?;

        // There is no hashing for a Solana pre-image: ed25519 signs the whole message, so the
        // pre-image is exactly the bytes the key signs.
        Ok(CompilerProto::PreSigningOutput {
            data: to_sign.into(),
            ..CompilerProto::PreSigningOutput::default()
        })
    }

    pub fn verify_message_impl(
        _coin: &dyn CoinContext,
        input: Proto::MessageVerifyingInput,
    ) -> SigningResult<bool> {
        let public_key = ed25519::sha512::PublicKey::try_from(input.public_key.as_ref())?;
        let signer = SolanaAddress::with_public_key_ed25519(&public_key);

        let signed = Self::signed_bytes(
            input.message_type,
            &input.application_domain,
            signer,
            Self::verifying_body(&input.message_payload)?,
        )?;

        let sign = base58::decode(&input.signature, SOLANA_ALPHABET)
            .tw_err(SigningErrorType::Error_input_parse)
            .context("'signature' is not valid base58")?;
        let sign = ed25519::Signature::try_from(sign.as_slice())?;

        Ok(public_key.verify(sign, signed))
    }

    /// Produces the bytes the key signs. The signer is only used by the off-chain envelope,
    /// which states the key inside the signed bytes.
    fn signed_bytes(
        message_type: Proto::MessageType,
        application_domain: &[u8],
        signer: SolanaAddress,
        body: String,
    ) -> SigningResult<Data> {
        match message_type {
            Proto::MessageType::MessageType_raw => Ok(body.into_bytes()),
            Proto::MessageType::MessageType_offchain_v0 => {
                OffchainMessage::new(application_domain, signer, body)?.encode()
            },
        }
    }

    fn signing_body(payload: &SigningPayload) -> SigningResult<String> {
        match payload {
            SigningPayload::message(msg) => Ok(msg.to_string()),
            SigningPayload::structured_message(msg) => StructuredMessageRenderer::render(msg),
            SigningPayload::None => SigningError::err(SigningErrorType::Error_invalid_params)
                .context("No message to sign: set either 'message' or 'structured_message'"),
        }
    }

    fn verifying_body(payload: &VerifyingPayload) -> SigningResult<String> {
        match payload {
            VerifyingPayload::message(msg) => Ok(msg.to_string()),
            VerifyingPayload::structured_message(msg) => StructuredMessageRenderer::render(msg),
            VerifyingPayload::None => SigningError::err(SigningErrorType::Error_invalid_params)
                .context("No message to verify: set either 'message' or 'structured_message'"),
        }
    }
}

impl MessageSigner for SolanaMessageSigner {
    type MessageSigningInput<'a> = Proto::MessageSigningInput<'a>;
    type MessagePreSigningOutput = CompilerProto::PreSigningOutput<'static>;
    type MessageSigningOutput = Proto::MessageSigningOutput<'static>;
    type MessageVerifyingInput<'a> = Proto::MessageVerifyingInput<'a>;

    fn message_preimage_hashes(
        &self,
        coin: &dyn CoinContext,
        input: Self::MessageSigningInput<'_>,
    ) -> Self::MessagePreSigningOutput {
        Self::message_preimage_hashes_impl(coin, input)
            .unwrap_or_else(|e| signing_output_error!(CompilerProto::PreSigningOutput, e))
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
        coin: &dyn CoinContext,
        input: Self::MessageVerifyingInput<'_>,
    ) -> bool {
        try_or_false!(Self::verify_message_impl(coin, input))
    }
}
