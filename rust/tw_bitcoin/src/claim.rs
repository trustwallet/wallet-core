use crate::{
    Error, Recipient, Result, TaprootScript, TxInputP2PKH,
    TxInputP2TRKeyPath, TxInputP2TRScriptPath, TxInputP2WPKH,
};
use bitcoin::key::{KeyPair, PublicKey};
use bitcoin::secp256k1::Secp256k1;
use bitcoin::sighash::{EcdsaSighashType, TapSighashType};
use bitcoin::taproot::{LeafVersion, Signature};
use bitcoin::{ScriptBuf, Witness};

#[derive(Debug, Clone)]
pub enum ClaimLocation {
    Script(ScriptBuf),
    Witness(Witness),
}

pub trait TransactionSigner {
    /// Claiming mechanism for (legacy) P2PKH outputs.
    fn claim_p2pkh(
        &self,
        input: &TxInputP2PKH,
        sighash: secp256k1::Message,
        sighash_type: EcdsaSighashType,
    ) -> Result<ClaimP2PKH>;
    /// Claiming mechanism for SegWit P2WPKH outputs.
    fn claim_p2wpkh(
        &self,
        input: &TxInputP2WPKH,
        sighash: secp256k1::Message,
        sighash_type: EcdsaSighashType,
    ) -> Result<ClaimP2WPKH>;
    /// Claiming mechanism for Taproot P2TR key-path outputs.
    fn claim_p2tr_key_path(
        &self,
        input: &TxInputP2TRKeyPath,
        sighash: secp256k1::Message,
        sighash_type: TapSighashType,
    ) -> Result<ClaimP2TRKeyPath>;
    /// Claiming mechanism for Taproot P2TR script-path outputs.
    fn claim_p2tr_script_path(
        &self,
        input: &TxInputP2TRScriptPath,
        sighash: secp256k1::Message,
        sighash_type: TapSighashType,
    ) -> Result<ClaimP2TRScriptPath>;
}

// Contains the `scriptBuf` that must be included in the transaction when
// spending the P2PKH input.
pub struct ClaimP2PKH(pub ScriptBuf);

// Contains the Witness that must be included in the transaction when spending
// the SegWit P2WPKH input.
pub struct ClaimP2WPKH(pub Witness);

// Contains the Witness that must be included in the transaction when spending
// the Taproot P2TR key-path input.
pub struct ClaimP2TRKeyPath(pub Witness);

// Contains the Witness that must be included in the transaction when spending
// the Taproot P2TR script-path input.
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
        if input.recipient.pubkey_hash() != &me.pubkey_hash() {
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
    fn claim_p2wpkh(
        &self,
        input: &TxInputP2WPKH,
        sighash: secp256k1::Message,
        sighash_type: EcdsaSighashType,
    ) -> Result<ClaimP2WPKH> {
        let me = Recipient::<PublicKey>::from_keypair(self);

        if input.recipient.wpubkey_hash() != &me.wpubkey_hash() {
            return Err(Error::Todo);
        }

        // Construct the signature ECDSA signature.
        let sig = bitcoin::ecdsa::Signature {
            sig: self.secret_key().sign_ecdsa(sighash),
            hash_ty: sighash_type,
        };

        // Construct the Witness for claiming.
        let mut witness = Witness::new();
        witness.push(sig.serialize());
        // Serialize public key.
        witness.push(me.public_key().to_bytes());

        Ok(ClaimP2WPKH(witness))
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

        // The control block contains information on which script of the
        // script-path is being executed.
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
        let mut witness = Witness::new();
        // Serialize signature.
        witness.push(&sig.to_vec());
        witness.push(&input.script);
        witness.push(control_block.serialize());

        todo!()
    }
}
