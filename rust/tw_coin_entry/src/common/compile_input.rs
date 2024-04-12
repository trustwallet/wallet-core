// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::coin_entry::{PublicKeyBytes, SignatureBytes};
use crate::error::prelude::*;

pub struct SingleSignaturePubkey {
    pub signature: SignatureBytes,
    pub public_key: PublicKeyBytes,
}

impl SingleSignaturePubkey {
    pub fn from_sign_pubkey_list(
        signatures: Vec<SignatureBytes>,
        public_keys: Vec<PublicKeyBytes>,
    ) -> TWResult<Self> {
        if signatures.len() > 1 || public_keys.len() > 1 {
            return TWError::err(TWErrorKind::Error_no_support_n2n)
                .context("Expected exactly one signature and public key");
        }

        let signature = signatures
            .into_iter()
            .next()
            .or_tw_err(TWErrorKind::Error_signatures_count)
            .context("Expected exactly one signature and public key")?;
        let public_key = public_keys
            .into_iter()
            .next()
            .or_tw_err(TWErrorKind::Error_invalid_params)
            .context("Expected exactly one signature and public key")?;

        Ok(SingleSignaturePubkey {
            signature,
            public_key,
        })
    }

    pub fn from_sign_list(signatures: Vec<SignatureBytes>) -> TWResult<Self> {
        if signatures.len() > 1 {
            return TWError::err(TWErrorKind::Error_no_support_n2n)
                .context("Expected exactly one signature");
        }

        let signature = signatures
            .into_iter()
            .next()
            .or_tw_err(TWErrorKind::Error_signatures_count)
            .context("Expected exactly one signature")?;

        Ok(SingleSignaturePubkey {
            signature,
            public_key: PublicKeyBytes::default(),
        })
    }
}
