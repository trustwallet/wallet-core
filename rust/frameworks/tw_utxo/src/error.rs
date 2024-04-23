// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_keypair::KeyPairError;

pub type UtxoErrorKind = tw_proto::Utxo::Proto::Error;
pub type UtxoResult<T> = Result<T, UtxoError>;

#[derive(Debug, Clone)]
pub struct UtxoError(pub UtxoErrorKind);

impl From<KeyPairError> for UtxoError {
    fn from(e: KeyPairError) -> Self {
        match e {
            KeyPairError::InvalidSecretKey => UtxoError(UtxoErrorKind::Error_invalid_private_key),
            KeyPairError::InvalidPublicKey => UtxoError(UtxoErrorKind::Error_invalid_public_key),
            KeyPairError::InvalidSignature => UtxoError(UtxoErrorKind::Error_invalid_signature),
            // TODO Error_signing,
            KeyPairError::InvalidSignMessage | KeyPairError::SigningError => {
                UtxoError(UtxoErrorKind::Error_sighash_failed)
            },
            KeyPairError::SignatureVerifyError => {
                UtxoError(UtxoErrorKind::Error_verifying_signature)
            },
        }
    }
}
