#[macro_use]
extern crate serde;

use crate::claim::{ClaimLocation, TransactionSigner};
use bitcoin::address::NetworkChecked;
use bitcoin::blockdata::locktime::absolute::{Height, LockTime};
use bitcoin::consensus::Encodable;
use bitcoin::hashes::Hash;
use bitcoin::key::{KeyPair, TapTweak, TweakedPublicKey, UntweakedPublicKey};
use bitcoin::script::{PushBytesBuf, ScriptBuf};
use bitcoin::sighash::TapSighashType;
use bitcoin::sighash::{EcdsaSighashType, LegacySighash, SighashCache, TapSighash};
use bitcoin::taproot::TapNodeHash;
use bitcoin::transaction::Transaction;
use bitcoin::{
    network,
    secp256k1::{self, XOnlyPublicKey},
    Network, PublicKey,
};
use bitcoin::{Address, OutPoint, PubkeyHash, Sequence, TxIn, TxOut, Witness};

pub mod brc20;
pub mod claim;
pub mod input;
pub mod ordinals;
pub mod output;
pub mod utils;

// Reexports
pub use input::*;
pub use output::*;
pub use utils::*;

pub type Result<T> = std::result::Result<T, Error>;

// TODO: Deprecate this.
pub struct TransactionHash([u8; 32]);

impl TransactionHash {
    pub fn from_legacy_sig_hash(hash: LegacySighash) -> Self {
        TransactionHash(hash.to_byte_array())
    }
    pub fn from_tapsig_hash(hash: TapSighash) -> Self {
        TransactionHash(hash.to_byte_array())
    }
}

impl From<TransactionHash> for secp256k1::Message {
    fn from(hash: TransactionHash) -> Self {
        // Never fails since the byte array is always 32 bytes.
        secp256k1::Message::from_slice(&hash.0).unwrap()
    }
}

#[derive(Debug, Clone)]
pub enum Error {
    Todo,
}

/*
#[test]
fn poc() {
    let secp = Secp256k1::new();
    let (private, public) = generate_keypair(&mut rand::thread_rng());
    let tweaked = UntweakedPublicKey::from(public);

    let script1 = ScriptBuf::new();
    let script2 = ScriptBuf::new();

    let spend_info = TaprootBuilder::new()
        .add_leaf(1, script1.clone())
        .unwrap()
        .add_leaf(1, script2)
        .unwrap()
        .finalize(&secp, tweaked)
        .unwrap();

    let root = spend_info.merkle_root().unwrap();

    let tapscript = ScriptBuf::new_v1_p2tr(&secp, tweaked, Some(root));

    let control_block = spend_info.control_block(&(script1, LeafVersion::TapScript));
}
*/

#[derive(Debug, Clone, Eq, PartialEq)]
pub struct Recipient<T> {
    t: T,
}

impl Recipient<PubkeyHash> {
    pub fn from_keypair(keypair: &KeyPair) -> Self {
        Recipient {
            t: PublicKey::new(keypair.public_key()).into(),
        }
    }
    pub fn pubkey_hash(&self) -> &PubkeyHash {
        &self.t
    }
}

impl From<Recipient<PublicKey>> for Recipient<PubkeyHash> {
    fn from(val: Recipient<PublicKey>) -> Self {
        Recipient {
            t: PubkeyHash::from(val.t),
        }
    }
}

