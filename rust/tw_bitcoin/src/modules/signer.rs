use crate::{Error, Result};
use bitcoin::key::{TapTweak, TweakedKeyPair};
use bitcoin::sighash::{EcdsaSighashType, TapSighashType};
use secp256k1::{KeyPair, Message, Secp256k1};
use std::collections::HashMap;
use tw_coin_entry::coin_entry::{PrivateKeyBytes, SignatureBytes};
use tw_misc::traits::ToBytesVec;
use tw_proto::BitcoinV2::Proto;
use tw_proto::Utxo::Proto as UtxoProto;

pub struct Signer;

impl Signer {
    pub fn signatures_from_proto(
        input: &Proto::PreSigningOutput<'_>,
        private_key: PrivateKeyBytes,
        individual_keys: HashMap<usize, PrivateKeyBytes>,
        dangerous_use_fixed_schnorr_rng: bool,
    ) -> Result<Vec<SignatureBytes>> {
        let secp = Secp256k1::new();

        let mut signatures = vec![];

        for (index, (entry, utxo)) in input
            .sighashes
            .iter()
            .zip(input.utxo_inputs.iter())
            .enumerate()
        {
            // Check if there's an individual private key for the given input. If not, use the primary one.
            let keypair = if let Some(slice) = individual_keys.get(&index) {
                KeyPair::from_seckey_slice(&secp, slice)
                    .map_err(|_| Error::from(Proto::Error::Error_invalid_private_key))?
            } else {
                KeyPair::from_seckey_slice(&secp, private_key.as_ref())
                    .map_err(|_| Error::from(Proto::Error::Error_invalid_private_key))?
            };

            // Create signable message from sighash.
            let sighash = Message::from_slice(entry.sighash.as_ref())
                .map_err(|_| Error::from(Proto::Error::Error_invalid_sighash))?;

            // Sign the sighash depending on signing method.
            match entry.signing_method {
                // Create a ECDSA signature for legacy and segwit transaction.
                UtxoProto::SigningMethod::Legacy | UtxoProto::SigningMethod::Segwit => {
                    let sighash_type =
                        if let UtxoProto::SighashType::UseDefault = entry.sighash_type {
                            EcdsaSighashType::All
                        } else {
                            EcdsaSighashType::from_consensus(entry.sighash_type as u32)
                        };

                    let sig = bitcoin::ecdsa::Signature {
                        sig: keypair.secret_key().sign_ecdsa(sighash),
                        hash_ty: sighash_type,
                    };

                    signatures.push(sig.serialize().to_vec());
                },
                // Create a Schnorr signature for taproot transactions.
                UtxoProto::SigningMethod::TaprootAll
                | UtxoProto::SigningMethod::TaprootOnePrevout => {
                    // Note that `input.sighash_type = 0` is handled by the underlying library.
                    let sighash_type = TapSighashType::from_consensus_u8(entry.sighash_type as u8)
                        .map_err(|_| Error::from(Proto::Error::Error_utxo_invalid_sighash_type))?;

                    // Any empty leaf hash implies P2TR key-path (balance transfer)
                    if utxo.leaf_hash.is_empty() {
                        // Tweak keypair for P2TR key-path (ie. zeroed Merkle root).
                        let tapped: TweakedKeyPair = keypair.tap_tweak(&secp, None);
                        let tweaked = KeyPair::from(tapped);

                        // Construct the Schnorr signature.
                        let schnorr = if dangerous_use_fixed_schnorr_rng {
                            // For tests, we disable the included randomness in order to create
                            // reproducible signatures. Randomness should ALWAYS be used in
                            // production.
                            secp.sign_schnorr_no_aux_rand(&sighash, &tweaked)
                        } else {
                            secp.sign_schnorr(&sighash, &tweaked)
                        };

                        let sig = bitcoin::taproot::Signature {
                            sig: schnorr,
                            hash_ty: sighash_type,
                        };

                        signatures.push(sig.to_vec());
                    }
                    // If it has a leaf hash, then it's a P2TR script-path (complex transaction)
                    else {
                        // NOTE: We do not tweak the key here since the complex
                        // spending condition(s) must take into account on who
                        // is allowed to spend the input, hence this signing
                        // process is simpler than for P2TR key-path.

                        // Construct the Schnorr signature.
                        let schnorr = if dangerous_use_fixed_schnorr_rng {
                            // For tests, we disable the included randomness in order to create
                            // reproducible signatures. Randomness should ALWAYS be used in
                            // production.
                            secp.sign_schnorr_no_aux_rand(&sighash, &keypair)
                        } else {
                            secp.sign_schnorr(&sighash, &keypair)
                        };

                        let sig = bitcoin::taproot::Signature {
                            sig: schnorr,
                            hash_ty: sighash_type,
                        };

                        signatures.push(sig.to_vec());
                    }
                },
            }
        }

        Ok(signatures)
    }
}
