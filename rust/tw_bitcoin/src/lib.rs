use bitcoin::blockdata::locktime::absolute::{Height, LockTime};
use bitcoin::blockdata::script::ScriptBuf;
use bitcoin::key::UntweakedPublicKey;
use bitcoin::taproot::{LeafVersion, TaprootBuilder};
use bitcoin::transaction::Transaction;
use bitcoin::{TxIn, TxOut};
use secp256k1::{generate_keypair, KeyPair, Secp256k1};

pub mod transaction;

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

pub struct PublicKey();
pub struct Hash();

pub enum ScriptVariant {
    P2pk,
    P2pkh,
    P2sh,
    P2shP2wpkh,
    P2shP2wsh,
    P2wpkh,
    P2wsh,
    P2trKeyPath,
    P2trScriptPath,
    NonStandard,
}

pub struct TransactionBuilder {
    tx: Transaction,
}

impl TransactionBuilder {
    fn new() -> Self {
        TransactionBuilder {
            tx: Transaction {
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
    pub fn add_claimable(mut self, claim: Claimable) -> Self {
        self.tx.input.push(claim.input.tx);
        self.tx.output.push(claim.output.tx);
        self
    }
}

// TODO: Rename to "Spendable"?
pub struct Claimable {
    input: TxContext<TxIn>,
	claim_script: (),
    output: TxContext<TxOut>,
}

struct TxContext<T> {
    tx: T,
    variant: ScriptVariant,
}

impl From<TxOut> for Claimable {
    fn from(tx: TxOut) -> Self {
        let s = tx.script_pubkey;
        // TODO: Extend to all `ScriptVariant`'s.
        let variant = if s.is_p2pk() {
            ScriptVariant::P2pk
        } else if s.is_p2pkh() {
            ScriptVariant::P2pkh
        } else if s.is_p2sh() {
            ScriptVariant::P2sh
        } else if s.is_v0_p2wpkh() {
            ScriptVariant::P2wpkh
        } else if s.is_v0_p2wsh() {
            ScriptVariant::P2wsh
        } else if s.is_v1_p2tr() {
            // TODO:
            ScriptVariant::P2trScriptPath
        } else {
            ScriptVariant::NonStandard
        };

        todo!()
    }
}

pub fn new_p2pk(pubkey: PublicKey) {}
pub fn new_p2pkh(hash: Hash) {}
pub fn new_p2tr(script: ScriptBuf) {}
