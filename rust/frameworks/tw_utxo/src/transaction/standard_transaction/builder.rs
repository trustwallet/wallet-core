// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::{
    error::UtxoError,
    sighash::{BitcoinEcdsaSignature, BitcoinSchnorrSignature},
    sighash_computer::SpendingData,
};
use bitcoin::hashes::Hash;
use tw_encoding::hex;
use tw_hash::{hasher::Hasher, ripemd::bitcoin_hash_160, H160, H256, H264};
use tw_keypair::{ecdsa, schnorr, tw};
use tw_misc::traits::ToBytesVec;

use crate::{
    error::{UtxoErrorKind, UtxoResult},
    script::{
        standard_script::{claims, conditions},
        Script, Witness,
    },
    sighash::SighashType,
    sighash_computer::{TxSigningArgs, UtxoToSign},
    signing_mode::SigningMethod,
    transaction::transaction_parts::{Amount, OutPoint},
};

use super::{Transaction, TransactionInput, TransactionOutput};

pub fn txid_from_str(txid: &str) -> UtxoResult<H256> {
    hex::decode(txid)
        .map_err(|_| UtxoError(UtxoErrorKind::Error_internal))?
        .as_slice()
        .try_into()
        .map_err(|_| UtxoError(UtxoErrorKind::Error_internal))
}

pub fn txid_from_str_and_rev(txid: &str) -> UtxoResult<H256> {
    hex::decode(txid)
        .map_err(|_| UtxoError(UtxoErrorKind::Error_internal))?
        .into_iter()
        .rev()
        .collect::<Vec<u8>>()
        .as_slice()
        .try_into()
        .map_err(|_| UtxoError(UtxoErrorKind::Error_internal))
}

/// Transaction builder for standard Bitcoin transaction only.
/// It parses `BitcoinV2::Proto::SigningInput` as the standard [`super::Transaction`].
pub struct TransactionBuilder {
    pub version: u32,
    pub inputs: Vec<TransactionInput>,
    pub outputs: Vec<TransactionOutput>,
    pub locktime: u32,
    pub args: TxSigningArgs,
}

impl TransactionBuilder {
    pub fn new() -> Self {
        TransactionBuilder {
            version: 2,
            inputs: Vec::new(),
            outputs: Vec::new(),
            locktime: 0,
            args: TxSigningArgs::default(),
        }
    }
    pub fn version(mut self, version: u32) -> Self {
        self.version = version;
        self
    }
    pub fn lock_time(mut self, locktime: u32) -> Self {
        self.locktime = locktime;
        self
    }
    pub fn push_input(mut self, input: TransactionInput, arg: UtxoToSign) -> Self {
        self.inputs.push(input);
        self.args.utxos_to_sign.push(arg);
        self
    }
    pub fn push_output(mut self, out: TransactionOutput) -> Self {
        self.outputs.push(out);
        self
    }
    pub fn build(self) -> (Transaction, TxSigningArgs) {
        (
            Transaction {
                // TODO: Why is this i32?
                version: self.version as i32,
                inputs: self.inputs,
                outputs: self.outputs,
                locktime: self.locktime,
            },
            self.args,
        )
    }
}

pub struct UtxoBuilder {
    input: TransactionInput,
    prev_txid: Option<H256>,
    prev_index: Option<u32>,
    amount: Option<Amount>,
}

