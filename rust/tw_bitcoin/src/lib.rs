use bitcoin::address::{Address as BTCAddress, Payload as BTCPayload};
use bitcoin::blockdata::locktime::absolute::{Height as BTCHeight, LockTime as BTCLockTime};
use bitcoin::blockdata::script::ScriptBuf as BTCScriptBuf;
use bitcoin::blockdata::transaction::OutPoint as BTCOutPoint;
use bitcoin::consensus::{Decodable, Encodable};
use bitcoin::hash_types::PubkeyHash as BTCPubkeyHash;
use bitcoin::hashes::hash160::Hash as BTCHash;
use bitcoin::hashes::Hash as BTCHashTrait;
use bitcoin::sighash::{LegacySighash as BTCLegacySighash, SighashCache as BTCSighashCache};
use bitcoin::transaction::Transaction as BTCTransaction;
use bitcoin::{Sequence as BTCSequence, TxIn as BTCTxIn, TxOut as BTCTxOut, Witness as BTCWitness};
use claim::TransactionSigner;
use std::str::FromStr;
use tw_hash::H256;
use tw_keypair::ecdsa::secp256k1;
use tw_keypair::traits::KeyPairTrait;

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

pub fn keypair_from_wif(wif: &str) -> Result<secp256k1::KeyPair> {
    secp256k1::KeyPair::from_wif(wif).map_err(|_| Error::Todo)
}

fn convert_legacy_btc_hash_to_h256(hash: BTCLegacySighash) -> H256 {
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

    let script1 = BTCScriptBuf::new();
    let script2 = BTCScriptBuf::new();

    let spend_info = TaprootBuilder::new()
        .add_leaf(1, script1.clone())
        .unwrap()
        .add_leaf(1, script2)
        .unwrap()
        .finalize(&secp, tweaked)
        .unwrap();

    let root = spend_info.merkle_root().unwrap();

    let tapscript = BTCScriptBuf::new_v1_p2tr(&secp, tweaked, Some(root));

    let control_block = spend_info.control_block(&(script1, LeafVersion::TapScript));
}
*/

pub struct ScriptHash;

pub struct TransactionBuilder {
    version: i32,
    lock_time: BTCLockTime,
    inputs: Vec<TxInput>,
    outputs: Vec<TxOutput>,
    btc_tx: Option<BTCTransaction>,
}

