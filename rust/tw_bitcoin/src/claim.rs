use crate::{tweak_pubkey, Error, PubkeyHash, Recipient, Result, TxInputP2PKH, TxInputP2TRKeyPath};
use bitcoin::key::{KeyPair, PublicKey};
use bitcoin::secp256k1::Secp256k1;
use bitcoin::sighash::{EcdsaSighashType, TapSighashType};
use bitcoin::{Network, ScriptBuf, Witness};

pub enum ClaimLocation {
    Script(ScriptBuf),
    Witness(Witness),
}

pub trait TransactionSigner {
    fn claim_p2pkh(
        &self,
        input: &TxInputP2PKH,
        hash: secp256k1::Message,
        sighash_type: Option<EcdsaSighashType>,
    ) -> Result<ClaimP2PKH>;

    fn claim_p2tr_key_path(
        &self,
        input: &TxInputP2TRKeyPath,
        hash: secp256k1::Message,
    ) -> Result<ClaimP2TRKeyPath>;
}

pub struct ClaimP2TRKeyPath(pub Witness);

pub struct ClaimP2PKH(pub ScriptBuf);

impl TransactionSigner for KeyPair {
    fn claim_p2tr_key_path(
        &self,
        input: &TxInputP2TRKeyPath,
        sighash: secp256k1::Message,
    ) -> Result<ClaimP2TRKeyPath> {
        // TODO: Pass network as param.
        let me = Recipient::<PublicKey>::from_keypair(self, Network::Regtest);

        // Check whether we can actually claim the input.
        if input.recipient != me {
            return Err(Error::Todo);
        }

        // We the this structure handle the serialization.
        let schnorr_sig = Secp256k1::new().sign_schnorr(&sighash, self);

        // Construct the witness for claiming.
        Ok(ClaimP2TRKeyPath(Witness::from_slice(&[
            schnorr_sig.as_ref()
        ])))
    }

    fn claim_p2pkh(
        &self,
        input: &TxInputP2PKH,
        hash: secp256k1::Message,
        sighash_type: Option<EcdsaSighashType>,
    ) -> Result<ClaimP2PKH> {
        // TODO: Pass network as param.
        let me = Recipient::<PublicKey>::from_keypair(self, Network::Regtest);

        // Check whether we can actually claim the input.
        if input.recipient != me {
            return Err(Error::Todo);
        }

        let sighash_type = sighash_type.unwrap_or(EcdsaSighashType::All);

        // Construct the signature for claiming.
        let sig = bitcoin::ecdsa::Signature {
            sig: self.secret_key().sign_ecdsa(hash),
            hash_ty: sighash_type,
        };

        // Construct the Script for claiming.
        let script = ScriptBuf::builder()
            .push_slice(sig.serialize())
            .push_key(&me.public_key())
            .into_script();

        Ok(ClaimP2PKH(script))
    }
}
