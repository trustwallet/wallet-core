// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::modules::sighash_verifier::SighashVerifier;
use crate::sighash::SighashType;
use crate::signature::{BitcoinEcdsaSignature, BitcoinSchnorrSignature, FromRawOrDerBytes};
use crate::spending_data::{
    EcdsaSpendingDataConstructor, SchnorrSpendingDataConstructor, SpendingData,
    SpendingDataConstructor,
};
use crate::transaction::transaction_interface::{TransactionInterface, TxInputInterface};
use crate::transaction::unsigned_transaction::UnsignedTransaction;
use crate::transaction::TransactionPreimage;
use std::marker::PhantomData;
use tw_coin_entry::coin_entry::SignatureBytes;
use tw_coin_entry::error::prelude::*;
use tw_keypair::{ecdsa, schnorr};

/// Transaction Compiler with a standard Bitcoin behaviour.
///
/// # Important
///
/// If needed to implement a custom logic, consider adding a different Transaction Compiler.
pub struct TxCompiler<Transaction> {
    _phantom: PhantomData<Transaction>,
}

impl<Transaction> TxCompiler<Transaction>
where
    Transaction: TransactionPreimage + TransactionInterface,
{
    /// Tries to compile an unsigned transaction with the given signatures.
    ///
    /// # Important
    ///
    /// The given signatures must be verified via [`SighashVerifier::verify_signatures`]
    /// if they were provided by an external sighash signer (remotely, outside of the WalletCore library).
    pub fn compile(
        unsigned_tx: UnsignedTransaction<Transaction>,
        signatures: &[SignatureBytes],
    ) -> SigningResult<Transaction> {
        SighashVerifier::<Transaction>::check_signatures_number(
            unsigned_tx.inputs().len(),
            signatures.len(),
        )?;

        let claims = unsigned_tx
            .input_args()
            .iter()
            .zip(signatures)
            .map(|(utxo, sign)| match utxo.spending_data_constructor {
                SpendingDataConstructor::Ecdsa(ref ecdsa_constructor) => {
                    Self::get_ecdsa_spending_data(ecdsa_constructor.as_ref(), sign, utxo.sighash_ty)
                },
                SpendingDataConstructor::Schnorr(ref schnorr_constructor) => {
                    Self::get_schnorr_spending_data(
                        schnorr_constructor.as_ref(),
                        sign,
                        utxo.sighash_ty,
                    )
                },
            })
            .collect::<SigningResult<_>>()?;

        Self::compile_spending_data(unsigned_tx, claims)
    }

    /// Compiles the transaction with the given spending data.
    pub fn compile_spending_data(
        unsigned_tx: UnsignedTransaction<Transaction>,
        claims: Vec<SpendingData>,
    ) -> SigningResult<Transaction> {
        SighashVerifier::<Transaction>::check_signatures_number(
            unsigned_tx.inputs().len(),
            claims.len(),
        )?;

        let mut signed_tx = unsigned_tx.into_transaction();

        // Add the claiming script (scriptSig or Witness) to the transaction inputs.
        for (utxo, claim) in signed_tx.inputs_mut().iter_mut().zip(claims.into_iter()) {
            if claim.script_sig.is_empty() && claim.witness.is_empty() {
                return SigningError::err(SigningErrorType::Error_invalid_params).context(
                    "Either scriptSig or witness must be provided to compile a transaction",
                );
            }

            utxo.set_script_sig(claim.script_sig);
            utxo.set_witness(claim.witness);
        }

        Ok(signed_tx)
    }

    fn get_ecdsa_spending_data(
        ecdsa_constructor: &dyn EcdsaSpendingDataConstructor,
        sign: &[u8],
        sighash_ty: SighashType,
    ) -> SigningResult<SpendingData> {
        let ecdsa_sign = ecdsa::secp256k1::VerifySignature::from_raw_or_der_bytes(sign)?;
        let der_sign = ecdsa_sign
            .to_der()
            .into_tw()
            .context("Error getting ASN.1 DER-encoded signature")?;

        let signature = BitcoinEcdsaSignature::new(der_sign, sighash_ty)?;
        Ok(ecdsa_constructor.get_spending_data(&signature))
    }

    fn get_schnorr_spending_data(
        schnorr_constructor: &dyn SchnorrSpendingDataConstructor,
        sign: &[u8],
        sighash_ty: SighashType,
    ) -> SigningResult<SpendingData> {
        let schnorr_sign = schnorr::Signature::try_from(sign)
            .into_tw()
            .context("Invalid schnorr signature")?;

        let signature = BitcoinSchnorrSignature::new(schnorr_sign, sighash_ty)?;
        Ok(schnorr_constructor.get_spending_data(&signature))
    }
}
