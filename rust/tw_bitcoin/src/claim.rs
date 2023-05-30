use bitcoin::blockdata::script::PushBytesBuf as BTCPushBytesBuf;
use bitcoin::consensus::Encodable;
use bitcoin::{ScriptBuf as BTCScriptBuf, VarInt};
//use secp256k1::{generate_keypair, KeyPair, Secp256k1};
use crate::{Error, PubkeyHash, Result, SigHashType, TxInputP2PKH};
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
    sig: Vec<u8>,
    sighash: SigHashType,
    pubkey: Vec<u8>,
}

impl ClaimP2PKH {
    // TODO: Should unwraps be handled?
    pub fn into_script_buf(self) -> BTCScriptBuf {
        // We let the `bitcoin` crate handle the encoding of the public key.
        let pubkey = bitcoin::key::PublicKey::from_slice(&self.pubkey).unwrap();

        let mut buf = BTCPushBytesBuf::new();
        // DER-encoding handles the prefix with the length of the signature. No
        // need to do it manually.
        buf.extend_from_slice(&self.sig).unwrap();
        buf.push(self.sighash.to_le_byte()).unwrap();

        BTCScriptBuf::builder()
            .push_slice(buf)
            .push_key(&pubkey)
            .into_script()
    }
}

impl TransactionSigner for secp256k1::KeyPair {
    fn claim_p2pkh_with_sighash(
        &self,
        input: &TxInputP2PKH,
        hash: H256,
        sighash: SigHashType,
    ) -> Result<ClaimP2PKH> {
        let my_pubkey = PubkeyHash::from_keypair(self, true)?;

        // If the expected recipient is a RIPEMD160 hash of the COMPRESSED
        // key...
        // TODO: `RecipientHash160` should implement Deref
        let pubkey = if input.recipient == my_pubkey {
            self.public().compressed().to_vec()
        }
        // ... if not, then we check whether it is a RIPEMD160 hash of the
        // UNCOMPRESSEd public key.
        else {
            let my_pubkey = PubkeyHash::from_keypair(self, false)?;

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

        Ok(
            // The expected script to claim the P2PKH.
            ClaimP2PKH {
                // Signature length, one byte.
                // Signature, 70-72 bytes and DER-encoded.
                sig: der_sig.as_bytes().to_vec(),
                // SIGHASH type, one byte.
                sighash,
                // Public key length, one byte.
                // Public key itself, compressed (33 bytes) or uncompressed (65 bytes).
                pubkey,
            },
        )
    }
}
