use bitcoin::address::Payload;
use bitcoin::address::{Address as BAddress, Payload as BPayload};
use bitcoin::blockdata::locktime::absolute::{Height as BHeight, LockTime as BLockTime};
use bitcoin::blockdata::script::ScriptBuf as BScriptBuf;
use bitcoin::blockdata::transaction::OutPoint as BOutPoint;
use bitcoin::consensus::{Decodable, Encodable};
use bitcoin::hash_types::PubkeyHash as BPubkeyHash;
use bitcoin::hashes::hash160::Hash as BHash;
use bitcoin::hashes::Hash as BHashTrait;
use bitcoin::key::{
    KeyPair, PrivateKey, TapTweak, TweakedKeyPair, TweakedPublicKey as BTweakedPublicKey,
    UntweakedPublicKey as BUntweakedPublicKey,
};
use bitcoin::opcodes::All as AnyOpcode;
use bitcoin::script::PushBytesBuf as BPushBytesBuf;
use bitcoin::secp256k1::PublicKey;
use bitcoin::secp256k1::{self, XOnlyPublicKey};
use bitcoin::sighash::LegacySighash;
use bitcoin::sighash::{
    LegacySighash as BLegacySighash, SighashCache as BSighashCache, TapSighash,
};
use bitcoin::transaction::Transaction as BTransaction;
use bitcoin::{Sequence as BSequence, TxIn as BTxIn, TxOut as BTxOut, Witness as BWitness};
use claim::{ClaimLocation, TransactionSigner};
use std::str::FromStr;

pub mod claim;

pub fn pubkey_hash_from_address(string: &str) -> Result<PubkeyHash> {
    let addr = Address::from_str(string).map_err(|_| Error::Todo)?;
    match addr.payload {
        Payload::PubkeyHash(hash) => Ok(hash),
        _ => Err(Error::Todo),
    }
}

pub fn keypair_from_wif(string: &str) -> Result<KeyPair> {
    let pk = PrivateKey::from_wif(string).map_err(|_| Error::Todo)?;
    let keypair = KeyPair::from_secret_key(&secp256k1::Secp256k1::new(), &pk.inner);
    Ok(keypair)
}

fn tweak_pubkey(pubkey: PublicKey) -> BTweakedPublicKey {
    let xonly = XOnlyPublicKey::from(pubkey);
    let (tweaked, _) = xonly.tap_tweak(&secp256k1::Secp256k1::new(), None);
    tweaked
}

fn tweak_keypair(keypair: &KeyPair) -> TweakedKeyPair {
    keypair.tap_tweak(&secp256k1::Secp256k1::new(), None)
}

// Reexports
pub use bitcoin;
pub use bitcoin::{Address, PubkeyHash};

pub type Result<T> = std::result::Result<T, Error>;

const SIGHASH_ALL: u32 = 1;

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

    let script1 = BScriptBuf::new();
    let script2 = BScriptBuf::new();

    let spend_info = TaprootBuilder::new()
        .add_leaf(1, script1.clone())
        .unwrap()
        .add_leaf(1, script2)
        .unwrap()
        .finalize(&secp, tweaked)
        .unwrap();

    let root = spend_info.merkle_root().unwrap();

    let tapscript = BScriptBuf::new_v1_p2tr(&secp, tweaked, Some(root));

    let control_block = spend_info.control_block(&(script1, LeafVersion::TapScript));
}
*/

pub struct ScriptHash;

#[derive(Debug, Clone)]
pub struct TransactionBuilder {
    version: i32,
    lock_time: BLockTime,
    inputs: Vec<TxInput>,
    outputs: Vec<TxOutput>,
    contains_taproot: bool,
    btc_tx: Option<BTransaction>,
}

impl Default for TransactionBuilder {
    fn default() -> Self {
        TransactionBuilder {
            // TODO: Check this.
            version: 2,
            // No lock time, transaction is immediately spendable.
            lock_time: BLockTime::Blocks(BHeight::ZERO),
            inputs: vec![],
            outputs: vec![],
            contains_taproot: false,
            btc_tx: None,
        }
    }
}

