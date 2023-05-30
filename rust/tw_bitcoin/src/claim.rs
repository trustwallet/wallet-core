use bitcoin::ScriptBuf as BTCScriptBuf;
//use secp256k1::{generate_keypair, KeyPair, Secp256k1};
use crate::{Error, PubkeyHash, RecipientHash160, Result, SigHashType, TxInputP2PKH};
use ripemd::{Digest, Ripemd160};
use tw_hash::H256;
use tw_keypair::ecdsa::secp256k1;
use tw_keypair::traits::{KeyPairTrait, SigningKeyTrait};

pub trait TransactionSigner {
    fn claim_p2pkh_with_sighash(
        &self,
        input: &TxInputP2PKH,
        // TODO: Should be wrapped.
        hash: H256,
        sighash: SigHashType,
    ) -> Result<ClaimP2PKH>;

    // P2PKH signer with `SIGHASH_ALL` as default.
    fn claim_p2pkh(&self, input: &TxInputP2PKH, hash: H256) -> Result<ClaimP2PKH> {
        <Self as TransactionSigner>::claim_p2pkh_with_sighash(self, input, hash, SigHashType::All)
    }
}

pub struct ClaimP2PKH {
    sig_len: u8,
    sig: Vec<u8>,
    sighash: SigHashType,
    pubkey_len: u8,
    pubkey: Vec<u8>,
}

impl ClaimP2PKH {
    pub fn into_script_buf(self) -> BTCScriptBuf {
        todo!()
    }
}

impl TransactionSigner for secp256k1::KeyPair {
    fn claim_p2pkh_with_sighash(
        &self,
        input: &TxInputP2PKH,
        hash: H256,
        sighash: SigHashType,
    ) -> Result<ClaimP2PKH> {
        let my_pubkey = PubkeyHash::from_keypair(&self, true)?;

        // If the expected recipient is a RIPEMD160 hash of the COMPRESSED
        // key...
        // TODO: `RecipientHash160` should implement Deref
        let pubkey = if input.recipient == my_pubkey {
            self.public().compressed().to_vec()
        }
        // ... if not, then we check whether it is a RIPEMD160 hash of the
        // UNCOMPRESSEd public key.
        else {
            let my_pubkey = PubkeyHash::from_keypair(&self, false)?;

            if input.recipient == my_pubkey {
                self.public().uncompressed().to_vec()
            } else {
                // Invalid, wrong signer!
                println!("INVALID SIGNER");
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
