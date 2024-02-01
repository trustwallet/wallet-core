// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::coin_entry::{PublicKeyBytes, SignatureBytes};
use crate::error::{SigningError, SigningErrorType, SigningResult};

pub struct SingleSignaturePubkey {
    pub signature: SignatureBytes,
    pub public_key: PublicKeyBytes,
}

impl SingleSignaturePubkey {
    pub fn from_sign_pubkey_list(
        signatures: Vec<SignatureBytes>,
        public_keys: Vec<PublicKeyBytes>,
    ) -> SigningResult<Self> {
        if signatures.len() > 1 || public_keys.len() > 1 {
            return Err(SigningError(SigningErrorType::Error_no_support_n2n));
        }

        let signature = signatures
            .into_iter()
            .next()
            .ok_or(SigningError(SigningErrorType::Error_signatures_count))?;
        let public_key = public_keys
            .into_iter()
            .next()
            .ok_or(SigningError(SigningErrorType::Error_invalid_params))?;

        Ok(SingleSignaturePubkey {
            signature,
            public_key,
        })
    }

    pub fn from_sign_list(signatures: Vec<SignatureBytes>) -> SigningResult<Self> {
        if signatures.len() > 1 {
            return Err(SigningError(SigningErrorType::Error_no_support_n2n));
        }

        let signature = signatures
            .into_iter()
            .next()
            .ok_or(SigningError(SigningErrorType::Error_signatures_count))?;

        Ok(SingleSignaturePubkey {
            signature,
            public_key: PublicKeyBytes::default(),
        })
    }
}
