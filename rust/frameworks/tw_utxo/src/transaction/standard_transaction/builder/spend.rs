use crate::{
    error::UtxoError,
    error::{UtxoErrorKind, UtxoResult},
    script::{
        standard_script::{claims, conditions},
        Script, Witness,
    },
    sighash::SighashType,
    sighash::{BitcoinEcdsaSignature, BitcoinSchnorrSignature},
    sighash_computer::SpendingData,
    sighash_computer::{TxSigningArgs, UtxoToSign},
    signing_mode::SigningMethod,
    transaction::asset::brc20::{BRC20TransferInscription, Brc20Ticker},
    transaction::transaction_parts::{Amount, OutPoint},
};
use bitcoin::{hashes::Hash, taproot::TaprootSpendInfo};
use tw_encoding::hex;
use tw_hash::{hasher::Hasher, ripemd::bitcoin_hash_160, H160, H256, H264};
use tw_keypair::{ecdsa, schnorr, tw};
use tw_misc::traits::ToBytesVec;

pub struct SpendingScriptBuilder {
    sighash_ty: Option<SighashType>,
}

impl SpendingScriptBuilder {
    pub fn new() -> Self {
        SpendingScriptBuilder { sighash_ty: None }
    }
    pub fn sighash_ty(mut self, sighash_ty: SighashType) -> Self {
        self.sighash_ty = Some(sighash_ty);
        self
    }
    pub fn p2pkh(
        self,
        sig: ecdsa::secp256k1::Signature,
        pubkey: tw::PublicKey,
    ) -> UtxoResult<SpendingData> {
        // TODO: Check unwrap
        let sig = BitcoinEcdsaSignature::new(
            sig.to_der().unwrap(),
            self.sighash_ty
                .ok_or(UtxoError(UtxoErrorKind::Error_internal))?,
        )
        .unwrap();

        let pubkey: H264 = pubkey
            .to_bytes()
            .as_slice()
            .try_into()
            .expect("pubkey length is 33 bytes");

        let script_sig = claims::new_p2pkh(&sig.serialize(), &pubkey);

        Ok(SpendingData {
            script_sig,
            witness: Witness::default(),
        })
    }
    pub fn p2wpkh(
        self,
        sig: ecdsa::secp256k1::Signature,
        pubkey: tw::PublicKey,
    ) -> UtxoResult<SpendingData> {
        // TODO: Check unwrap
        let sig = BitcoinEcdsaSignature::new(
            sig.to_der().unwrap(),
            self.sighash_ty
                .ok_or(UtxoError(UtxoErrorKind::Error_internal))?,
        )
        .unwrap();

        let pubkey: H264 = pubkey
            .to_bytes()
            .as_slice()
            .try_into()
            .expect("pubkey length is 33 bytes");

        let witness = claims::new_p2wpkh(sig.serialize(), pubkey);

        Ok(SpendingData {
            script_sig: Script::default(),
            witness,
        })
    }
    pub fn p2tr_key_path(self, sig: schnorr::Signature) -> UtxoResult<SpendingData> {
        let sig = BitcoinSchnorrSignature::new(
            sig,
            self.sighash_ty
                .ok_or(UtxoError(UtxoErrorKind::Error_internal))?,
        )
        .unwrap();

        let witness = claims::new_p2tr_key_path(sig.serialize());

        Ok(SpendingData {
            script_sig: Script::default(),
            witness,
        })
    }
    pub fn p2tr_script_path(
        self,
        sig: schnorr::Signature,
        payload: Script,
        control_block: Vec<u8>,
    ) -> UtxoResult<SpendingData> {
        let sig = BitcoinSchnorrSignature::new(
            sig,
            self.sighash_ty
                .ok_or(UtxoError(UtxoErrorKind::Error_internal))?,
        )
        .unwrap();

        let witness = claims::new_p2tr_script_path(sig.serialize(), payload, control_block);

        Ok(SpendingData {
            script_sig: Script::default(),
            witness,
        })
    }
}