impl UtxoBuilder {
    pub fn new() -> Self {
        UtxoBuilder {
            input: TransactionInput {
                previous_output: OutPoint::default(),
                script_sig: Script::new(),
                sequence: u32::MAX,
                witness: Witness::new(),
            },
            prev_txid: None,
            prev_index: None,
            amount: None,
        }
    }
    pub fn prev_txid(mut self, txid: H256) -> Self {
        self.prev_txid = Some(txid);
        self
    }
    pub fn prev_txid_str_and_reverse(mut self, txid: &str) -> Self {
        let txid: H256 = hex::decode(txid)
            .unwrap()
            .into_iter()
            .rev()
            .collect::<Vec<u8>>()
            .as_slice()
            .try_into()
            .unwrap();

        self.prev_txid = Some(txid);
        self
    }
    pub fn prev_index(mut self, index: u32) -> Self {
        self.prev_index = Some(index);
        self
    }
    pub fn sequence(mut self, sequence: u32) -> Self {
        self.input.sequence = sequence;
        self
    }
    pub fn custom_script(mut self, custom: Script) -> Self {
        todo!()
    }
    pub fn amount(mut self, amount: Amount) -> Self {
        self.amount = Some(amount);
        self
    }
    // TODO: Can we somehow get rid of this?
    fn finalize_out_point(&mut self) -> UtxoResult<()> {
        // Populate the input with the previous output.
        self.input.previous_output.hash = self
            .prev_txid
            .ok_or(UtxoError(UtxoErrorKind::Error_internal))?;
        self.input.previous_output.index = self
            .prev_index
            .ok_or(UtxoError(UtxoErrorKind::Error_internal))?;

        Ok(())
    }
    pub fn p2pkh(mut self, pubkey: tw::PublicKey) -> UtxoResult<(TransactionInput, UtxoToSign)> {
        let h = bitcoin_hash_160(&pubkey.to_bytes());
        let pubkey_hash: H160 = h.as_slice().try_into().expect("hash length is 20 bytes");

        self.finalize_out_point()?;

        Ok((
            self.input,
            UtxoToSign {
                script_pubkey: conditions::new_p2pkh(&pubkey_hash),
                signing_method: SigningMethod::Legacy,
                amount: self
                    .amount
                    .ok_or(UtxoError(UtxoErrorKind::Error_internal))?,
                ..Default::default()
            },
        ))
    }
    pub fn p2wpkh(mut self, pubkey: tw::PublicKey) -> UtxoResult<(TransactionInput, UtxoToSign)> {
        let h = bitcoin_hash_160(&pubkey.to_bytes());
        let pubkey_hash: H160 = h.as_slice().try_into().expect("hash length is 20 bytes");

        self.finalize_out_point()?;

        Ok((
            self.input,
            UtxoToSign {
                // Generating special scriptPubkey for P2WPKH.
                script_pubkey: conditions::new_p2wpkh_script_code(&pubkey_hash),
                signing_method: SigningMethod::Segwit,
                amount: self
                    .amount
                    .ok_or(UtxoError(UtxoErrorKind::Error_internal))?,
                ..Default::default()
            },
        ))
    }
    pub fn p2tr_key_path(
        mut self,
        pubkey: tw::PublicKey,
    ) -> UtxoResult<(TransactionInput, UtxoToSign)> {
        let pubkey: H264 = pubkey
            .to_bytes()
            .as_slice()
            .try_into()
            .expect("pubkey length is 33 bytes");

        self.finalize_out_point()?;

        Ok((
            self.input,
            UtxoToSign {
                // Generating special scriptPubkey for P2WPKH.
                script_pubkey: conditions::new_p2tr_key_path(&pubkey),
                signing_method: SigningMethod::TaprootAll,
                amount: self
                    .amount
                    .ok_or(UtxoError(UtxoErrorKind::Error_internal))?,
                // Note that we don't use the default double-hasher.
                tx_hasher: Hasher::Sha256,
                ..Default::default()
            },
        ))
    }
    pub fn p2tr_script_path(
        mut self,
        pubkey: tw::PublicKey,
        payloads: &[(u8, Script)],
    ) -> UtxoResult<(TransactionInput, UtxoToSign)> {
        let pubkey: H264 = pubkey
            .to_bytes()
            .as_slice()
            .try_into()
            .expect("pubkey length is 33 bytes");

        let builder = bitcoin::taproot::TaprootBuilder::new();

        // TODO: This logic, including merkle root calculation, should be moved somewhere else.
        for (depth, payload) in payloads {
            let payload = bitcoin::ScriptBuf::from_bytes(payload.as_data().to_vec());
            let leaf_hash = bitcoin::taproot::TapLeafHash::from_script(
                &payload,
                bitcoin::taproot::LeafVersion::TapScript,
            );
            //let leaf_hash: H256 = leaf_hash.to_byte_array().try_into().expect("leaf hash must be 32 bytes");

            builder.add_leaf(*depth, payload).unwrap();
        }

        // The internal key.
        let xonly = bitcoin::secp256k1::XOnlyPublicKey::from(
            bitcoin::PublicKey::from_slice(pubkey.as_slice())
                .unwrap()
                .inner,
        );

        let spend_info = builder
            .finalize(&bitcoin::secp256k1::Secp256k1::new(), xonly)
            .unwrap();

        let merkle_root: H256 = spend_info
            .merkle_root()
            .unwrap()
            .to_byte_array()
            .as_slice()
            .try_into()
            .unwrap();

        let script_pubkey = conditions::new_p2tr_script_path(&pubkey, &merkle_root);

        self.finalize_out_point()?;

        Ok((
            self.input,
            UtxoToSign {
                script_pubkey,
                signing_method: SigningMethod::TaprootAll,
                amount: self
                    .amount
                    .ok_or(UtxoError(UtxoErrorKind::Error_internal))?,
                leaf_hash_code_separator: Some((leaf_hash, 0)),
                // Note that we don't use the default double-hasher.
                tx_hasher: Hasher::Sha256,
                ..Default::default()
            },
        ))
    }
}

