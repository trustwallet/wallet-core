extern crate serde;

use crate::claim::{ClaimLocation, TransactionSigner};
use bitcoin::blockdata::locktime::absolute::{Height, LockTime};
use bitcoin::consensus::Encodable;
use bitcoin::key::{KeyPair, TweakedPublicKey, UntweakedPublicKey};
use bitcoin::script::ScriptBuf;
use bitcoin::sighash::{EcdsaSighashType, SighashCache, TapSighashType};
use bitcoin::taproot::{LeafVersion, TapLeafHash, TapNodeHash, TaprootSpendInfo};
use bitcoin::transaction::Transaction;
use bitcoin::{
    secp256k1::{self, XOnlyPublicKey},
    Network, PublicKey,
};
use bitcoin::{Address, OutPoint, PubkeyHash, Sequence, TxIn, TxOut, WPubkeyHash, Witness};

pub mod brc20;
pub mod claim;
pub mod ffi;
pub mod input;
pub mod ordinals;
pub mod output;
mod recipient_impl;
#[cfg(test)]
mod tests;
pub mod utils;

// Reexports
pub use input::*;
pub use output::*;
pub use utils::*;

pub type Result<T> = std::result::Result<T, Error>;

#[derive(Debug, Clone)]
pub struct TaprootProgram {
    // TODO:
    #[allow(dead_code)]
    script: ScriptBuf,
    spend_info: TaprootSpendInfo,
}

#[derive(Debug, Clone)]
pub enum Error {
    Todo,
}

#[derive(Debug, Clone, Eq, PartialEq)]
pub struct Recipient<T> {
    t: T,
}

impl Recipient<PublicKey> {
    pub fn from_keypair(keypair: &KeyPair) -> Self {
        Recipient {
            t: PublicKey::new(keypair.public_key()),
        }
    }
    pub fn public_key(&self) -> PublicKey {
        self.t
    }
    pub fn pubkey_hash(&self) -> PubkeyHash {
        PubkeyHash::from(self.t)
    }
    pub fn wpubkey_hash(&self) -> Result<WPubkeyHash> {
        self.t.wpubkey_hash().ok_or(Error::Todo)
    }
    pub fn tweaked_pubkey(&self) -> TweakedPublicKey {
        tweak_pubkey(self.t)
    }
    pub fn untweaked_pubkey(&self) -> UntweakedPublicKey {
        XOnlyPublicKey::from(self.t.inner)
    }
    pub fn legacy_address(&self, network: Network) -> Address {
        Address::p2pkh(&self.t, network)
    }
    pub fn segwit_address(&self, network: Network) -> Result<Address> {
        Address::p2wpkh(&self.t, network).map_err(|_| Error::Todo)
    }
    pub fn taproot_address(&self, network: Network) -> Address {
        let untweaked = UntweakedPublicKey::from(self.t.inner);
        Address::p2tr(&secp256k1::Secp256k1::new(), untweaked, None, network)
    }
    pub fn legacy_address_string(&self, network: Network) -> String {
        self.legacy_address(network).to_string()
    }
    pub fn segwit_address_string(&self, network: Network) -> Result<String> {
        self.segwit_address(network).map(|addr| addr.to_string())
    }
    pub fn taproot_address_string(&self, network: Network) -> String {
        self.taproot_address(network).to_string()
    }
}

#[derive(Debug, Clone, Eq, PartialEq)]
pub struct TaprootScript {
    pubkey: PublicKey,
    merkle_root: TapNodeHash,
}

impl Recipient<TaprootScript> {
    pub fn from_keypair(keypair: &KeyPair, merkle_root: TapNodeHash) -> Self {
        Recipient {
            t: TaprootScript {
                pubkey: PublicKey::new(keypair.public_key()),
                merkle_root,
            },
        }
    }
    pub fn from_pubkey_recipient(
        recipient: Recipient<PublicKey>,
        merkle_root: TapNodeHash,
    ) -> Self {
        Recipient {
            t: TaprootScript {
                pubkey: recipient.t,
                merkle_root,
            },
        }
    }
    pub fn untweaked_pubkey(&self) -> UntweakedPublicKey {
        XOnlyPublicKey::from(self.t.pubkey.inner)
    }
    pub fn merkle_root(&self) -> TapNodeHash {
        self.t.merkle_root
    }
}

#[derive(Debug, Clone)]
pub struct TransactionBuilder {
    version: i32,
    lock_time: LockTime,
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
        let mut total_satoshi_inputs = 0;
        for input in self.inputs.iter().cloned() {
            total_satoshi_inputs += input.satoshis();

            let btxin = TxIn::from(input);
            tx.input.push(btxin);
        }

        // Prepare the outputs for `bitcoin` crate.
        let mut total_satoshis_outputs = 0;
        for output in self.outputs.iter().cloned() {
            total_satoshis_outputs += output.satoshis();

            let btc_txout = TxOut::from(output);
            tx.output.push(btc_txout);
        }

        // Satoshi output check
        let miner_fee = self.miner_fee.ok_or(Error::Todo)?;
        if total_satoshis_outputs + miner_fee > total_satoshi_inputs {
            return Err(Error::Todo);
        }

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
                TxInput::P2TRScriptPath(p) => {
                    let leaf_hash = TapLeafHash::from_script(&p.script, LeafVersion::TapScript);

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

        Ok(TransactionSigned { tx })
    }
}

pub struct TransactionSigned {
    tx: Transaction,
}

impl TransactionSigned {
    pub fn serialize(&self) -> Result<Vec<u8>> {
        let mut buffer = vec![];
        self.tx
            .consensus_encode(&mut buffer)
            .map_err(|_| Error::Todo)?;

        Ok(buffer)
    }
}

pub struct TxInputsOuputs {
    pub inputs: Vec<TxInput>,
    pub outputs: Vec<TxOutput>,
}

#[derive(Debug, Clone)]
pub struct InputContext {
    pub previous_output: OutPoint,
    pub value: u64,
    // The condition for claiming the output.
    pub script_pubkey: ScriptBuf,
    // TODO: Document this.
    pub sequence: Sequence,
    // Witness data for Segwit/Taproot transactions.
    // TODO: Remove this?
    pub witness: Witness,
}

impl InputContext {
    pub fn new(utxo: TxOut, point: OutPoint) -> Self {
        InputContext {
            previous_output: point,
            value: utxo.value,
            script_pubkey: utxo.script_pubkey,
            // Default value of `0xFFFFFFFF = 4294967295`.
            sequence: Sequence::default(),
            // Empty witness.
            witness: Witness::new(),
        }
    }
}