impl TransactionBuilder {
    pub fn new() -> Self {
        TransactionBuilder {
            // TODO: Check this.
            version: 2,
            // No lock time, transaction is immediately spendable.
            lock_time: BTCLockTime::Blocks(BTCHeight::ZERO),
            inputs: vec![],
            outputs: vec![],
            btc_tx: None,
        }
    }
    pub fn version(mut self, version: i32) -> Self {
        self.version = version;
        self
    }
    // TODO: handle locktime seconds?.
    pub fn lock_time(mut self, height: u32) -> Self {
        self.lock_time = BTCLockTime::Blocks(BTCHeight::from_consensus(height).unwrap());
        self
    }
    pub fn add_input(mut self, input: TxInput) -> Self {
        self.inputs.push(input);
        self
    }
    pub fn add_output(mut self, output: TxOutput) -> Self {
        self.outputs.push(output);
        self
    }
    pub fn prepare_for_signing(mut self) -> Self {
        // Prepare boilerplate transaction for `bitcoin` crate.
        let mut tx = BTCTransaction {
            version: self.version,
            lock_time: self.lock_time,
            input: vec![],
            output: vec![],
        };

        // Prepare the inputs for `bitcoin` crate.
        for input in self.inputs.iter().cloned() {
            let btc_txin = match input {
                // TODO: `BTCTxIn` should implement `From<TxInput>`.
                TxInput::P2PKH(p2pkh) => BTCTxIn::from(p2pkh.ctx),
                TxInput::NonStandard { ctx } => BTCTxIn::from(ctx),
            };

            tx.input.push(btc_txin);
        }

        // Prepare the outputs for `bitcoin` crate.
        for output in &self.outputs {
            // TODO: Doable without clone?
            let btc_txout = BTCTxOut::from(output.clone());
            tx.output.push(btc_txout);
        }

        self.btc_tx = Some(tx);
        self
    }
    pub fn sign_inputs<S>(self, signer: S) -> Result<Self>
    where
        S: TransactionSigner,
    {
        self.sign_inputs_fn(|input, sighash| match input {
            TxInput::P2PKH(p2pkh) => signer
                .claim_p2pkh(p2pkh, sighash)
                // TODO: Should not convert into BTCScriptBuf here.
                .map(|claim| claim.into_script_buf()),
            TxInput::NonStandard { ctx: _ } => {
                panic!()
            },
        })
    }
    // TODO: Does this have to return `Result<T>`?
    pub fn sign_inputs_fn<F>(mut self, signer: F) -> Result<Self>
    where
        F: Fn(&TxInput, H256) -> Result<BTCScriptBuf>,
    {
        let cache = BTCSighashCache::new(self.btc_tx.unwrap());

        let mut updated_scriptsigs = vec![];

        // For each input (index), we create a hash which is to be signed.
        for (index, input) in self.inputs.iter().enumerate() {
            match input {
                TxInput::P2PKH(p2pkh) => {
                    let legacy_hash = cache
                        .legacy_signature_hash(
                            index,
                            // TODO: Add note that this is same as `scriptPubKey`,
                            // handled somewhere else.
                            &p2pkh.ctx.script_pubkey,
                            // TODO: Make adjustable.
                            SIGHASH_ALL,
                        )
                        .map_err(|_| Error::Todo)?;

                    let h256 = convert_legacy_btc_hash_to_h256(legacy_hash);
                    // TODO: Rename closure var.
                    let updated = signer(input, h256)?;

                    updated_scriptsigs.push((index, updated));
                },
                // Skip.
                TxInput::NonStandard { ctx: _ } => continue,
            };
        }

        let mut tx = cache.into_transaction();

        // Update the transaction with the updated scriptSig's.
        for (index, script_sig) in updated_scriptsigs {
            tx.input[index].script_sig = script_sig;
        }

        self.btc_tx = Some(tx);

        // TODO: Return new type.
        Ok(self)
    }
    pub fn serialize(&self) -> Result<Vec<u8>> {
        let mut buffer = vec![];
        self.btc_tx
            .as_ref()
            .unwrap()
            .consensus_encode(&mut buffer)
            .map_err(|_| Error::Todo)?;
        Ok(buffer)
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

#[derive(Debug, Clone)]
// TODO: Should be private.
pub struct InputContext {
    pub previous_output: BTCOutPoint,
    // The condition for claiming the output.
    pub script_pubkey: BTCScriptBuf,
    // TODO: Document this.
    pub sequence: BTCSequence,
    // BTCWitness data for Segwit/Taproot transactions.
    pub witness: BTCWitness,
}

impl InputContext {
    pub fn new(utxo: BTCTxOut, point: BTCOutPoint) -> Self {
        InputContext {
            previous_output: point,
            // TODO: Document this.
            script_pubkey: utxo.script_pubkey,
            // Default value of `0xFFFFFFFF = 4294967295`.
            sequence: BTCSequence::default(),
            // Empty witness.
            witness: BTCWitness::new(),
        }
    }
    pub fn from_slice(mut slice: &[u8]) -> Result<Self> {
        Ok(InputContext {
            previous_output: Decodable::consensus_decode_from_finite_reader(&mut slice)
                .map_err(|_| Error::Todo)?,
            script_pubkey: Decodable::consensus_decode_from_finite_reader(&mut slice)
                .map_err(|_| Error::Todo)?,
            sequence: Decodable::consensus_decode_from_finite_reader(&mut slice)
                .map_err(|_| Error::Todo)?,
            witness: BTCWitness::default(),
        })
    }
}

impl From<InputContext> for BTCTxIn {
    fn from(ctx: InputContext) -> Self {
        BTCTxIn {
            previous_output: ctx.previous_output,
            // TODO: Document this.
            script_sig: BTCScriptBuf::default(),
            sequence: ctx.sequence,
            witness: ctx.witness,
        }
    }
}

#[derive(Debug, Clone)]
pub enum TxOutput {
    P2PKH(TxOutputP2PKH),
}

impl From<TxOutputP2PKH> for TxOutput {
    fn from(output: TxOutputP2PKH) -> Self {
        TxOutput::P2PKH(output)
    }
}

#[derive(Debug, Clone)]
pub struct TxOutputP2PKH {
    satoshis: u64,
    script_pubkey: BTCScriptBuf,
}

impl TxOutputP2PKH {
    pub fn new(satoshis: u64, recipient: &PubkeyHash) -> Self {
        TxOutputP2PKH {
            satoshis,
            script_pubkey: BTCScriptBuf::new_p2pkh(&recipient.0),
        }
    }
}

impl From<TxOutput> for BTCTxOut {
    fn from(out: TxOutput) -> Self {
        match out {
            TxOutput::P2PKH(p2pkh) => BTCTxOut {
                value: p2pkh.satoshis,
                script_pubkey: p2pkh.script_pubkey,
            },
        }
    }
}

#[derive(Debug, Clone)]
pub enum TxInput {
    P2PKH(TxInputP2PKH),
    NonStandard { ctx: InputContext },
}

#[derive(Debug, Clone)]
pub struct TxInputP2PKH {
    pub ctx: InputContext,
    pub recipient: PubkeyHash,
}

#[derive(Debug, Clone, Eq, PartialEq)]
pub struct PubkeyHash(BTCPubkeyHash);

impl PubkeyHash {
    pub fn from_address_str(slice: &str) -> Result<Self> {
        let checked = BTCAddress::from_str(slice).map_err(|_| Error::Todo)?;

        // TODO: Network should be checked.
        let hash = match checked.payload {
            BTCPayload::PubkeyHash(hash) => hash,
            _ => todo!(),
        };

        Ok(PubkeyHash(hash))
    }
    pub fn from_keypair(keypair: &secp256k1::KeyPair, compressed: bool) -> Result<Self> {
        let bhash = if compressed {
            BTCHash::hash(keypair.public().compressed().as_slice())
        } else {
            BTCHash::hash(keypair.public().uncompressed().as_slice())
        };

        let pubkey = BTCPubkeyHash::from_raw_hash(bhash);

        Ok(PubkeyHash(pubkey))
    }
    pub fn from_bytes(bytes: [u8; 20]) -> Result<Self> {
        Ok(PubkeyHash(BTCPubkeyHash::from_byte_array(bytes)))
    }
    pub fn from_script(script: &BTCScriptBuf) -> Result<Self> {
        let pubkey = match BTCPayload::from_script(script).map_err(|_| Error::Todo)? {
            BTCPayload::PubkeyHash(hash) => PubkeyHash(hash),
            _ => return Err(Error::Todo),
        };

        Ok(pubkey)
    }
}

impl TxInput {
    pub fn new_p2pkh() -> Self {
        todo!()
    }
    pub fn from_slice(slice: &[u8]) -> Result<Self> {
        let ctx = InputContext::from_slice(slice)?;
        let recipient = PubkeyHash::from_script(&ctx.script_pubkey)?;

        Ok(TxInput::P2PKH(TxInputP2PKH { ctx, recipient }))
    }
}
