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

pub enum TxInputBuilder {
    P2pkh(P2pkhInputBuilder),
    NonStandard,
}

impl TxInputBuilder {
    fn from_utxo(utxo: TxOut, point: OutPoint) -> Self {
        if utxo.script_pubkey.is_p2pkh() {
            TxInputBuilder::P2pkh(P2pkhInputBuilder {
                ctx: InputContext::new(utxo, point),
                hash: None,
            })
        } else {
            TxInputBuilder::NonStandard
        }
    }
}

pub struct TransactionBuilder {
    tx: Transaction,
    privkey: PrivateKey,
    pubkey: PublicKey,
    inputs: Vec<TxInput>,
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
    // TODO: handle locktime seconds?.
    pub fn lock_time(mut self, height: u32) -> Self {
        self.tx.lock_time = LockTime::Blocks(Height::from_consensus(height).unwrap());
        self
    }
    fn add_tx_input(self, spend: TxInput) -> Self {
        todo!()
    }
    fn add_tx_input_from_utxo(mut self, utxo: TxOut, point: OutPoint) -> Self {
        match TxInputBuilder::from_utxo(utxo, point) {
            TxInputBuilder::P2pkh(builder) => {
                let input = builder
                    .my_pubkey_hash(self.pubkey.hash())
                    .build()
                    // Panicing implies bug.
                    .unwrap();

                self.inputs.push(input);
            },
            TxInputBuilder::NonStandard => {
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

pub struct P2pkhInputBuilder {
    ctx: InputContext,
    hash: Option<PublicKeyHash>,
}

impl P2pkhInputBuilder {
    pub fn my_pubkey_hash(mut self, hash: PublicKeyHash) -> Self {
        self.hash = Some(hash);
        self
    }
    pub fn build(self) -> Result<TxInput> {
        Ok(TxInput::P2pkh {
            input: self.ctx,
            hash: self.hash.ok_or(Error::Todo)?,
        })
    }
}

pub enum TxInput {
    P2pkh {
        input: InputContext,
        hash: PublicKeyHash,
    },
}
