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

impl From<ScriptBuf> for ScriptVariant {
	fn from(s: ScriptBuf) -> Self {
        // TODO: Extend to all `ScriptVariant`'s.
        if s.is_p2pk() {
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
        }
	}
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

impl Claimable {
    fn from(tx: TxOut, tx_hash: Hash) -> Self {
		// Determine the script for unlocking output.
		let variant = ScriptVariant::from(tx.script_pubkey);

		// TODO: Depending on the variant, we need different type of data.
		let script_sig = match variant {
			ScriptVariant::P2pkh => {
				todo!()
			}
			_ => todo!()
		};

		// PoC
        let txin = TxIn {
            previous_output: OutPoint {
                txid: Txid::from_raw_hash(tx_hash),
                vout: 0,
            },
			// Provide the necessary data to claim the output.
            script_sig: ScriptBuf::new(),
            // Use default value of `0xFFFFFFFF`.
            sequence: Sequence::default(),
			// Provide the necessary witness to claim the output.
            witness: Witness::new(),
        };

        todo!()
    }
}

struct TxContext<T> {
    tx: T,
    variant: ScriptVariant,
}

pub fn new_p2pk() {}
pub fn new_p2pkh() {}
pub fn new_p2tr(script: ScriptBuf) {}
