use bitcoin::blockdata::script::PushBytesBuf as BPushBytesBuf;
use bitcoin::sighash::{EcdsaSighashType, TapSighashType};
use bitcoin::{ScriptBuf as BScriptBuf, Witness as BWitness};
use bitcoin::secp256k1;
//use secp256k1::{generate_keypair, KeyPair, Secp256k1};
use crate::{
    Error, PubkeyHash, Result, TxInputP2PKH, TxInputP2TRKeySpend,
};

pub enum ClaimLocation {
    Script(BScriptBuf),
    Witness(BWitness),
}

pub trait TransactionSigner {
    fn claim_p2pkh_with_sighash(
        &self,
        input: &TxInputP2PKH,
        // TODO: Should be wrapped.
        hash: secp256k1::Message,
        sighash: EcdsaSighashType,
    ) -> Result<ClaimP2PKH>;

    fn claim_p2tr_key_spend_sighash(
        &self,
        input: &TxInputP2TRKeySpend,
        hash: secp256k1::Message,
        sighash: TapSighashType,
    ) -> Result<ClaimP2TRKeySpend>;

    fn claim_p2tr_key_spend(
        &self,
        input: &TxInputP2TRKeySpend,
        hash: secp256k1::Message,
    ) -> Result<ClaimP2TRKeySpend> {
        <Self as TransactionSigner>::claim_p2tr_key_spend_sighash(
            self,
            input,
            hash,
            TapSighashType::All,
        )
    }

    // P2PKH signer with `SIGHASH_ALL` as default.
    fn claim_p2pkh(&self, input: &TxInputP2PKH, hash: secp256k1::Message) -> Result<ClaimP2PKH> {
        <Self as TransactionSigner>::claim_p2pkh_with_sighash(self, input, hash, EcdsaSighashType::All)
    }
}

pub struct ClaimP2TRKeySpend(BWitness);

// TODO: Create `BScriptBuf` directly, skip this structure.
pub struct ClaimP2PKH(pub BScriptBuf);

impl TransactionSigner for secp256k1::KeyPair {
    fn claim_p2tr_key_spend_sighash(
        &self,
        input: &TxInputP2TRKeySpend,
        hash: secp256k1::Message,
        sighash: TapSighashType,
    ) -> Result<ClaimP2TRKeySpend> {
        /*
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
        */
        todo!()
    }

    fn claim_p2pkh_with_sighash(
        &self,
        input: &TxInputP2PKH,
        hash: secp256k1::Message,
        sighash: EcdsaSighashType,
    ) -> Result<ClaimP2PKH> {
        let my_pubkey = bitcoin::PublicKey::new(self.public_key());
        if input.recipient != PubkeyHash::from(my_pubkey) {
            return Err(Error::Todo)
        }

        // Sign the hash of the transaction data (OP_CHECKSIG).
        //let sig = self.pri().sign(hash).map_err(|_| Error::Todo)?;
        // Encode signature as DER.
        //let der_sig = sig.to_der();
        let sig = bitcoin::ecdsa::Signature::sighash_all(self.secret_key().sign_ecdsa(hash));

        let script = BScriptBuf::builder()
            .push_slice(sig.serialize())
            .push_key(&my_pubkey)
            .into_script();

        Ok(ClaimP2PKH(script))
    }
}
