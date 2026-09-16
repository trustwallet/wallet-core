// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::SolanaAddress;
use tw_coin_entry::error::prelude::*;
use tw_hash::H256;
use tw_memory::Data;

/// The 16-byte prefix of every Solana off-chain message. The leading `0xff` can never start a
/// transaction, which is what makes these bytes unsignable as one — the whole point of wrapping
/// a message instead of signing it raw.
pub const SIGNING_DOMAIN: &[u8; 16] = b"\xffsolana offchain";

/// The only header version the standard specifies.
///
/// A later proposal removes the application domain, which is the slot an application's own
/// domain separator lives in. Adopting it would change every signature, so it belongs behind a
/// new `MessageType`, never an edit here.
pub const HEADER_VERSION: u8 = 0;

/// What the body's bytes are, as the header states it.
///
/// Chosen by the caller and never derived from the body. `solana_sdk` derives it, which means
/// editing "hello" to "héllo" silently moves the format byte — and that byte is inside the
/// signed bytes, so the same-looking message signs differently. A caller that asks for a format
/// its body cannot satisfy gets an error instead.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum MessageFormat {
    /// Format 0: the characters `isprint(3)` is true for, `0x20..=0x7e`. **Excludes the newline
    /// and the tab**, so a body of labeled lines cannot use it. It is the only format a hardware
    /// wallet renders on screen; format 1 is blind-sign only.
    RestrictedAscii,
    /// Format 1: UTF-8. What a body of labeled lines needs, at the cost of the display above.
    Utf8,
}

impl MessageFormat {
    /// The byte the header states. The standard's numbering, which is not this enum's order.
    pub fn header_byte(self) -> u8 {
        match self {
            MessageFormat::RestrictedAscii => 0,
            MessageFormat::Utf8 => 1,
        }
    }

    /// Refuses a body the format cannot carry.
    ///
    /// The byte is a claim about these bytes, so it is checked against them here rather than
    /// left to whoever reads the message: a verifier that enforces the format would reject it,
    /// and a hardware wallet that trusts the byte would render whatever the body really holds.
    fn check(self, body: &str) -> SigningResult<()> {
        match self {
            MessageFormat::Utf8 => Ok(()),
            // `' '..='~'` is `0x20..=0x7e`, and a `char` outside ASCII never falls in it.
            MessageFormat::RestrictedAscii => match body.chars().find(|c| !matches!(c, ' '..='~')) {
                None => Ok(()),
                Some(found) => SigningError::err(SigningErrorType::Error_invalid_params).context(
                    format!(
                        "Message format 'restricted ascii' carries only printable ASCII, 0x20..=0x7e, and this body holds {found:?}. A body of labeled lines is one field per line, so it needs 'MessageFormat_utf8'"
                    ),
                ),
            },
        }
    }
}

/// Exactly one key signs one of these messages.
pub const SIGNER_COUNT: u8 = 1;

/// The width of the application domain in the header, which is the width of the hash that
/// usually produces it.
pub const APPLICATION_DOMAIN_SIZE: usize = H256::LEN;

/// The standard's cap for message formats 0 and 1. It covers the preamble and the body
/// **together**, not the body alone, and the standard makes enforcing it a MUST on the verifier
/// as well as on the signer.
pub const MAX_MESSAGE_SIZE: usize = 1232;

/// The fixed header this scheme emits: the signing domain, the header version, the application
/// domain, the message format, the signer count, one signer, and the message length. It is a
/// constant because [`SIGNER_COUNT`] is, and it is 85 bytes rather than `solana_sdk`'s 20 —
/// see [`OffchainMessage`].
pub const PREAMBLE_SIZE: usize =
    SIGNING_DOMAIN.len() + 1 + APPLICATION_DOMAIN_SIZE + 1 + 1 + H256::LEN + 2;

/// What is left for the body once the preamble is paid for: 1147 bytes.
pub const MAX_BODY_SIZE: usize = MAX_MESSAGE_SIZE - PREAMBLE_SIZE;

