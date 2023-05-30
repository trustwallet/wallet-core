use bitcoin::{TxOut, ScriptBuf};
//use secp256k1::{generate_keypair, KeyPair, Secp256k1};
use crate::{Error, Result, SigHashType};
use ripemd::{Digest, Ripemd160};
use tw_hash::H256;
use tw_keypair::ecdsa::secp256k1;
use tw_keypair::traits::{KeyPairTrait, SigningKeyTrait};

pub trait TransactionSigner {
    fn claim_p2pkh_with_sighash(
        &self,
        script_pubkey: &ScriptBuf,
        hash: H256,
        sighash: SigHashType,
    ) -> Result<ClaimP2PKH>;

    // P2PKH signer with `SIGHASH_ALL` as default.
    fn claim_p2pkh(&self, script_pubkey: &ScriptBuf, hash: H256) -> Result<ClaimP2PKH> {
        <Self as TransactionSigner>::claim_p2pkh_with_sighash(self, script_pubkey, hash, SigHashType::All)
    }
}

pub struct ClaimP2PKH {
    sig_len: u8,
    sig: Vec<u8>,
    sighash: SigHashType,
    pubkey_len: u8,
    pubkey: Vec<u8>,
}

impl TransactionSigner for secp256k1::KeyPair {
    fn claim_p2pkh_with_sighash(
        &self,
        script_pubkey: &ScriptBuf,
        hash: H256,
        sighash: SigHashType,
    ) -> Result<ClaimP2PKH> {
        // Sanity check; make sure the scriptPubKey is actually a `P2PKH` script.
        if script_pubkey.is_p2pkh() {
            return Err(Error::Todo);
        }

        // Extract the expected recipient from the scriptPubKey.
		//
        // script[0] == OP_DUP
        // script[1] == OP_HASH160
        // script[2] == OP_PUSHBYTES_20
        // script[3..23] == <PUBKEY-RIPEMD160>
        // script[23] == OP_EQUALVERIFY
        // script[24] == OP_CHECKSIG
        let expected_recipient = &script_pubkey.as_bytes()[3..23];

        // The expected recipient is a RIPEMD160 hash, so first we check
        // whether it's a hash from the COMPRESSED public key.
        let mut hasher = Ripemd160::new();
        hasher.update(self.public().compressed().as_slice());
        let finalized = hasher.finalize();
        let hashed_pubkey = &finalized[..];

        debug_assert_eq!(expected_recipient.len(), hashed_pubkey.len());

        // If the expected recipient is a RIPEMD160 hash of the COMPRESSED
        // key...
        let pubkey = if expected_recipient == hashed_pubkey {
            self.public().compressed().to_vec()
        }
        // ... if not, then we check whether it is a RIPEMD160 hash of the
        // UNCOMPRESSEd public key.
        else {
            let mut hasher = Ripemd160::new();
            hasher.update(self.public().uncompressed().as_slice());
            let finalized = hasher.finalize();
            let hashed_pubkey = &finalized[..];

            if expected_recipient == hashed_pubkey {
                self.public().uncompressed().to_vec()
            } else {
                // Invalid, wrong signer!
                return Err(Error::Todo);
            }
        };

        // Sign the hash of the transaction data (OP_CHECKSIG).
        let sig = self.private().sign(hash).map_err(|_| Error::Todo)?;
        // Encode signature as DER.
        let der_sig = sig.to_der();

        debug_assert_eq!(der_sig.len(), 70);
        debug_assert!(
            pubkey.len() == secp256k1::PublicKey::COMPRESSED
                || pubkey.len() == secp256k1::PublicKey::UNCOMPRESSED
        );

        Ok(
            // The expected script to claim the P2PKH.
            ClaimP2PKH {
                // Signature length, one byte.
                sig_len: der_sig.len() as u8,
                // Signature, 70-72 bytes and DER-encoded.
                sig: der_sig.as_bytes().to_vec(),
                // SIGHASH type, one byte.
                sighash,
                // Public key length, one byte.
                pubkey_len: pubkey.len() as u8,
                // Public key itself, compressed (33 bytes) or uncompressed (65 bytes).
                pubkey,
            },
        )
    }
}
