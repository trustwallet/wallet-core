// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::modules::sighash_computer::{SighashComputer, UtxoSighash};
use crate::signature::FromRawOrDerBytes;
use crate::signing_mode::SigningMethod;
use crate::transaction::transaction_interface::TransactionInterface;
use crate::transaction::unsigned_transaction::UnsignedTransaction;
use crate::transaction::{TransactionPreimage, UtxoToSign};
use std::marker::PhantomData;
use tw_coin_entry::coin_entry::SignatureBytes;
use tw_coin_entry::error::prelude::*;
use tw_encoding::hex::ToHex;
use tw_keypair::traits::VerifyingKeyTrait;
use tw_keypair::{ecdsa, schnorr};

/// Sighash Verifier with a standard Bitcoin behaviour.
///
/// # Important
///
/// If needed to implement a custom logic, consider adding a different Sighash Verifier.
pub struct SighashVerifier<Transaction> {
    _phantom: PhantomData<Transaction>,
}

impl<Transaction> SighashVerifier<Transaction>
where
    Transaction: TransactionPreimage + TransactionInterface,
{
    /// Validates if the given signatures and public keys match the actual [`SighashComputer::transaction`] inputs.
    /// The method should be used to validate externally passed signatures before [`SighashComputer::compile`].
    ///
    /// # Hint
    ///
    /// Not required when [`SighashComputer::compile`] is called right after [`SighashComputer::preimage_tx`],
    /// as this method is expensive in terms of computations.
    pub fn verify_signatures(
        unsigned_tx: &UnsignedTransaction<Transaction>,
        signatures: &[SignatureBytes],
    ) -> SigningResult<()> {
        Self::check_signatures_number(unsigned_tx.inputs().len(), signatures.len())?;

        // Compute transaction preimage and verify if all given signatures correspond to the result sighashes.
        let tx_preimage = SighashComputer::preimage_tx(unsigned_tx)?;

        for ((sighash, utxo_args), signature) in tx_preimage
            .sighashes
            .iter()
            .zip(unsigned_tx.input_args())
            .zip(signatures)
        {
            Self::verify_signature(sighash, utxo_args, signature)?;
        }

        Ok(())
    }

    pub fn verify_signature(
        sighash: &UtxoSighash,
        utxo_args: &UtxoToSign,
        signature: &[u8],
    ) -> SigningResult<()> {
        match utxo_args.signing_method {
            SigningMethod::Legacy | SigningMethod::Segwit => {
                Self::verify_ecdsa_signature(sighash, &utxo_args.spender_public_key, signature)
            },
            SigningMethod::Taproot => {
                Self::verify_schnorr_signature(sighash, &utxo_args.spender_public_key, signature)
            },
        }
    }

    pub fn verify_ecdsa_signature(
        sighash: &UtxoSighash,
        public_key: &[u8],
        signature: &[u8],
    ) -> SigningResult<()> {
        let public_key = ecdsa::secp256k1::PublicKey::try_from(public_key)
            .into_tw()
            .with_context(|| format!("Error invalid ecdsa public key: {}", public_key.to_hex()))?;

        let sign = ecdsa::secp256k1::VerifySignature::from_raw_or_der_bytes(signature)?;

        if !public_key.verify(sign, sighash.sighash) {
            return SigningError::err(SigningErrorType::Error_signing).context(format!(
                "Error verifying ecdsa signature: sighash='{}', pubkey='{}', signature='{}'",
                sighash.sighash,
                public_key.compressed(),
                signature.to_hex()
            ));
        }

        Ok(())
    }

    /// Please note that the public key must be tweaked already if needed.
    pub fn verify_schnorr_signature(
        sighash: &UtxoSighash,
        public_key: &[u8],
        signature: &[u8],
    ) -> SigningResult<()> {
        let public_key = schnorr::XOnlyPublicKey::try_from(public_key)
            .into_tw()
            .with_context(|| {
                format!(
                    "Error invalid schnorr x-only public key: {}",
                    public_key.to_hex()
                )
            })?;

        let sign = schnorr::Signature::try_from(signature)
            .into_tw()
            .context("Given an invalid schnorr signature")?;

        if !public_key.verify(sign, sighash.sighash) {
            return SigningError::err(SigningErrorType::Error_signing).context(format!(
                "Error verifying schnorr signature: sighash='{}', pubkey='{}', signature='{}'",
                sighash.sighash,
                public_key.bytes(),
                signature.to_hex()
            ));
        }

        Ok(())
    }

    pub(crate) fn check_signatures_number(
        inputs_len: usize,
        signatures_len: usize,
    ) -> SigningResult<()> {
        if inputs_len != signatures_len {
            return SigningError::err(SigningErrorType::Error_signatures_count).context(format!(
                "SighashComputer's error: there are '{inputs_len}' transaction inputs, but given '{signatures_len}' signatures",
            ));
        }
        Ok(())
    }
}