impl Recipient<PublicKey> {
    pub fn from_wif(wif: &str) -> Result<Self> {
        let keypair = keypair_from_wif(wif)?;
        Ok(Self::from_keypair(&keypair))
    }
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
    pub fn tweaked_pubkey(&self) -> TweakedPublicKey {
        tweak_pubkey(self.t)
    }
    pub fn untweaked_pubkey(&self) -> UntweakedPublicKey {
        XOnlyPublicKey::from(self.t.inner)
    }
    pub fn legacy_address(&self, network: Network) -> Address {
        Address::p2pkh(&self.t, network)
    }
    pub fn segwit_address(&self, network: Network) -> Address {
        // The key is always compressed.
        debug_assert!(self.t.compressed);
        // "Will only return an Error if an uncompressed public key is provided."
        Address::p2wpkh(&self.t, network).unwrap()
    }
    pub fn taproot_address(&self, network: Network) -> Address {
        let untweaked = UntweakedPublicKey::from(self.t.inner);
        Address::p2tr(&secp256k1::Secp256k1::new(), untweaked, None, network)
    }
    pub fn legacy_address_string(&self, network: Network) -> String {
        self.legacy_address(network).to_string()
    }
    pub fn segwit_address_string(&self, network: Network) -> String {
        self.segwit_address(network).to_string()
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
    network: Network,
    version: i32,
    lock_time: LockTime,
    inputs: Vec<TxInput>,
    outputs: Vec<TxOutput>,
    miner_fee: Option<u64>,
    return_address: Option<Address>,
    contains_taproot: bool,
}

impl TransactionBuilder {
    pub fn new(network: Network) -> Self {
        TransactionBuilder {
            network,
            // TODO: Check this.
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
    pub fn version(mut self, version: i32) -> Self {
        self.version = version;
        self
    }
    // TODO: handle locktime seconds?.
    pub fn lock_time(mut self, height: u32) -> Self {
        self.lock_time = LockTime::Blocks(Height::from_consensus(height).unwrap());
        self
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
            TxInput::P2TRKeyPath(_) => self.contains_taproot = true,
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
            TxInput::P2PKH(p2pkh) => signer
                .claim_p2pkh(p2pkh, sighash, EcdsaSighashType::All)
                .map(|claim| ClaimLocation::Script(claim.0)),
            TxInput::P2TRKeyPath(p2tr) => signer
                .claim_p2tr_key_path(p2tr, sighash, TapSighashType::All)
                .map(|claim| ClaimLocation::Witness(claim.0)),
            TxInput::NonStandard { ctx: _ } => {
                panic!()
            },
        })
    }
    pub fn sign_inputs_fn<F>(mut self, signer: F) -> Result<TransactionSigned>
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
            total_satoshi_inputs += input.satoshis().unwrap();

            let btxin = TxIn::from(input);
            tx.input.push(btxin);
        }

        // Prepare the outputs for `bitcoin` crate.
        let mut total_satoshis_outputs = 0;
        for output in &self.outputs {
            total_satoshis_outputs += output.satoshis();

            // TODO: Doable without clone?
            let btc_txout = TxOut::from(output.clone());
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
                    value: input.ctx().value.ok_or(Error::Todo)?,
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
                            &p2pkh.ctx.script_pubkey,
                            EcdsaSighashType::All.to_u32(),
                        )
                        .map_err(|_| Error::Todo)?;

                    let message: secp256k1::Message =
                        TransactionHash::from_legacy_sig_hash(hash).into();
                    let updated = signer(input, message)?;

                    claims.push((index, updated));
                },
                TxInput::P2TRKeyPath(_) => {
                    let hash = cache
                        .taproot_key_spend_signature_hash(
                            index,
                            &bitcoin::psbt::Prevouts::All(&prevouts),
                            TapSighashType::All,
                        )
                        .map_err(|_| Error::Todo)?;

                    let message: secp256k1::Message =
                        TransactionHash::from_tapsig_hash(hash).into();
                    let updated = signer(input, message)?;

                    claims.push((index, updated));
                },
                // Skip.
                TxInput::NonStandard { ctx: _ } => continue,
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
    pub value: Option<u64>,
    // The condition for claiming the outputr
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
            // TODO: Track `TxOut` directly?
            value: Some(utxo.value),
            // TODO: Document this.
            script_pubkey: utxo.script_pubkey,
            // Default value of `0xFFFFFFFF = 4294967295`.
            sequence: Sequence::default(),
            // Empty witness.
            witness: Witness::new(),
        }
    }
}

impl From<InputContext> for TxIn {
    fn from(ctx: InputContext) -> Self {
        TxIn {
            previous_output: ctx.previous_output,
            script_sig: ScriptBuf::default(),
            sequence: ctx.sequence,
            witness: ctx.witness,
        }
    }
}