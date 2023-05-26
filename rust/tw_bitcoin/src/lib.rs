use bitcoin::blockdata::locktime::absolute::{Height, LockTime};
use bitcoin::blockdata::script::ScriptBuf;
use bitcoin::blockdata::transaction::OutPoint;
use bitcoin::hash_types::Txid;
use bitcoin::hashes::sha256d::Hash;
use bitcoin::key::UntweakedPublicKey;
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
}

pub struct PublicKey;
pub struct PublicKeyHash;
pub struct ScriptHash;

fn claim_utxo(utxo: TxOut, point: OutPoint) -> ScriptVariant {
    let s = utxo.script_pubkey;

    if s.is_p2pkh() {
        ScriptVariant::P2pkh(ClaimP2pkhBuilder {
            ctx: InputContext {
                previous_output: point,
                script_sig: s,
                sequence: None,
                witness: None,
                value: None,
            },
        })
    } else {
        ScriptVariant::NonStandard
    }
}

struct InputContext {
    previous_output: OutPoint,
    script_sig: ScriptBuf,
    sequence: Option<Sequence>,
    witness: Option<Witness>,
    value: Option<u64>,
}

pub struct ClaimP2pkhBuilder {
    ctx: InputContext,
}

pub struct ClaimP2pkh {
    input: TxIn,
    output: TxOut,
}

impl ClaimP2pkhBuilder {
    fn with_pubkey(self, pubkey: PublicKey) -> ClaimP2pkhBuilder {
        todo!()
    }
    fn with_value(mut self, value: u64) -> ClaimP2pkhBuilder {
        self.ctx.value = Some(value);
        self
    }
    fn finalize(self) -> Result<ClaimP2pkh> {
        todo!()
    }
}
