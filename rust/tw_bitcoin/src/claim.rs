use crate::{
    tweak_pubkey, Error, PubkeyHash, Recipient, Result, TaprootScript, TxInputP2PKH,
    TxInputP2TRKeyPath, TxInputP2TRScriptPath,
};
use bitcoin::key::{KeyPair, PublicKey};
use bitcoin::secp256k1::Secp256k1;
use bitcoin::sighash::{EcdsaSighashType, TapSighashType};
use bitcoin::taproot::{ControlBlock, LeafVersion, Signature};
use bitcoin::{ScriptBuf, Witness};

pub enum ClaimLocation {
    Script(ScriptBuf),
    Witness(Witness),
}

pub trait TransactionSigner {
    fn claim_p2pkh(
        &self,
        input: &TxInputP2PKH,
        sighash: secp256k1::Message,
        sighash_type: EcdsaSighashType,
    ) -> Result<ClaimP2PKH>;
    fn claim_p2tr_key_path(
        &self,
        input: &TxInputP2TRKeyPath,
        sighash: secp256k1::Message,
        sighash_type: TapSighashType,
    ) -> Result<ClaimP2TRKeyPath>;
    fn claim_p2tr_script_path(
        &self,
        input: &TxInputP2TRScriptPath,
        sighash: secp256k1::Message,
        sighash_type: TapSighashType,
    ) -> Result<ClaimP2TRScriptPath>;
}

pub struct ClaimP2PKH(pub ScriptBuf);

pub struct ClaimP2TRKeyPath(pub Witness);

pub struct ClaimP2TRScriptPath(pub Witness);

impl TransactionSigner for KeyPair {
    fn claim_p2pkh(
        &self,
        input: &TxInputP2PKH,
        sighash: secp256k1::Message,
        sighash_type: EcdsaSighashType,
    ) -> Result<ClaimP2PKH> {
        // TODO: Pass network as param.
        let me = Recipient::<PublicKey>::from_keypair(self);

        // Check whether we can actually claim the input.
        if input.recipient != Recipient::<PubkeyHash>::from(me.clone()) {
            return Err(Error::Todo);
        }

        // Construct the signature ECDSA signature.
        let sig = bitcoin::ecdsa::Signature {
            sig: self.secret_key().sign_ecdsa(sighash),
            hash_ty: sighash_type,
        };

        // Construct the Script for claiming.
        let script = ScriptBuf::builder()
            .push_slice(sig.serialize())
            .push_key(&me.public_key())
            .into_script();

        Ok(ClaimP2PKH(script))
    }
    fn claim_p2tr_key_path(
        &self,
        input: &TxInputP2TRKeyPath,
        sighash: secp256k1::Message,
        sighash_type: TapSighashType,
    ) -> Result<ClaimP2TRKeyPath> {
        let me = Recipient::<PublicKey>::from_keypair(self);

        // Check whether we can actually claim the input.
        if input.recipient != me {
            return Err(Error::Todo);
        }

        // Construct the Schnorr signature.
        let sig = Signature {
            sig: Secp256k1::new().sign_schnorr(&sighash, self),
            hash_ty: sighash_type,
        };

        // Construct the witness for claiming.
        Ok(ClaimP2TRKeyPath(Witness::from_slice(&[
            // Serialize signature.
            &sig.to_vec(),
        ])))
    }
    fn claim_p2tr_script_path(
        &self,
        input: &TxInputP2TRScriptPath,
        sighash: secp256k1::Message,
        sighash_type: TapSighashType,
    ) -> Result<ClaimP2TRScriptPath> {
        let me = Recipient::<TaprootScript>::from_keypair(self, input.recipient.merkle_root());

        // Check whether we can actually claim the input.
        if input.recipient != me {
            return Err(Error::Todo);
        }

        let control_block = input
            .spend_info
            // TODO: Can cloning be avoided here?
            .control_block(&(input.script.clone(), LeafVersion::TapScript))
            .ok_or(Error::Todo)?;

        // Construct the Schnorr signature.
        let sig = Signature {
            sig: Secp256k1::new().sign_schnorr(&sighash, self),
            hash_ty: sighash_type,
        };

        // Construct the Witness for claiming.
        let mut witness = Witness::default();
        // Serialize signature.
        witness.push(&sig.to_vec());
        witness.push(&input.script);
        witness.push(control_block.serialize());

        todo!()
    }
}