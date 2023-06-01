use bitcoin::blockdata::script::PushBytesBuf as BPushBytesBuf;
use bitcoin::{ScriptBuf as BScriptBuf, Witness as BWitness};
//use secp256k1::{generate_keypair, KeyPair, Secp256k1};
use crate::{
    Error, PubkeyHash, Result, SigHashType, TaprootPubkey, TxInputP2PKH, TxInputP2TRKeySpend,
};
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

    fn claim_p2tr_key_spend_sighash(
        &self,
        input: &TxInputP2TRKeySpend,
        hash: H256,
        sighash: SigHashType,
    ) -> Result<ClaimP2TRKeySpend>;

    fn claim_p2tr_key_spend(
        &self,
        input: &TxInputP2TRKeySpend,
        hash: H256,
    ) -> Result<ClaimP2TRKeySpend> {
        <Self as TransactionSigner>::claim_p2tr_key_spend_sighash(
            self,
            input,
            hash,
            SigHashType::All,
        )
    }

    // P2PKH signer with `SIGHASH_ALL` as default.
    fn claim_p2pkh(&self, input: &TxInputP2PKH, hash: H256) -> Result<ClaimP2PKH> {
        <Self as TransactionSigner>::claim_p2pkh_with_sighash(self, input, hash, SigHashType::All)
    }
}

pub struct ClaimP2TRKeySpend(BWitness);

// TODO: Create `BScriptBuf` directly, skip this structure.
pub struct ClaimP2PKH {
    sig: Vec<u8>,
    sighash: SigHashType,
    pubkey: Vec<u8>,
}

impl ClaimP2PKH {
    // TODO: Should unwraps be handled?
    pub fn into_script(self) -> BScriptBuf {
        // We let the `bitcoin` crate handle the encoding of the public key.
        let pubkey = bitcoin::key::PublicKey::from_slice(&self.pubkey).unwrap();

        let mut buf = BPushBytesBuf::new();
        // DER-encoding handles the prefix with the length of the signature. No
        // need to do it manually.
        buf.extend_from_slice(&self.sig).unwrap();
        buf.push(self.sighash.to_le_byte()).unwrap();

        BScriptBuf::builder()
            .push_slice(buf)
            .push_key(&pubkey)
            .into_script()
    }
}

impl TransactionSigner for secp256k1::KeyPair {
    fn claim_p2tr_key_spend_sighash(
        &self,
        input: &TxInputP2TRKeySpend,
        hash: H256,
        sighash: SigHashType,
    ) -> Result<ClaimP2TRKeySpend> {
        let my_pubkey = TaprootPubkey::from_keypair(self)?;

        if input.recipient != my_pubkey {
            return Err(Error::Todo);
        }

        let sig = self.private().sign_schnorr_bip340(hash);

        Ok(
            // This is weird, but the `from_slice` function wants a parameter that.
            // TODO: Open an issue?
            ClaimP2TRKeySpend(BWitness::from_slice(&[sig.to_bytes()])),
        )
    }

    fn claim_p2pkh_with_sighash(
        &self,
        input: &TxInputP2PKH,
        hash: H256,
        sighash: SigHashType,
    ) -> Result<ClaimP2PKH> {
        // If the expected recipient is the hash of the COMPRESSED key...
        let compressed = true;
        let my_pubkey = PubkeyHash::from_keypair(self, compressed)?;
        let pubkey = if input.recipient == my_pubkey {
            self.public().compressed().to_vec()
        }
        // ... if not, then we check whether the recipient is the hash of the
        // UNCOMPRESSEd public key.
        else {
            let compressed = false;
            let my_pubkey = PubkeyHash::from_keypair(self, compressed)?;

            if input.recipient == my_pubkey {
                self.public().uncompressed().to_vec()
            } else {
                // Invalid/wrong signer!
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
                // Signature, 70-72 bytes and DER-encoded.
                sig: der_sig.as_bytes().to_vec(),
                // SIGHASH type, one byte.
                sighash,
                // Public key itself, compressed (33 bytes) or uncompressed (65 bytes).
                pubkey,
            },
        )
    }
}
