// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::coin_entry::{PublicKeyBytes, SignatureBytes};
use crate::error::{SigningError, SigningErrorType, SigningResult};
use tw_keypair::KeyPairError;

pub struct SingleSignaturePubkey<Signature, PublicKey> {
    pub signature: Signature,
    pub public_key: PublicKey,
}

impl<Signature, PublicKey> SingleSignaturePubkey<Signature, PublicKey>
where
    Signature: for<'a> TryFrom<&'a [u8], Error = KeyPairError>,
    PublicKey: for<'a> TryFrom<&'a [u8], Error = KeyPairError>,
{
    pub fn from_sign_pubkey_list(
        signatures: Vec<SignatureBytes>,
        public_keys: Vec<PublicKeyBytes>,
    ) -> SigningResult<Self> {
        if signatures.len() > 1 || public_keys.len() > 1 {
            return Err(SigningError(SigningErrorType::Error_no_support_n2n));
        }

        let signature_data = signatures
            .into_iter()
            .next()
            .ok_or(SigningError(SigningErrorType::Error_signatures_count))?;
        let public_key_data = public_keys
            .into_iter()
            .next()
            .ok_or(SigningError(SigningErrorType::Error_invalid_params))?;

        let signature = Signature::try_from(signature_data.as_slice())?;
        let public_key = PublicKey::try_from(public_key_data.as_slice())?;

        Ok(SingleSignaturePubkey {
            signature,
            public_key,
        })
    }
}
