#[macro_use]
extern crate serde;

use std::str::FromStr;

use crate::claim::{ClaimLocation, TransactionSigner};
use bitcoin::address::NetworkChecked;
use bitcoin::blockdata::locktime::absolute::{Height, LockTime};
use bitcoin::consensus::Encodable;
use bitcoin::hashes::Hash;
use bitcoin::key::{KeyPair, TapTweak, TweakedPublicKey, UntweakedPublicKey};
use bitcoin::script::{PushBytesBuf, ScriptBuf};
use bitcoin::sighash::{EcdsaSighashType, LegacySighash, Prevouts, SighashCache, TapSighash};
use bitcoin::sighash::{SegwitV0Sighash, TapSighashType};
use bitcoin::taproot::TapNodeHash;
use bitcoin::transaction::Transaction;
use bitcoin::{
    network,
    secp256k1::{self, XOnlyPublicKey},
    Network, PublicKey,
};
use bitcoin::{Address, OutPoint, PubkeyHash, Sequence, TxIn, TxOut, WPubkeyHash, Witness};

pub mod brc20;
pub mod claim;
pub mod input;
pub mod ordinals;
pub mod output;
pub mod temp;
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
    pub fn from_segwit_hash(hash: SegwitV0Sighash) -> Self {
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

use tw_encoding::hex;

#[test]
fn some() {
    use bitcoin::*;

    let secp = secp256k1::Secp256k1::new();

    // WIF-formatted private keys (*regtest mode!*)
    let alice_wif = "cNDFvH3TXCjxgWeVc7vbu4Jw5m2Lu8FkQ69Z2XvFUD9D9rGjofN1";
    let bob_wif = "cNt3XNHiJdJpoX5zt3CXY8ncgrCted8bxmFBzcGeTZbBw6jkByWB";

    // Public keys:
    // alice: mvBCW6bHUpxrLD6ProHDSbPiBZHr1QrHE8
    // bob: mpHjC7jP4DApXZe9DbaqM6J8wMrCUoQRo6

    // Construct keypairs.
    let pk = PrivateKey::from_wif(alice_wif).unwrap();
    let alice = KeyPair::from_secret_key(&secp, &pk.inner);

    let pk = PrivateKey::from_wif(bob_wif).unwrap();
    let bob = KeyPair::from_secret_key(&secp, &pk.inner);

    // ## Create input
    let txid =
        Txid::from_str("9a582032f6a50cedaff77d3d5604b33adf8bc31bdaef8de977c2187e395860ac").unwrap();
    let vout = 0;
    let amount_sent = 4_999_000_000;

    // Here, Alice send `amount_sent` to Bob, now Bob wants to claim it.
    let input = TxIn {
        previous_output: OutPoint { txid, vout },
        script_sig: ScriptBuf::new(),
        sequence: Sequence::default(),
        witness: Witness::new(),
    };

    // Bob wants to send the (some) of the amount back to Alice.
    let (recicpient, _) = bob.x_only_public_key();

    // >> For debugging, lets tweak with zeroed merkle-root and display it
    {
        let (tweaked, _) = recicpient.tap_tweak(&secp, None);
        println!("{}", hex::encode(tweaked.serialize(), false));
    }

    // ## Create output for P2TR key-path (zeroed merkle root).
    let output = TxOut {
        // This is `amount_sent` - miner-fee
        value: 4_998_000_000,
        script_pubkey: ScriptBuf::new_v1_p2tr(&secp, recicpient, None),
    };

    // Prepare transaction.
    let tx = Transaction {
        version: 2,
        // No lock
        lock_time: LockTime::Blocks(Height::ZERO),
        input: vec![input],
        output: vec![output],
    };

    // Prepare prevouts, which are part of the signing payload.
    let prevouts = [TxOut {
        value: amount_sent,
        script_pubkey: ScriptBuf::new_v1_p2tr(&secp, recicpient, None),
    }];

    // ## Construct the sighash `TapSighashType::Default`.
    let mut signer = SighashCache::new(tx);
    let sighash = signer
        .taproot_key_spend_signature_hash(0, &Prevouts::All(&prevouts), TapSighashType::Default)
        .unwrap();

    // Bob signs the message.
    let message = secp256k1::Message::from_slice(sighash.as_ref()).unwrap();

    let sig = bitcoin::taproot::Signature {
        sig: secp.sign_schnorr(&message, &bob),
        hash_ty: TapSighashType::Default,
    };

    // ## Update transaction with the witness containing the (serialized) Schnorr signature.
    let mut tx = signer.into_transaction();
    tx.input[0].witness = Witness::from_slice(&[sig.to_vec().as_slice()]);

    // Serialize and encode transaction.
    let mut buffer = vec![];
    tx.consensus_encode(&mut buffer).unwrap();

    let prefix = false;
    let hex = hex::encode(buffer, prefix);

    println!("{hex}");
}

#[derive(Debug, Clone, Eq, PartialEq)]
pub struct Recipient<T> {
    t: T,
}

impl Recipient<TweakedPublicKey> {
    pub fn from_keypair(keypair: &KeyPair) -> Self {
        let pubkey = PublicKey::new(keypair.public_key());
        let tweaked = tweak_pubkey(pubkey);

        Recipient { t: tweaked }
    }
}

impl Recipient<WPubkeyHash> {
    pub fn from_keypair(keypair: &KeyPair) -> Self {
        Recipient {
            // TODO: Check unwrap
            t: PublicKey::new(keypair.public_key()).wpubkey_hash().unwrap(),
        }
    }
    pub fn wpubkey_hash(&self) -> &WPubkeyHash {
        &self.t
    }
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
    pub fn wpubkey_hash(&self) -> WPubkeyHash {
        // The key is always compressed.
        debug_assert!(self.t.compressed);
        self.t.wpubkey_hash().unwrap()
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
            TxInput::P2PKH(p) => signer
                .claim_p2pkh(p, sighash, EcdsaSighashType::All)
                .map(|claim| ClaimLocation::Script(claim.0)),
            TxInput::P2WPKH(p) => signer
                .claim_p2wpkh(p, sighash, EcdsaSighashType::All)
                .map(|claim| ClaimLocation::Witness(claim.0)),
            TxInput::P2TRKeyPath(p) => signer
                .claim_p2tr_key_path(p, sighash, TapSighashType::Default)
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
            dbg!("CONTAINS TAPROOT!");
            for input in &self.inputs {
                prevouts.push(TxOut {
                    value: input.ctx().value.ok_or(Error::Todo)?,
                    script_pubkey: input.ctx().script_pubkey.clone(),
                });
            }
        }

        dbg!(&prevouts);

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
                TxInput::P2WPKH(p2wpkh) => {
                    let hash = cache
                        .segwit_signature_hash(
                            index,
                            // TODO: Handle unwrap?
                            p2wpkh
                                .ctx
                                .script_pubkey
                                .p2wpkh_script_code()
                                .as_ref()
                                .unwrap(),
                            // TODO: Should not be an Option
                            p2wpkh.ctx.value.unwrap(),
                            EcdsaSighashType::All,
                        )
                        .map_err(|_| Error::Todo)?;

                    let message: secp256k1::Message =
                        TransactionHash::from_segwit_hash(hash).into();
                    let updated = signer(input, message)?;

                    claims.push((index, updated));
                },
                TxInput::P2TRKeyPath(p) => {
                    let hash = cache
                        .taproot_key_spend_signature_hash(
                            index,
                            &bitcoin::sighash::Prevouts::All(&prevouts),
                            TapSighashType::Default,
                        )
                        .map_err(|_| Error::Todo)?;

                    let message = secp256k1::Message::from_slice(hash.as_ref()).unwrap();
                    let updated = signer(input, message)?;

                    claims.push((index, updated));
                },
                // Skip.
                TxInput::NonStandard { ctx: _ } => continue,
            };
        }

        //dbg!(&self.inputs);
        dbg!(&self.outputs);

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
