// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::SolanaAddress;
use crate::modules::offchain_message::{MessageFormat, OffchainMessage};
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

        let to_sign = Envelope::signing(&input, Some(signer))
            .signed_bytes(Self::signing_body(&input.message_payload)?)?;

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
        // Only the type that states a signer inside the signed bytes reads a key here. Raw
        // signs the body and nothing else, and never read one before the off-chain types
        // existed, so a caller that passes none — or a malformed one — still gets its bytes.
        let signer = match input.message_type {
            Proto::MessageType::MessageType_raw => None,
            Proto::MessageType::MessageType_offchain_v0 => Self::preimage_signer(&input)?,
        };

        let to_sign = Envelope::signing(&input, signer)
            .signed_bytes(Self::signing_body(&input.message_payload)?)?;

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

        let signed = Envelope::verifying(&input, signer)
            .signed_bytes(Self::verifying_body(&input.message_payload)?)?;

        let sign = base58::decode(&input.signature, SOLANA_ALPHABET)
            .tw_err(SigningErrorType::Error_input_parse)
            .context("'signature' is not valid base58")?;
        let sign = ed25519::Signature::try_from(sign.as_slice())?;

        Ok(public_key.verify(sign, signed))
    }

    /// The signer a pre-image is built for. `public_key` comes first because the flow this
    /// method exists for — external signing — has no private key to derive one from; a caller
    /// that does have the key can still pass it instead. Neither is needed for
    /// [`Proto::MessageType::MessageType_raw`], which signs the body and nothing else, so an
    /// absent signer is returned rather than refused here.
    fn preimage_signer(input: &Proto::MessageSigningInput) -> SigningResult<Option<SolanaAddress>> {
        if !input.public_key.is_empty() {
            let public_key = ed25519::sha512::PublicKey::try_from(input.public_key.as_ref())?;
            return Ok(Some(SolanaAddress::with_public_key_ed25519(&public_key)));
        }

        if !input.private_key.is_empty() {
            let private_key = ed25519::sha512::PrivateKey::try_from(input.private_key.as_ref())?;
            return Ok(Some(SolanaAddress::with_public_key_ed25519(
                &private_key.public(),
            )));
        }

        Ok(None)
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

/// The proto's numbering is its own — `MessageFormat_utf8` is 0 there so that an input naming no
/// format keeps signing what it signed before the field existed — and the header's numbering is
/// the standard's. This is the one place the two meet.
impl From<Proto::MessageFormat> for MessageFormat {
    fn from(format: Proto::MessageFormat) -> MessageFormat {
        match format {
            Proto::MessageFormat::MessageFormat_utf8 => MessageFormat::Utf8,
            Proto::MessageFormat::MessageFormat_restricted_ascii => MessageFormat::RestrictedAscii,
        }
    }
}

/// The off-chain header fields, read out of whichever input they arrived in so that signing, a
/// pre-image and verifying build the bytes by one set of rules.
struct Envelope<'a> {
    message_type: Proto::MessageType,
    format: Proto::MessageFormat,
    /// The caller's own 32-byte domain separator.
    application_domain: &'a [u8],
    /// Absent only when a pre-image is asked for with no key, which
    /// [`Proto::MessageType::MessageType_raw`] can do without and the envelope cannot.
    signer: Option<SolanaAddress>,
}

impl<'a> Envelope<'a> {
    fn signing(input: &'a Proto::MessageSigningInput, signer: Option<SolanaAddress>) -> Self {
        Envelope {
            message_type: input.message_type,
            format: input.message_format,
            application_domain: &input.application_domain,
            signer,
        }
    }

    fn verifying(input: &'a Proto::MessageVerifyingInput, signer: SolanaAddress) -> Self {
        Envelope {
            message_type: input.message_type,
            format: input.message_format,
            application_domain: &input.application_domain,
            signer: Some(signer),
        }
    }

    /// Produces the bytes the key signs.
    fn signed_bytes(self, body: String) -> SigningResult<Data> {
        match self.message_type {
            Proto::MessageType::MessageType_raw => {
                // Refused rather than ignored. `MessageType_raw` is the proto3 default, so an
                // input that sets a header field and forgets the type would otherwise be signed
                // without it — a plausible-looking signature over the wrong bytes, which the
                // verifier rejects for no visible reason.
                if !self.application_domain.is_empty() {
                    return SigningError::err(SigningErrorType::Error_invalid_params).context(
                        "'application_domain' is set, but 'message_type' is 'MessageType_raw', which signs the body bare and has nowhere to state a domain separator. Set 'message_type' to 'MessageType_offchain_v0', or clear 'application_domain' to sign raw on purpose",
                    );
                }

                if self.format != Proto::MessageFormat::MessageFormat_utf8 {
                    return SigningError::err(SigningErrorType::Error_invalid_params).context(
                        "'message_format' is set, but 'message_type' is 'MessageType_raw', which signs the body bare and has no header to state a format in. Set 'message_type' to 'MessageType_offchain_v0' to have the format mean something",
                    );
                }

                // These bytes carry nothing that marks them as not-a-transaction, the way the
                // envelope's 0xff prefix does. Being UTF-8 stands in for it: a transaction
                // message would need the signer's pubkey and a blockhash as raw bytes inside
                // the text, and a versioned one a leading 0x80. A `bytes` payload would lose
                // that and need the check — `signing_body` stops compiling on one.
                Ok(body.into_bytes())
            },
            Proto::MessageType::MessageType_offchain_v0 => {
                let signer = self
                    .signer
                    .or_tw_err(SigningErrorType::Error_invalid_params)
                    .context("An off-chain message states its signer inside the signed bytes: set 'public_key' (or 'private_key') to build these bytes")?;

                OffchainMessage::new(self.application_domain, self.format.into(), signer, body)?
                    .encode()
            },
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
