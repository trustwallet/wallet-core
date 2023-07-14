// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_keypair::{
    tw::{PublicKey, Signature},
};
use crate::error::{SigningError, SigningErrorType, SigningResult};

pub struct SingleSignaturePubkey {
    pub signature: Signature,
    pub public_key: PublicKey,
}

impl SingleSignaturePubkey {
    fn from_sign_pubkey_list(
        signatures: Vec<Signature>,
        public_keys: Vec<PublicKey>,
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
}
