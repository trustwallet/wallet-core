use crate::{tweak_pubkey, Error, PubkeyHash, Result, TxInputP2PKH, TxInputP2TRKeySpend};
use bitcoin::key::{KeyPair, PublicKey};
use bitcoin::secp256k1::Secp256k1;
use bitcoin::sighash::{EcdsaSighashType, TapSighashType};
use bitcoin::{ScriptBuf, Witness};

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

    fn claim_p2tr_key_spend(
        &self,
        input: &TxInputP2TRKeySpend,
        hash: secp256k1::Message,
        sighash_type: Option<TapSighashType>,
    ) -> Result<ClaimP2TRKeySpend>;
}

pub struct ClaimP2TRKeySpend(pub Witness);

pub struct ClaimP2PKH(pub ScriptBuf);

impl TransactionSigner for KeyPair {
    fn claim_p2tr_key_spend(
        &self,
        input: &TxInputP2TRKeySpend,
        sighash: secp256k1::Message,
        sighash_type: Option<TapSighashType>,
    ) -> Result<ClaimP2TRKeySpend> {
        // Given that we're using the "key spend" method of P2TR, we tweak the
        // key without a Merkle root.
        let my_pubkey = tweak_pubkey(PublicKey::new(self.public_key()));

        // Check whether we can actually claim the input.
        if input.recipient != my_pubkey {
            return Err(Error::Todo);
        }

        let sighash_type = sighash_type.unwrap_or(TapSighashType::All);

        // We the this structure handle the serialization.
        let schnorr_sig = bitcoin::taproot::Signature {
            sig: Secp256k1::new().sign_schnorr(&sighash, self),
            hash_ty: sighash_type,
        };

        // Construct the witness for claiming.
        Ok(ClaimP2TRKeySpend(Witness::from_slice(&[
            schnorr_sig.to_vec()
        ])))
    }

    fn claim_p2pkh(
        &self,
        input: &TxInputP2PKH,
        hash: secp256k1::Message,
        sighash_type: Option<EcdsaSighashType>,
    ) -> Result<ClaimP2PKH> {
        let my_pubkey = bitcoin::PublicKey::new(self.public_key());

        // Check whether we can actually claim the input.
        if input.recipient != PubkeyHash::from(my_pubkey) {
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
            .push_key(&my_pubkey)
            .into_script();

        Ok(ClaimP2PKH(script))
    }
}
