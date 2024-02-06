// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::transaction::{versioned, Pubkey, Signature};
use tw_coin_entry::error::{SigningError, SigningErrorType, SigningResult};
use tw_keypair::ed25519;
use tw_keypair::traits::SigningKeyTrait;
use tw_memory::Data;

pub struct TxSigner;

impl TxSigner {
    pub fn sign_versioned(
        mut tx: versioned::VersionedTransaction,
        keys: &[ed25519::sha512::PrivateKey],
    ) -> SigningResult<versioned::VersionedTransaction> {
        if keys.len() != tx.message.num_required_signatures() {
            return Err(SigningError(SigningErrorType::Error_signatures_count));
        }

        tx.zeroize_signatures();
        let message_data = bincode::serialize(&tx.message)
            .map_err(|_| SigningError(SigningErrorType::Error_invalid_params))?;

        for private_key in keys {
            let signing_pubkey = Pubkey(private_key.public().to_bytes());
            // Find an index of the corresponding account.
            let account_index = tx
                .message
                .get_account_index(signing_pubkey)
                .ok_or(SigningError(SigningErrorType::Error_missing_private_key))?;
            let signature_to_reassign = tx
                .signatures
                .get_mut(account_index)
                .ok_or(SigningError(SigningErrorType::Error_signatures_count))?;

            let ed25519_signature = private_key.sign(message_data.clone())?;
            *signature_to_reassign = Signature(ed25519_signature.to_bytes());
        }

        Ok(tx)
    }

    pub fn preimage_versioned(tx: &versioned::VersionedTransaction) -> SigningResult<Data> {
        bincode::serialize(&tx.message)
            .map_err(|_| SigningError(SigningErrorType::Error_invalid_params))
    }
}
