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

#[derive(Debug, Clone)]
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
    privkey: PrivateKey,
    pubkey: PublicKey,
    inputs: Vec<Spendable>,
}

impl TransactionBuilder {
    fn new(privkey: PrivateKey, pubkey: PublicKey) -> Self {
        TransactionBuilder {
            tx: Transaction {
                // TODO: Check
                version: 2,
                lock_time: LockTime::Blocks(Height::ZERO),
                input: vec![],
                output: vec![],
            },
            privkey,
            pubkey,
            inputs: vec![],
        }
    }
    pub fn version(mut self, version: i32) -> Self {
        self.tx.version = version;
        self
    }
    // TODO: handle locktime blocks/seconds.
    pub fn lock_time(mut self, height: u32) -> Self {
        self.tx.lock_time = LockTime::Blocks(Height::from_consensus(height).unwrap());
        self
    }
    fn add_spendable(self, spend: Spendable) -> Self {
        todo!()
    }
    fn add_input(mut self, utxo: TxOut, point: OutPoint) -> Self {
        match claim_utxo(utxo, point) {
            ScriptVariant::P2pkh(builder) => {
                let input = builder
                    .my_pubkey_hash(self.pubkey.hash())
                    .build()
                    // Panicing implies bug.
                    .unwrap();

                self.inputs.push(input);
            },
            ScriptVariant::NonStandard => {
                todo!()
            },
        }

        self
    }
}

pub struct PrivateKey;
pub struct PublicKey;

impl PublicKey {
    pub fn hash(&self) -> PublicKeyHash {
        todo!()
    }
}

pub struct PublicKeyHash(bitcoin::PubkeyHash);
pub struct ScriptHash;

fn claim_utxo(utxo: TxOut, point: OutPoint) -> ScriptVariant {
    if utxo.script_pubkey.is_p2pkh() {
        ScriptVariant::P2pkh(ClaimP2pkhBuilder {
            ctx: InputContext::new(utxo, point),
            hash: None,
        })
    } else {
        ScriptVariant::NonStandard
    }
}

// TODO: Should be private.
pub struct InputContext {
    previous_output: OutPoint,
    // Inputs for `script_pub_key`.
    script_sig: ScriptBuf,
    // The condition for claiming the output.
    script_pub_key: ScriptBuf,
    // TODO: Document this.
    sequence: Sequence,
    // Witness data for Segwit/Taproot transactions.
    witness: Witness,
}

impl InputContext {
    pub fn new(utxo: TxOut, point: OutPoint) -> Self {
        InputContext {
            previous_output: point,
            // Empty scriptbuf.
            script_sig: ScriptBuf::new(),
            // TODO: Document this.
            script_pub_key: utxo.script_pubkey,
            // Default value of `0xFFFFFFFF`.
            sequence: Sequence::default(),
            // Empty witness.
            witness: Witness::new(),
        }
    }
}

pub struct ClaimP2pkhBuilder {
    ctx: InputContext,
    hash: Option<PublicKeyHash>,
}

impl ClaimP2pkhBuilder {
    pub fn my_pubkey_hash(mut self, hash: PublicKeyHash) -> Self {
        self.hash = Some(hash);
        self
    }
    pub fn build(self) -> Result<Spendable> {
        Ok(Spendable::P2pkh {
            input: self.ctx,
            hash: self.hash.ok_or(Error::Todo)?,
        })
    }
}

pub enum Spendable {
    P2pkh {
        input: InputContext,
        hash: PublicKeyHash,
    },
}
