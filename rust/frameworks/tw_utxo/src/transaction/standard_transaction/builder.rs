// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_hash::{ripemd::bitcoin_hash_160, H160, H256};
use tw_keypair::tw;

use crate::{
    script::{
        standard_script::{claims, conditions},
        Script, Witness,
    },
    sighash::SighashType,
    signer::{TxSigningArgs, UtxoToSign},
    signing_mode::SigningMethod,
    transaction::{transaction_parts::{Amount, OutPoint}, UtxoPreimageArgs},
};

use super::{Transaction, TransactionInput, TransactionOutput};

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
    // pub fn build(input: BitcoinV2::Proto::SigningInput<'_>) -> UtxoResult<super::Transaction> { todo!() }
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
    pub fn sighash_ty(mut self, sighash_ty: SighashType) -> Self {
        self.args.sighash_ty = sighash_ty;
        self
    }
    pub fn input_builder<F>(mut self, f: F) -> Self
    where
        F: FnOnce(UtxoBuilder) -> (TransactionInput, UtxoToSign),
    {
        let (input, utxo) = f(UtxoBuilder::new());
        self.inputs.push(input);
        self.args.utxos_to_sign.push(utxo);
        self
    }
    pub fn output_builder<F>(mut self, f: F) -> Self
    where
        F: FnOnce(OutputBuilder) -> TransactionOutput,
    {
        let out = f(OutputBuilder);
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
            self.args
        )
    }
}

pub struct UtxoBuilder {
    input: TransactionInput,
}

impl UtxoBuilder {
    pub fn new() -> Self {
        UtxoBuilder {
            input: TransactionInput {
                previous_output: OutPoint::default(),
                script_sig: Script::default(),
                sequence: u32::MAX,
                witness: Witness::default(),
            },
        }
    }
    pub fn previous_output(mut self, hash: H256, index: u32) -> Self {
        self.input.previous_output = OutPoint { hash, index };
        self
    }
    pub fn sequence(mut self, sequence: u32) -> Self {
        self.input.sequence = sequence;
        self
    }
    pub fn p2pkh(self, pubkey: tw::PublicKey, amount: Amount) -> (TransactionInput, UtxoToSign) {
        let h = bitcoin_hash_160(&pubkey.to_bytes());
        let pubkey_hash: H160 = h.as_slice().try_into().expect("hash length is 20 bytes");

        (
            self.input,
            UtxoToSign {
                script_pubkey: conditions::new_p2pkh(&pubkey_hash),
                signing_method: SigningMethod::Legacy,
                amount,
            },
        )
    }
    pub fn p2wpkh(self, pubkey: tw::PublicKey, amount: Amount) -> (TransactionInput, UtxoToSign) {
        let h = bitcoin_hash_160(&pubkey.to_bytes());
        let pubkey_hash: H160 = h.as_slice().try_into().expect("hash length is 20 bytes");

        (
            self.input,
            UtxoToSign {
                // Generating special scriptPubkey for P2WPKH.
                script_pubkey: conditions::new_p2wpkh_script_code(&pubkey_hash),
                signing_method: SigningMethod::Segwit,
                amount,
            },
        )
    }
}

pub struct OutputBuilder;

impl OutputBuilder {
    pub fn p2pkh(&self, pubkey: tw::PublicKey, amount: Amount) -> TransactionOutput {
        let h = bitcoin_hash_160(&pubkey.to_bytes());
        let pubkey_hash: H160 = h.as_slice().try_into().expect("hash length is 20 bytes");

        TransactionOutput {
            value: amount,
            script_pubkey: conditions::new_p2pkh(&pubkey_hash),
        }
    }
    pub fn p2wpkh(&self, pubkey: tw::PublicKey, amount: Amount) -> TransactionOutput {
        let h = bitcoin_hash_160(&pubkey.to_bytes());
        let pubkey_hash: H160 = h.as_slice().try_into().expect("hash length is 20 bytes");

        TransactionOutput {
            value: amount,
            script_pubkey: conditions::new_p2wpkh(&pubkey_hash),
        }
    }
}
