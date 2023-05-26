use bitcoin::blockdata::locktime::absolute::{Height, LockTime};
use bitcoin::blockdata::script::ScriptBuf;
use bitcoin::blockdata::transaction::OutPoint;
use bitcoin::hash_types::Txid;
use bitcoin::hashes::sha256d::Hash;
use bitcoin::key::UntweakedPublicKey;
use bitcoin::psbt::Input;
use bitcoin::taproot::{LeafVersion, TaprootBuilder};
use bitcoin::transaction::Transaction;
use bitcoin::{Sequence, TxIn, TxOut, Witness};
use secp256k1::{generate_keypair, KeyPair, Secp256k1};

pub type Result<T> = std::result::Result<T, Error>;

pub enum Error {
    Todo,
}

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

pub enum ScriptVariant {
    P2pkh(ClaimP2pkhBuilder),
    NonStandard,
}

pub struct TransactionBuilder {
    tx: Transaction,
}

impl TransactionBuilder {
    fn new() -> Self {
        TransactionBuilder {
            tx: Transaction {
                // TODO: Check
                version: 2,
                lock_time: LockTime::Blocks(Height::ZERO),
                input: vec![],
                output: vec![],
            },
        }
    }
    pub fn version(self, version: i32) -> Self {
        Self {
            tx: Transaction { version, ..self.tx },
        }
    }
    // TODO: handle locktime blocks/seconds.
    pub fn lock_time(self, height: u32) -> Self {
        Self {
            tx: Transaction {
                lock_time: LockTime::Blocks(Height::from_consensus(height).unwrap()),
                ..self.tx
            },
        }
    }
    fn add_spendable(self, spend: Spendable) -> Self {
        todo!()
    }
}

pub struct PublicKey;
pub struct PublicKeyHash(bitcoin::PubkeyHash);
pub struct ScriptHash;

fn claim_utxo(utxo: TxOut, point: OutPoint) -> ScriptVariant {
    if utxo.script_pubkey.is_p2pkh() {
        ScriptVariant::P2pkh(ClaimP2pkhBuilder {
            ctx: InputContextBuilder {
                previous_output: point,
                script_pub_key: utxo.script_pubkey,
                sequence: None,
                witness: None,
            },
            script_sig: None,
        })
    } else {
        ScriptVariant::NonStandard
    }
}

struct InputContextBuilder {
    previous_output: OutPoint,
    // The condition for claiming the output.
    script_pub_key: ScriptBuf,
    // TODO: Document this.
    sequence: Option<Sequence>,
    // Witness data for Segwit/Taproot transactions.
    witness: Option<Witness>,
}

// TODO: Should be private.
pub struct InputContext {
    previous_output: OutPoint,
    // The condition for claiming the output.
    script_pub_key: ScriptBuf,
    // TODO: Document this.
    sequence: Sequence,
    // Witness data for Segwit/Taproot transactions.
    witness: Witness,
}

pub struct ClaimP2pkhBuilder {
    ctx: InputContextBuilder,
    // Script for claiming the output.
    script_sig: Option<ScriptBuf>,
}

impl ClaimP2pkhBuilder {
    fn my_pubkey_hash(self, hash: PublicKeyHash) -> Spendable {
        Spendable::P2pkh {
            // TODO: We probably don't need to construct this here, but in the
            // `TransactionBuilder`.
            input: InputContext {
                previous_output: self.ctx.previous_output,
                script_pub_key: self.ctx.script_pub_key,
                // Use `0xFFFFFFFF` default.
                sequence: Sequence::default(),
                // Empty witness.
                witness: Witness::new(),
            },
            // This hash
            hash,
        }
    }
}

pub enum Spendable {
    P2pkh {
        input: InputContext,
        hash: PublicKeyHash,
    },
}