pub struct OutputBuilder {
    amount: Option<Amount>,
}

impl OutputBuilder {
    pub fn new() -> Self {
        OutputBuilder { amount: None }
    }
    pub fn amount(mut self, amount: Amount) -> Self {
        self.amount = Some(amount);
        self
    }
    // TODO: Be more precise with PublicKey type?.
    // TODO: There should be a hash-equivalent.
    pub fn p2pkh(self, pubkey: tw::PublicKey) -> UtxoResult<TransactionOutput> {
        let h = bitcoin_hash_160(&pubkey.to_bytes());
        let pubkey_hash: H160 = h.as_slice().try_into().expect("hash length is 20 bytes");

        Ok(TransactionOutput {
            value: self
                .amount
                .ok_or(UtxoError(UtxoErrorKind::Error_internal))?,
            script_pubkey: conditions::new_p2pkh(&pubkey_hash),
        })
    }
    // TODO: Be more precise with PublicKey type?.
    // TODO: There should be a hash-equivalent.
    pub fn p2wpkh(self, pubkey: tw::PublicKey) -> UtxoResult<TransactionOutput> {
        let h = bitcoin_hash_160(&pubkey.to_bytes());
        let pubkey_hash: H160 = h.as_slice().try_into().expect("hash length is 20 bytes");

        Ok(TransactionOutput {
            value: self
                .amount
                .ok_or(UtxoError(UtxoErrorKind::Error_internal))?,
            script_pubkey: conditions::new_p2wpkh(&pubkey_hash),
        })
    }
    pub fn p2tr_key_path(self, pubkey: tw::PublicKey) -> UtxoResult<TransactionOutput> {
        let pubkey: H264 = pubkey
            .to_bytes()
            .as_slice()
            .try_into()
            .expect("pubkey length is 33 bytes");

        Ok(TransactionOutput {
            value: self
                .amount
                .ok_or(UtxoError(UtxoErrorKind::Error_internal))?,
            script_pubkey: conditions::new_p2tr_key_path(&pubkey),
        })
    }
    pub fn p2tr_script_path(
        self,
        pubkey: tw::PublicKey,
        merkle_root: H256,
    ) -> UtxoResult<TransactionOutput> {
        let pubkey: H264 = pubkey
            .to_bytes()
            .as_slice()
            .try_into()
            .expect("pubkey length is 33 bytes");

        Ok(TransactionOutput {
            value: self
                .amount
                .ok_or(UtxoError(UtxoErrorKind::Error_internal))?,
            script_pubkey: conditions::new_p2tr_script_path(&pubkey, &merkle_root),
        })
    }
}

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
}
