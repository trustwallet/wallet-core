// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::transaction::{versioned, Pubkey, Signature};
use tw_coin_entry::error::{SigningError, SigningErrorType, SigningResult};
use tw_keypair::ed25519;
use tw_keypair::traits::SigningKeyTrait;

pub struct TxSigner;

impl TxSigner {
    pub fn sign_versioned(
        mut tx: versioned::VersionedTransaction,
        keys: &[ed25519::sha512::PrivateKey],
    ) -> SigningResult<versioned::VersionedTransaction> {
        if keys.len() != tx.message.num_required_signatures() {
            return Err(SigningError(SigningErrorType::Error_signatures_count));
        }

        tx.signatures = vec![Signature::default(); tx.message.num_required_signatures()];

        for private_key in keys {
            let signing_pubkey = Pubkey(private_key.public().to_bytes());
            // Find an index of the corresponding account.
            let account_index = tx
                .message
                .get_account_index(signing_pubkey)
                .ok_or(SigningError(SigningErrorType::Error_missing_private_key))?;

            let message_data = bincode::serialize(&tx.message)
                .map_err(|_| SigningError(SigningErrorType::Error_invalid_params))?;
            let ed25519_signature = private_key.sign(message_data)?;

            tx.signatures[account_index] = Signature(ed25519_signature.to_bytes());
        }

        Ok(tx)
    }
}