/// A Solana off-chain message, header version 0, as the off-chain message signing *proposal*
/// lays it out: signing domain, header version, application domain, message format, signer
/// count, signers, length-prefixed body.
///
/// Not the same bytes as `solana_sdk::offchain_message::v0`, which implements a reduced form of
/// the same proposal — a 20-byte preamble of signing domain, version, format and length, with no
/// application domain and no signer list, and a derived rather than pinned format byte. Its
/// 1212-byte ledger cap is that 1232 - 20. So these signatures do not verify with
/// `solana verify-offchain-signature`, and `solana sign-offchain-message` cannot produce them;
/// what they do interoperate with is a service that rebuilds this layout. The application domain
/// is the reason to pay the extra 65 bytes: without it there is no domain separator in the
/// signed bytes at all.
///
/// The parts are public because reproducing a message on another platform, re-verifying a stored
/// signature and debugging a byte mismatch all need them, not just the concatenation.
pub struct OffchainMessage {
    /// The caller's domain separator, in the slot the standard provides for it. It is the
    /// analogue of an EIP-712 domain: it stops a signature over one application's messages from
    /// verifying against another's.
    pub application_domain: H256,
    /// What the header says the body is. Not derived from the body — see [`MessageFormat`].
    pub format: MessageFormat,
    /// The key the header binds the message to. ed25519 has no key recovery, so the signer is
    /// stated in the signed bytes and then proven by the signature — which is not weaker than
    /// recovering it: bytes signed for one address cannot be presented as a proof of another.
    pub signer: SolanaAddress,
    /// The UTF-8 body. The standard says nothing about its content and treats it as opaque
    /// application bytes.
    pub body: String,
}

impl OffchainMessage {
    pub fn new(
        application_domain: &[u8],
        format: MessageFormat,
        signer: SolanaAddress,
        body: String,
    ) -> SigningResult<OffchainMessage> {
        format.check(&body)?;

        let domain_len = application_domain.len();
        let application_domain = H256::try_from(application_domain)
            .tw_err(SigningErrorType::Error_invalid_params)
            .with_context(|| {
                format!(
                    "'application_domain' must be exactly {APPLICATION_DOMAIN_SIZE} bytes, got {domain_len}"
                )
            })?;

        Ok(OffchainMessage {
            application_domain,
            format,
            signer,
            body,
        })
    }

    /// Encodes what the key signs: the signing domain, the header version, the application
    /// domain, the message format, the signer count, the signer, the body length as a
    /// little-endian `u16`, and the body.
    ///
    /// Every width and the byte order are the standard's, not ours.
    pub fn encode(&self) -> SigningResult<Data> {
        // Checked before the length field is written, not only after: the field is a `u16`, and a
        // body over 64 KiB would wrap it into a plausible-looking value. The total check below is
        // the standard's actual rule; this one keeps a wrapped length from ever being built.
        if self.body.len() > MAX_BODY_SIZE {
            return SigningError::err(SigningErrorType::Error_invalid_params).context(format!(
                "The message body is {} bytes, at most {MAX_BODY_SIZE} allowed once the {PREAMBLE_SIZE}-byte preamble is counted",
                self.body.len()
            ));
        }

        let mut encoded = Vec::with_capacity(PREAMBLE_SIZE + self.body.len());
        encoded.extend_from_slice(SIGNING_DOMAIN);
        encoded.push(HEADER_VERSION);
        encoded.extend_from_slice(self.application_domain.as_slice());
        encoded.push(self.format.header_byte());
        encoded.push(SIGNER_COUNT);
        encoded.extend_from_slice(self.signer.bytes().as_slice());
        encoded.extend_from_slice(&(self.body.len() as u16).to_le_bytes());
        encoded.extend_from_slice(self.body.as_bytes());

        // The standard's cap is on these bytes, so it is checked on these bytes.
        if encoded.len() > MAX_MESSAGE_SIZE {
            return SigningError::err(SigningErrorType::Error_invalid_params).context(format!(
                "The off-chain message is {} bytes, at most {MAX_MESSAGE_SIZE} allowed",
                encoded.len()
            ));
        }

        Ok(encoded)
    }
}
