// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_hash::{ripemd::bitcoin_hash_160, H160};
use tw_keypair::tw;

use crate::{
    script::{
        standard_script::{claims, conditions},
        Script, Witness,
    },
    signer::{TxSigningArgs, UtxoToSign},
    signing_mode::SigningMethod,
    transaction::transaction_parts::{Amount, OutPoint},
};

use super::{TransactionInput, TransactionOutput};

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
    pub fn input_builder<F>(mut self, f: F) -> Self
    where
        F: FnOnce(UtxoBuilder) -> (TransactionInput, UtxoToSign),
    {
        let (input, utxo) = f(UtxoBuilder::new());
        self.inputs.push(input);
        self.args.utxos_to_sign.push(utxo);
        self
    }
}

pub struct UtxoBuilder {
    input: TransactionInput,
    script_pubkey: Option<Script>,
    signing_method: Option<SigningMethod>,
    amount: Option<Amount>,
}

impl UtxoBuilder {
    pub fn new() -> Self {
        UtxoBuilder {
            input: TransactionInput {
                previous_output: OutPoint::default(),
                script_sig: Script::default(),
                sequence: 0,
                witness: Witness::default(),
            },
            script_pubkey: None,
            signing_method: None,
            amount: None,
        }
    }
    pub fn previous_output(mut self, previous_output: OutPoint) -> Self {
        self.input.previous_output = previous_output;
        self
    }
    pub fn sequence(mut self, sequence: u32) -> Self {
        self.input.sequence = sequence;
        self
    }
    pub fn p2pkh(
        mut self,
        pubkey: tw::PublicKey,
        amount: Amount,
    ) -> (TransactionInput, UtxoToSign) {
        let h = bitcoin_hash_160(&pubkey.to_bytes());
        let pubkey_hash: H160 = h.as_slice().try_into().expect("hash length is 20 bytes");

        self.script_pubkey = Some(conditions::new_p2pkh(&pubkey_hash));
        self.signing_method = Some(SigningMethod::Legacy);
        self.amount = Some(amount);

        (
            self.input,
            UtxoToSign {
                script_pubkey: self.script_pubkey.expect("script_pubkey is not set"),
                signing_method: self.signing_method.expect("signing_method is not set"),
                amount: self.amount.expect("amount is not set"),
            },
        )
    }
}
