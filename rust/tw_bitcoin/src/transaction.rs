use crate::claim::{ClaimLocation, TransactionSigner};
use crate::input::*;
use crate::output::*;
use crate::{Error, Result};
use bitcoin::blockdata::locktime::absolute::{Height, LockTime};
use bitcoin::consensus::Encodable;
use bitcoin::sighash::{EcdsaSighashType, SighashCache, TapSighashType};
use bitcoin::taproot::{LeafVersion, TapLeafHash};
use bitcoin::{secp256k1, Address, TxIn, TxOut};
use bitcoin::{Transaction, Weight};

/// Determines the weight of the transaction and calculates the fee with the
/// given satoshis per vbyte.
pub fn calculate_fee(tx: &Transaction, sat_vb: u64) -> (Weight, u64) {
    let weight = tx.weight();
    (weight, weight.to_vbytes_ceil() * sat_vb)
}

#[derive(Debug, Clone)]
pub struct TransactionBuilder {
    pub version: i32,
    pub lock_time: LockTime,
    inputs: Vec<TxInput>,
    outputs: Vec<TxOutput>,
    miner_fee: Option<u64>,
    return_address: Option<Address>,
    contains_taproot: bool,
}

impl Default for TransactionBuilder {
    fn default() -> Self {
        TransactionBuilder {
            version: 2,
            // No lock time, transaction is immediately spendable.
            lock_time: LockTime::Blocks(Height::ZERO),
            inputs: vec![],
            outputs: vec![],
            miner_fee: None,
            return_address: None,
            contains_taproot: false,
        }
    }
}

