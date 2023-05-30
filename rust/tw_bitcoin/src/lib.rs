use bitcoin::blockdata::locktime::absolute::{Height, LockTime};
use bitcoin::blockdata::script::ScriptBuf;
use bitcoin::blockdata::transaction::OutPoint;
use bitcoin::sighash::{LegacySighash, SighashCache};
use bitcoin::transaction::Transaction;
use bitcoin::{Sequence, TxIn, TxOut, Witness};
use claim::TransactionSigner;
//use secp256k1::{generate_keypair, KeyPair, Secp256k1};
use tw_hash::H256;

pub mod claim;

pub type Result<T> = std::result::Result<T, Error>;

const SIGHASH_ALL: u32 = 1;

pub enum SigHashType {
    All,
    None,
    Single,
    AnyoneCanPay,
}

impl SigHashType {
    fn to_le_byte(&self) -> u8 {
        match self {
            SigHashType::All => 1_u8.to_le(),
            SigHashType::None => 2_u8.to_le(),
            SigHashType::Single => 3_u8.to_le(),
            // 0x80 = 128
            SigHashType::AnyoneCanPay => 128_u8.to_le(),
        }
    }
}

fn convert_legacy_btc_hash_to_h256(hash: LegacySighash) -> H256 {
    let slice: &[u8] = hash.as_raw_hash().as_ref();
    debug_assert_eq!(slice.len(), 32);
    let bytes: [u8; 32] = slice.try_into().unwrap();
    H256::from(bytes)
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

#[derive(Debug, Clone)]
pub struct PublicKeyHash(bitcoin::PubkeyHash);
pub struct ScriptHash;

pub struct TransactionBuilder {
    version: i32,
    lock_time: LockTime,
    inputs: Vec<TxInput>,
    outputs: Vec<TxOutput>,
}

impl TransactionBuilder {
    fn new() -> Self {
        TransactionBuilder {
            // TODO: Check this.
            version: 2,
            // No lock time, transaction is immediately spendable.
            lock_time: LockTime::Blocks(Height::ZERO),
            inputs: vec![],
            outputs: vec![],
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
    fn add_input(mut self, input: TxInput) -> Self {
        self.inputs.push(input);
        self
    }
    fn add_output(mut self, output: TxOutput) -> Self {
        self.outputs.push(output);
        self
    }
    fn finalize<S>(self, signer: S) -> Result<Self>
    where
        S: TransactionSigner,
    {
        self.finalize_fn(|input, sighash| match input {
            TxInput::P2pkh { ctx, hash: _ } => signer
                .claim_p2pkh(&ctx.script_pubkey, sighash)
                // TODO: Should not convert into ScriptBuf here.
                .map(|claim| claim.into_script_buf()),
            TxInput::NonStandard { ctx: _ } => {
                panic!()
            },
        })
    }
    fn finalize_fn<F>(self, fun: F) -> Result<Self>
    where
        F: Fn(&TxInput, H256) -> Result<ScriptBuf>,
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
            let btc_txin = match input {
                // TODO: `TxIn` should implement `From<TxInput>`.
                TxInput::P2pkh { ctx, hash: _ } => TxIn::from(ctx),
                TxInput::NonStandard { ctx } => TxIn::from(ctx),
            };

            tx.input.push(btc_txin);
        }

        // Prepare the outputs for `bitcoin` crate.
        for output in &self.outputs {
            // TODO: Doable without clone?
            let btc_txout = TxOut::from(output.clone());
            tx.output.push(btc_txout);
        }

        let cache = SighashCache::new(tx);

        let mut updated_scriptsigs = vec![];

        // For each input (index), we create a hash which is to be signed.
        for (index, input) in self.inputs.iter().enumerate() {
            match input {
                TxInput::P2pkh { ctx, hash: _ } => {
                    let legacy_hash = cache
                        .legacy_signature_hash(
                            index,
                            // TODO: Add note that this is same as `scriptPubKey`,
                            // handled somewhere else.
                            &ctx.script_pubkey,
                            // TODO: Make adjustable.
                            SIGHASH_ALL,
                        )
                        .map_err(|_| Error::Todo)?;

                    let h256 = convert_legacy_btc_hash_to_h256(legacy_hash);
                    // TODO: Rename closure var.
                    let updated = fun(input, h256)?;

                    updated_scriptsigs.push((index, updated));
                },
                // Skip.
                TxInput::NonStandard { ctx: _ } => continue,
            };
        }

        let mut tx = cache.into_transaction();

        // UPdate the Transaction with the updated scriptSig's.
        for (index, script_sig) in updated_scriptsigs {
            tx.input[index].script_sig = script_sig;
        }

        // TODO: Return new type.
        Ok(self)
    }
}

pub enum TransactionSigHashType {
    Legacy(H256),
}

impl TransactionSigHashType {
    pub fn as_h256(&self) -> &H256 {
        match self {
            TransactionSigHashType::Legacy(h256) => h256,
        }
    }
}

pub enum Recipient {
    LegacyHash(PublicKeyHash),
    LegacyPubkey(()),
    Segwit(()),
    Taproot(()),
}

#[derive(Debug, Clone)]
// TODO: Should be private.
pub struct InputContext {
    previous_output: OutPoint,
    // The condition for claiming the output.
    script_pubkey: ScriptBuf,
    // TODO: Document this.
    sequence: Sequence,
    // Witness data for Segwit/Taproot transactions.
    witness: Witness,
}

impl InputContext {
    pub fn new(utxo: TxOut, point: OutPoint) -> Self {
        InputContext {
            previous_output: point,
            // TODO: Document this.
            script_pubkey: utxo.script_pubkey,
            // Default value of `0xFFFFFFFF`.
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
            // TODO: Document this.
            script_sig: ctx.script_pubkey,
            sequence: ctx.sequence,
            witness: ctx.witness,
        }
    }
}

#[derive(Debug, Clone)]
pub enum TxOutput {
    P2pkh {
        satoshis: u64,
        script_pubkey: ScriptBuf,
    },
}

impl From<TxOutput> for TxOut {
    fn from(out: TxOutput) -> Self {
        match out {
            TxOutput::P2pkh {
                satoshis,
                script_pubkey,
            } => TxOut {
                value: satoshis,
                script_pubkey,
            },
        }
    }
}

#[derive(Debug, Clone)]
pub enum TxInput {
    P2pkh {
        ctx: InputContext,
        // TODO: Needed?
        // TODO: Yes, pass this to the claimer, pubkey hash extraction should
        // happen before.
        hash: PublicKeyHash,
    },
    NonStandard {
        ctx: InputContext,
    },
}

impl TxInput {
    fn from_slice(slice: &[u8]) -> Result<Self> {
        todo!()
    }
}