impl TransactionBuilder {
    pub fn new() -> Self {
        Self::default()
    }
    pub fn version(mut self, version: i32) -> Self {
        self.version = version;
        self
    }
    // TODO: handle locktime seconds?.
    pub fn lock_time(mut self, height: u32) -> Self {
        self.lock_time = BLockTime::Blocks(BHeight::from_consensus(height).unwrap());
        self
    }
    pub fn add_input(mut self, input: TxInput) -> Self {
        match input {
            TxInput::P2TRKeySpend(_) => self.contains_taproot = true,
            _ => {},
        }

        self.inputs.push(input);
        self
    }
    pub fn add_output(mut self, output: TxOutput) -> Self {
        self.outputs.push(output);
        self
    }
    pub fn prepare_for_signing(mut self) -> Self {
        // Prepare boilerplate transaction for `bitcoin` crate.
        let mut tx = BTransaction {
            version: self.version,
            lock_time: self.lock_time,
            input: vec![],
            output: vec![],
        };

        // Prepare the inputs for `bitcoin` crate.
        for input in self.inputs.iter().cloned() {
            let btxin = BTxIn::from(input);
            tx.input.push(btxin);
        }

        // Prepare the outputs for `bitcoin` crate.
        for output in &self.outputs {
            // TODO: Doable without clone?
            let btc_txout = BTxOut::from(output.clone());
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
                .claim_p2pkh(p2pkh, sighash, None)
                // TODO: Should not convert into BScriptBuf here.
                .map(|claim| ClaimLocation::Script(claim.0)),
            TxInput::P2TRKeySpend(p2tr) => signer
                .claim_p2tr_key_spend(p2tr, sighash, None)
                .map(|claim| ClaimLocation::Witness(claim.0)),
            TxInput::NonStandard { ctx: _ } => {
                panic!()
            },
        })
    }
    // TODO: Does this have to return `Result<T>`?
    pub fn sign_inputs_fn<F>(mut self, signer: F) -> Result<Self>
    where
        F: Fn(&TxInput, secp256k1::Message) -> Result<ClaimLocation>,
    {
        // If Taproot is enabled, we prepare the full `BTxOuts` (value and
        // scriptPubKey) for hashing, which will then be signed.
        // TODO: Document some more.
        let mut btxouts = vec![];
        if self.contains_taproot {
            for input in &self.inputs {
                btxouts.push(BTxOut {
                    value: input.ctx().value.ok_or(Error::Todo)?,
                    script_pubkey: input.ctx().script_pubkey.clone(),
                });
            }
        }

        let mut cache = BSighashCache::new(self.btc_tx.unwrap());

        // TODO: Rename.
        let mut updated_scriptsigs = vec![];

        // For each input (index), we create a hash which is to be signed.
        for (index, input) in self.inputs.iter().enumerate() {
            match input {
                TxInput::P2PKH(p2pkh) => {
                    let hash = cache
                        .legacy_signature_hash(
                            index,
                            &p2pkh.ctx.script_pubkey,
                            // TODO: Make adjustable.
                            SIGHASH_ALL,
                        )
                        .map_err(|_| Error::Todo)?;

                    // TODO: Rename closure var.
                    let message: secp256k1::Message =
                        TransactionHash::from_legacy_sig_hash(hash).into();
                    let updated = signer(input, message)?;

                    updated_scriptsigs.push((index, updated));
                },
                TxInput::P2TRKeySpend(_) => {
                    let hash = cache
                        .taproot_key_spend_signature_hash(
                            index,
                            &bitcoin::psbt::Prevouts::All(&btxouts),
                            bitcoin::sighash::TapSighashType::All,
                        )
                        .map_err(|_| Error::Todo)?;

                    let message: secp256k1::Message =
                        TransactionHash::from_tapsig_hash(hash).into();
                    let updated = signer(input, message)?;

                    updated_scriptsigs.push((index, updated));
                },
                // Skip.
                TxInput::NonStandard { ctx: _ } => continue,
            };
        }

        let mut tx = cache.into_transaction();

        // Update the transaction with the updated scriptSig's.
        for (index, claim_loc) in updated_scriptsigs {
            match claim_loc {
                ClaimLocation::Script(script) => {
                    tx.input[index].script_sig = script;
                },
                ClaimLocation::Witness(witness) => {
                    tx.input[index].witness = witness;
                },
            }
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

#[derive(Debug, Clone)]
// TODO: Should be private.
pub struct InputContext {
    pub previous_output: BOutPoint,
    pub value: Option<u64>,
    // The condition for claiming the output.
    pub script_pubkey: BScriptBuf,
    // TODO: Document this.
    pub sequence: BSequence,
    // BWitness data for Segwit/Taproot transactions.
    // TODO: Remove this?
    pub witness: BWitness,
}

impl InputContext {
    pub fn new(utxo: BTxOut, point: BOutPoint) -> Self {
        InputContext {
            previous_output: point,
            // TODO: Track `BTxOut` directly?
            value: Some(utxo.value),
            // TODO: Document this.
            script_pubkey: utxo.script_pubkey,
            // Default value of `0xFFFFFFFF = 4294967295`.
            sequence: BSequence::default(),
            // Empty witness.
            witness: BWitness::new(),
        }
    }
    pub fn from_slice(mut slice: &[u8], value: Option<u64>) -> Result<Self> {
        Ok(InputContext {
            previous_output: Decodable::consensus_decode_from_finite_reader(&mut slice)
                .map_err(|_| Error::Todo)?,
            value,
            script_pubkey: Decodable::consensus_decode_from_finite_reader(&mut slice)
                .map_err(|_| Error::Todo)?,
            sequence: Decodable::consensus_decode_from_finite_reader(&mut slice)
                .map_err(|_| Error::Todo)?,
            witness: BWitness::default(),
        })
    }
}

impl From<InputContext> for BTxIn {
    fn from(ctx: InputContext) -> Self {
        BTxIn {
            previous_output: ctx.previous_output,
            script_sig: BScriptBuf::default(),
            sequence: ctx.sequence,
            witness: ctx.witness,
        }
    }
}

impl From<TxInput> for BTxIn {
    fn from(input: TxInput) -> Self {
        let ctx = input.ctx();

        BTxIn {
            previous_output: ctx.previous_output,
            script_sig: BScriptBuf::default(),
            sequence: ctx.sequence,
            witness: ctx.witness.clone(),
        }
    }
}

#[derive(Debug, Clone)]
pub enum TxOutput {
    P2PKH(TxOutputP2PKH),
    P2TRKeyPath(TxOutputP2TKeyPath),
}

impl From<TxOutputP2PKH> for TxOutput {
    fn from(output: TxOutputP2PKH) -> Self {
        TxOutput::P2PKH(output)
    }
}

#[derive(Debug, Clone)]
pub struct TxOutputP2PKH {
    satoshis: u64,
    script_pubkey: BScriptBuf,
}

impl TxOutputP2PKH {
    pub fn new(satoshis: u64, recipient: &PubkeyHash) -> Self {
        TxOutputP2PKH {
            satoshis,
            script_pubkey: BScriptBuf::new_p2pkh(recipient),
        }
    }
}

#[derive(Debug, Clone)]
pub struct TxOutputP2TKeyPath {
    satoshis: u64,
    script_pubkey: BScriptBuf,
}

impl TxOutputP2TKeyPath {
    pub fn new(satoshis: u64, recipient: PublicKey) -> Self {
        let tweaked = tweak_pubkey(recipient);

        TxOutputP2TKeyPath {
            satoshis,
            script_pubkey: BScriptBuf::new_v1_p2tr_tweaked(tweaked),
        }
    }
}

impl From<TxOutput> for BTxOut {
    fn from(out: TxOutput) -> Self {
        match out {
            TxOutput::P2PKH(p2pkh) => BTxOut {
                value: p2pkh.satoshis,
                script_pubkey: p2pkh.script_pubkey,
            },
            _ => todo!(),
        }
    }
}

#[derive(Debug, Clone)]
pub enum TxInput {
    P2PKH(TxInputP2PKH),
    P2TRKeySpend(TxInputP2TRKeySpend),
    NonStandard { ctx: InputContext },
}

impl TxInput {
    fn ctx(&self) -> &InputContext {
        match self {
            TxInput::P2PKH(t) => &t.ctx,
            TxInput::P2TRKeySpend(t) => &t.ctx,
            TxInput::NonStandard { ctx } => &ctx,
        }
    }
}

#[derive(Debug, Clone)]
pub struct TxInputP2PKH {
    pub ctx: InputContext,
    pub recipient: PubkeyHash,
}

#[derive(Debug, Clone)]
pub struct TxInputP2TRKeySpend {
    pub ctx: InputContext,
    pub recipient: BTweakedPublicKey,
}

impl TxInput {
    pub fn new_p2pkh() -> Self {
        todo!()
    }
    pub fn from_slice(slice: &[u8], value: Option<u64>) -> Result<Self> {
        let ctx = InputContext::from_slice(slice, value)?;
        let recipient = match Payload::from_script(&ctx.script_pubkey).map_err(|_| Error::Todo)? {
            Payload::PubkeyHash(hash) => hash,
            _ => todo!(),
        };

        Ok(TxInput::P2PKH(TxInputP2PKH { ctx, recipient }))
    }
}

fn get_push(size: u32) -> Result<(AnyOpcode, Option<Vec<u8>>)> {
    use bitcoin::opcodes::all::*;

    let ret = match size {
        // OP_PUSHBYTES[0|1|2|...|75]
        0..=75 => (bitcoin::opcodes::All::from(size as u8), None),
        76..=255 => (OP_PUSHDATA1, Some(size.to_le_bytes().to_vec())),
        256..=65535 => (OP_PUSHDATA2, Some(size.to_le_bytes().to_vec())),
        65536..=u32::MAX => (OP_PUSHDATA4, Some(size.to_le_bytes().to_vec())),
        _ => return Err(Error::Todo),
    };

    Ok(ret)
}

fn create_envelope(content_type: &str, data: &str) -> Result<BScriptBuf> {
    use bitcoin::opcodes::all::*;
    use bitcoin::opcodes::*;

    // TODO: Check overflow
    // Prepare content-type buffer.
    let mut content_ty_buf = BPushBytesBuf::new();
    let (op_push, size_buf) = get_push(content_type.len() as u32)?;

    // For any sizes below 75, we use encode as `OP_PUSHBYTES[0|1|2|...|75]`.
    // Fany any sized above 75, we use encode as `OP_PUSHDATA[1|2|3|4] <SIZE_BUF>`.
    content_ty_buf.push(op_push.to_u8()).unwrap();
    if let Some(size_buf) = size_buf {
        content_ty_buf
            .extend_from_slice(size_buf.as_slice())
            .unwrap();
    }

    // Prepare data buffer.
    let mut data_buf = BPushBytesBuf::new();

    let x = BScriptBuf::builder()
        .push_opcode(OP_FALSE)
        .push_opcode(OP_IF)
        // Push three bytes of "orb"
        .push_opcode(OP_PUSHBYTES_3)
        .push_slice(BPushBytesBuf::try_from(b"orb").unwrap())
        // OP_TRUE = OP_1
        .push_opcode(OP_TRUE)
        .push_slice(content_ty_buf)
        .push_opcode(OP_0)
        .push_slice(data_buf)
        .push_opcode(OP_ENDIF);

    todo!()
}
