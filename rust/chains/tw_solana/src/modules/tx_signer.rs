// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::SolanaAddress;
use crate::modules::PubkeySignatureMap;
use crate::transaction::{versioned, Signature};
use std::collections::HashMap;
use tw_coin_entry::error::{SigningError, SigningErrorType, SigningResult};
use tw_keypair::ed25519;
use tw_keypair::traits::{KeyPairTrait, SigningKeyTrait};
use tw_memory::Data;

pub struct TxSigner;

impl TxSigner {
    pub fn sign_versioned(
        unsigned_msg: versioned::VersionedMessage,
        keys: &[ed25519::sha512::KeyPair],
        external_signatures: &PubkeySignatureMap,
    ) -> SigningResult<versioned::VersionedTransaction> {
        let mut key_signs = HashMap::default();

        let message_encoded = Self::preimage_versioned(&unsigned_msg)?;

        // Add external signatures first, so they can be overriden if corresponding private keys are specified.
        key_signs.extend(external_signatures.clone());

        // Sign the message with all given private keys.
        for private_key in keys {
            let signing_pubkey =
                SolanaAddress::with_public_key_bytes(private_key.public().to_bytes());
            let ed25519_signature = private_key.sign(message_encoded.clone())?;

            key_signs.insert(signing_pubkey, ed25519_signature);
        }

        Self::compile_versioned(unsigned_msg, key_signs)
    }

    pub fn compile_versioned(
        unsigned_msg: versioned::VersionedMessage,
        key_signs: HashMap<SolanaAddress, ed25519::Signature>,
    ) -> SigningResult<versioned::VersionedTransaction> {
        let mut tx = versioned::VersionedTransaction::unsigned(unsigned_msg);

        if key_signs.len() != tx.message.num_required_signatures() {
            return Err(SigningError(SigningErrorType::Error_signatures_count));
        }

        for (signing_pubkey, ed25519_signature) in key_signs {
            // Find an index of the corresponding account.
            let account_index = tx
                .message
                .get_account_index(signing_pubkey)
                .ok_or(SigningError(SigningErrorType::Error_missing_private_key))?;
            let signature_to_reassign = tx
                .signatures
                .get_mut(account_index)
                .ok_or(SigningError(SigningErrorType::Error_signatures_count))?;
            *signature_to_reassign = Signature(ed25519_signature.to_bytes());
        }

        Ok(tx)
    }

    pub fn preimage_versioned(msg: &versioned::VersionedMessage) -> SigningResult<Data> {
        bincode::serialize(&msg).map_err(|_| SigningError(SigningErrorType::Error_internal))
    }
}
