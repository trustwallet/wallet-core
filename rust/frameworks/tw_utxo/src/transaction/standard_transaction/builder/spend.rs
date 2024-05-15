use crate::spending_data::SpendingData;
use crate::{
    script::{standard_script::claims, Script, Witness},
    sighash::{SighashBase, SighashType},
    signature::{BitcoinEcdsaSignature, BitcoinSchnorrSignature},
    transaction::asset::brc20::{BRC20TransferInscription, Brc20Ticker},
};
use tw_coin_entry::error::prelude::*;
use tw_hash::H264;
use tw_keypair::{ecdsa, schnorr, tw};
use tw_misc::traits::ToBytesVec;

pub struct SpendingScriptBuilder {
    sighash_ty: SighashType,
}

impl SpendingScriptBuilder {
    pub fn new() -> Self {
        SpendingScriptBuilder {
            sighash_ty: SighashType::new(SighashBase::All),
        }
    }

    pub fn sighash_ty(mut self, sighash_ty: SighashType) -> Self {
        self.sighash_ty = sighash_ty;
        self
    }

    /// TODO remove.
    pub fn custom_script_sig_witness(
        script_sig: Option<Script>,
        witness: Option<Witness>,
    ) -> SpendingData {
        SpendingData {
            script_sig: script_sig.unwrap_or_default(),
            witness: witness.unwrap_or_default(),
        }
    }

    pub fn p2sh(self, redeem_script: Script) -> SpendingData {
        SpendingData {
            script_sig: redeem_script,
            witness: Witness::default(),
        }
    }

    pub fn p2pkh(
        self,
        sig: ecdsa::secp256k1::Signature,
        pubkey: tw::PublicKey,
    ) -> SigningResult<SpendingData> {
        let sig = BitcoinEcdsaSignature::new(sig.to_der()?, self.sighash_ty)?;

        let pubkey: H264 = pubkey
            .to_secp256k1()
            .or_tw_err(SigningErrorType::Error_internal)
            .context("Expected secp256k1 public key")?
            .compressed();

        let script_sig = claims::new_p2pkh(&sig, &pubkey);

        Ok(SpendingData {
            script_sig,
            witness: Witness::default(),
        })
    }

    // TODO take secp256k1::PublicKey
    pub fn p2wpkh(
        self,
        sig: ecdsa::secp256k1::Signature,
        pubkey: tw::PublicKey,
    ) -> SigningResult<SpendingData> {
        let sig = BitcoinEcdsaSignature::new(sig.to_der()?, self.sighash_ty)?;

        let pubkey: H264 = pubkey
            .to_secp256k1()
            .or_tw_err(SigningErrorType::Error_internal)
            .context("Expected secp256k1 public key")?
            .compressed();

        let witness = claims::new_p2wpkh(&sig, pubkey);

        Ok(SpendingData {
            script_sig: Script::default(),
            witness,
        })
    }

    pub fn p2tr_key_path(self, sig: schnorr::Signature) -> SpendingData {
        let sig = BitcoinSchnorrSignature::new(sig, self.sighash_ty).unwrap();

        let witness = claims::new_p2tr_key_path(sig.serialize());

        SpendingData {
            script_sig: Script::default(),
            witness,
        }
    }

    pub fn p2tr_script_path(
        self,
        sig: schnorr::Signature,
        payload: Script,
        control_block: Vec<u8>,
    ) -> SpendingData {
        let sig = BitcoinSchnorrSignature::new(sig, self.sighash_ty).unwrap();
        let witness = claims::new_p2tr_script_path(&sig, payload, control_block);

        SpendingData {
            script_sig: Script::default(),
            witness,
        }
    }

    pub fn brc20_transfer(
        self,
        sig: schnorr::Signature,
        pubkey: &schnorr::PublicKey,
        ticker: String,
        value: String,
    ) -> SigningResult<SpendingData> {
        let sig = BitcoinSchnorrSignature::new(sig, self.sighash_ty)?;

        let pubkey: H264 = pubkey.compressed();

        let ticker = Brc20Ticker::new(ticker)?;
        let transfer = BRC20TransferInscription::new(&pubkey, &ticker, &value)?;

        let control_block = transfer
            .spend_info
            .control_block(&(
                transfer.script.to_owned(),
                bitcoin::taproot::LeafVersion::TapScript,
            ))
            .or_tw_err(SigningErrorType::Error_internal)
            .context("'TaprootSpendInfo::control_block' is None")?;

        let payload = Script::from(transfer.script.to_vec());

        let witness = claims::new_p2tr_script_path(&sig, payload, control_block.serialize());

        Ok(SpendingData {
            script_sig: Script::default(),
            witness,
        })
    }
}

impl Default for SpendingScriptBuilder {
    fn default() -> Self {
        SpendingScriptBuilder::new()
    }
}
