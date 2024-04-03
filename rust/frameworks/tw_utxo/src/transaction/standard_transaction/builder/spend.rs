use crate::{
    error::UtxoResult,
    script::{standard_script::claims, Script, Witness},
    sighash::{BitcoinEcdsaSignature, BitcoinSchnorrSignature, SighashBase, SighashType},
    sighash_computer::SpendingData,
    transaction::asset::brc20::{BRC20TransferInscription, Brc20Ticker},
};

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
    pub fn custom_script_sig_witness(
        script_sig: Option<Script>,
        witness: Option<Witness>,
    ) -> SpendingData {
        SpendingData {
            script_sig: script_sig.unwrap_or_default(),
            witness: witness.unwrap_or_default(),
        }
    }
    pub fn p2sh(self, redeem_script: Script) -> UtxoResult<SpendingData> {
        Ok(SpendingData {
            script_sig: redeem_script,
            witness: Witness::default(),
        })
    }
    pub fn p2pkh(
        self,
        sig: ecdsa::secp256k1::Signature,
        pubkey: tw::PublicKey,
    ) -> UtxoResult<SpendingData> {
        // TODO: Check unwrap
        let sig = BitcoinEcdsaSignature::new(sig.to_der().unwrap(), self.sighash_ty).unwrap();

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
        let sig = BitcoinEcdsaSignature::new(sig.to_der().unwrap(), self.sighash_ty).unwrap();

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
        let sig = BitcoinSchnorrSignature::new(sig, self.sighash_ty).unwrap();

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
        let sig = BitcoinSchnorrSignature::new(sig, self.sighash_ty).unwrap();

        let witness = claims::new_p2tr_script_path(sig.serialize(), payload, control_block);

        Ok(SpendingData {
            script_sig: Script::default(),
            witness,
        })
    }
    pub fn brc20_transfer(
        self,
        sig: schnorr::Signature,
        pubkey: tw::PublicKey,
        ticker: String,
        value: String,
    ) -> UtxoResult<SpendingData> {
        let sig = BitcoinSchnorrSignature::new(sig, self.sighash_ty).unwrap();

        let pubkey: H264 = pubkey
            .to_bytes()
            .as_slice()
            .try_into()
            .expect("pubkey length is 33 bytes");

        let ticker = Brc20Ticker::new(ticker).unwrap();
        let transfer = BRC20TransferInscription::new(&pubkey, &ticker, &value).unwrap();

        let control_block = transfer
            .spend_info
            .control_block(&(
                transfer.script.to_owned(),
                bitcoin::taproot::LeafVersion::TapScript,
            ))
            .unwrap();

        let payload = Script::from(transfer.script.to_vec());

        let witness =
            claims::new_p2tr_script_path(sig.serialize(), payload, control_block.serialize());

        Ok(SpendingData {
            script_sig: Script::default(),
            witness,
        })
    }
}
