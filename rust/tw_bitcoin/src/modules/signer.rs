use crate::{Error, Result};
use bitcoin::key::{TapTweak, TweakedKeyPair};
use secp256k1::{KeyPair, Message, Secp256k1};
use tw_coin_entry::coin_entry::{PrivateKeyBytes, SignatureBytes};
use tw_misc::traits::ToBytesVec;
use tw_proto::BitcoinV2::Proto;
use tw_proto::Utxo::Proto as UtxoProto;

pub struct Signer;

impl Signer {
    pub fn signatures_from_proto(
        input: &Proto::PreSigningOutput<'_>,
        private_key: PrivateKeyBytes,
    ) -> Result<Vec<SignatureBytes>> {
        let secp = Secp256k1::new();
        let keypair = KeyPair::from_seckey_slice(&secp, private_key.as_ref())
            .map_err(|_| Error::from(Proto::Error::Error_invalid_private_key))?;

        let mut signatures = vec![];

        for (entry, utxo) in input.sighashes.iter().zip(input.utxo_inputs.iter()) {
            let sighash = Message::from_slice(entry.sighash.as_ref())
                .map_err(|_| Error::from(Proto::Error::Error_invalid_sighash))?;

            match entry.signing_method {
                UtxoProto::SigningMethod::Legacy | UtxoProto::SigningMethod::Segwit => {
                    let sig = bitcoin::ecdsa::Signature {
                        sig: keypair.secret_key().sign_ecdsa(sighash),
                        // TODO
                        hash_ty: bitcoin::sighash::EcdsaSighashType::All,
                    };

                    signatures.push(sig.serialize().to_vec());
                },
                UtxoProto::SigningMethod::TaprootAll
                | UtxoProto::SigningMethod::TaprootOnePrevout => {
                    // Any empty leaf hash implies P2TR key-path (balance transfer)
                    if utxo.leaf_hash.is_empty() {
                        // Tweak keypair for P2TR key-path (ie. zeroed Merkle root).
                        let tapped: TweakedKeyPair = keypair.tap_tweak(&secp, None);
                        let tweaked = KeyPair::from(tapped);

                        dbg!(&sighash);

                        // Construct the Schnorr signature.
                        #[cfg(not(test))]
                        let schnorr = secp.sign_schnorr(&sighash, &tweaked);
                        #[cfg(test)]
                        // For tests, we disable the included randomness in order to create
                        // reproducible signatures. Randomness should ALWAYS be used in
                        // production.
                        let schnorr = secp.sign_schnorr_no_aux_rand(&sighash, &tweaked);

                        let sig = bitcoin::taproot::Signature {
                            sig: schnorr,
                            // TODO.
                            hash_ty: bitcoin::sighash::TapSighashType::Default,
                        };

                        dbg!(&sig);

                        signatures.push(sig.to_vec());
                    }
                    // If it has a leaf hash, then it's a P2TR script-path (complex transaction)
                    else {
                        // We do not tweak the key here since we're passing on
                        // the "control block" when claiming, hence this signing
                        // process is simpler that P2TR key-path.
                        let sig = bitcoin::taproot::Signature {
                            sig: keypair.sign_schnorr(sighash),
                            // TODO.
                            hash_ty: bitcoin::sighash::TapSighashType::Default,
                        };

                        signatures.push(sig.to_vec());
                    }
                },
            }
        }

        Ok(signatures)
    }
}