impl TransactionBuilder {
    pub fn new() -> Self {
        Self::default()
    }
    pub fn version(mut self, version: i32) -> Self {
        self.version = version;
        self
    }
    pub fn lock_time_height(mut self, height: u32) -> Result<Self> {
        self.lock_time = LockTime::Blocks(Height::from_consensus(height).map_err(|_| Error::Todo)?);
        Ok(self)
    }
    pub fn return_address(mut self, address: Address) -> Self {
        self.return_address = Some(address);
        self
    }
    pub fn miner_fee(mut self, satoshis: u64) -> Self {
        self.miner_fee = Some(satoshis);
        self
    }
    pub fn add_input(mut self, input: TxInput) -> Self {
        match input {
            TxInput::P2TRKeyPath(_) | TxInput::P2TRScriptPath(_) => self.contains_taproot = true,
            _ => {},
        }

        self.inputs.push(input);
        self
    }
    pub fn add_output(mut self, output: TxOutput) -> Self {
        self.outputs.push(output);
        self
    }
    pub fn sign_inputs<S>(self, signer: S) -> Result<TransactionSigned>
    where
        S: TransactionSigner,
    {
        self.sign_inputs_fn(|input, sighash| match input {
            TxInput::P2PKH(p) => signer
                .claim_p2pkh(p, sighash, EcdsaSighashType::All)
                .map(|claim| ClaimLocation::Script(claim.0)),
            TxInput::P2WPKH(p) => signer
                .claim_p2wpkh(p, sighash, EcdsaSighashType::All)
                .map(|claim| ClaimLocation::Witness(claim.0)),
            TxInput::P2TRKeyPath(p) => signer
                .claim_p2tr_key_path(p, sighash, TapSighashType::Default)
                .map(|claim| ClaimLocation::Witness(claim.0)),
            TxInput::P2TRScriptPath(p) => signer
                .claim_p2tr_script_path(p, sighash, TapSighashType::Default)
                .map(|claim| ClaimLocation::Witness(claim.0)),
        })
    }
    pub fn sign_inputs_fn<F>(self, signer: F) -> Result<TransactionSigned>
    where
        F: Fn(&TxInput, secp256k1::Message) -> Result<ClaimLocation>,
    {
        // Prepare boilerplate transaction for `bitcoin` crate.
        let mut tx = Transaction {
            version: self.version,
            lock_time: self.lock_time,
            input: vec![],
            output: vec![],
        };

        // Prepare the inputs for `bitcoin` crate.
        for input in self.inputs.iter().cloned() {
            let btxin = TxIn::from(input);
            tx.input.push(btxin);
        }

        // Prepare the outputs for `bitcoin` crate.
        for output in self.outputs.iter().cloned() {
            let btc_txout = TxOut::from(output);
            tx.output.push(btc_txout);
        }

        // Satoshi output check
        /*
        // TODO: This should be enabled, eventually.
        let miner_fee = self.miner_fee.ok_or(Error::Todo)?;
        if total_satoshis_outputs + miner_fee > total_satoshi_inputs {
            return Err(Error::Todo);
        }
        */

        // If Taproot is enabled, we prepare the full `TxOuts` (value and
        // scriptPubKey) for hashing, which will then be signed. What
        // distinguishes this from legacy signing is that the output value in
        // satoshis is actually part of the signature.
        let mut prevouts = vec![];
        if self.contains_taproot {
            for input in &self.inputs {
                prevouts.push(TxOut {
                    value: input.ctx().value,
                    script_pubkey: input.ctx().script_pubkey.clone(),
                });
            }
        }

        let mut cache = SighashCache::new(tx);

        let mut claims = vec![];

        // For each input (index), we create a hash which is to be signed.
        for (index, input) in self.inputs.iter().enumerate() {
            match input {
                TxInput::P2PKH(p2pkh) => {
                    let hash = cache
                        .legacy_signature_hash(
                            index,
                            &p2pkh.ctx().script_pubkey,
                            EcdsaSighashType::All.to_u32(),
                        )
                        .map_err(|_| Error::Todo)?;

                    let message = secp256k1::Message::from_slice(hash.as_ref())
                        .expect("Sighash must always convert to secp256k1::Message");
                    let updated = signer(input, message)?;

                    claims.push((index, updated));
                },
                TxInput::P2WPKH(p2wpkh) => {
                    let hash = cache
                        .segwit_signature_hash(
                            index,
                            p2wpkh
                                .ctx()
                                .script_pubkey
                                .p2wpkh_script_code()
                                .as_ref()
                                .expect("P2WPKH builder must set the script code correctly"),
                            p2wpkh.ctx().value,
                            EcdsaSighashType::All,
                        )
                        .map_err(|_| Error::Todo)?;

                    let message = secp256k1::Message::from_slice(hash.as_ref())
                        .expect("Sighash must always convert to secp256k1::Message");
                    let updated = signer(input, message)?;

                    claims.push((index, updated));
                },
                TxInput::P2TRKeyPath(_) => {
                    let hash = cache
                        .taproot_key_spend_signature_hash(
                            index,
                            &bitcoin::sighash::Prevouts::All(&prevouts),
                            TapSighashType::Default,
                        )
                        .map_err(|_| Error::Todo)?;

                    let message = secp256k1::Message::from_slice(hash.as_ref())
                        .expect("Sighash must always convert to secp256k1::Message");
                    let updated = signer(input, message)?;

                    claims.push((index, updated));
                },
                TxInput::P2TRScriptPath(p2trsp) => {
                    let leaf_hash =
                        TapLeafHash::from_script(p2trsp.witness(), LeafVersion::TapScript);

                    let hash = cache
                        .taproot_script_spend_signature_hash(
                            index,
                            &bitcoin::sighash::Prevouts::All(&prevouts),
                            leaf_hash,
                            TapSighashType::Default,
                        )
                        .map_err(|_| Error::Todo)?;

                    let message = secp256k1::Message::from_slice(hash.as_ref())
                        .expect("Sighash must always convert to secp256k1::Message");
                    let updated = signer(input, message)?;

                    claims.push((index, updated));
                },
            };
        }

        let mut tx = cache.into_transaction();

        // Update the transaction with the updated scriptSig/Witness.
        for (index, claim_loc) in claims {
            match claim_loc {
                ClaimLocation::Script(script) => {
                    tx.input[index].script_sig = script;
                },
                ClaimLocation::Witness(witness) => {
                    tx.input[index].witness = witness;
                },
            }
        }

        Ok(TransactionSigned { inner: tx })
    }
}

pub struct TransactionSigned {
    pub inner: Transaction,
}

impl TransactionSigned {
    pub fn serialize(&self) -> Result<Vec<u8>> {
        let mut buffer = vec![];
        self.inner
            .consensus_encode(&mut buffer)
            .map_err(|_| Error::Todo)?;

        Ok(buffer)
    }
}
