use bitcoin::blockdata::locktime::absolute::{Height, LockTime};
use bitcoin::blockdata::script::ScriptBuf;
use bitcoin::blockdata::transaction::OutPoint;
use bitcoin::hash_types::Txid;
use bitcoin::hashes::sha256d::Hash;
use bitcoin::key::UntweakedPublicKey;
use bitcoin::psbt::Input;
use bitcoin::sighash::{LegacySighash, SighashCache};
use bitcoin::taproot::{LeafVersion, TaprootBuilder};
use bitcoin::transaction::Transaction;
use bitcoin::{Sequence, TxIn, TxOut, Witness};
//use secp256k1::{generate_keypair, KeyPair, Secp256k1};
use ripemd::{Digest, Ripemd160};
use tw_hash::H256;
use tw_keypair::ecdsa::secp256k1;
use tw_keypair::traits::{KeyPairTrait, SigningKeyTrait};

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

pub trait TransactionSigner {
    fn sign_p2pkh_with_sighash(
        &self,
        utxo: &TxOut,
        hash: H256,
        sighash: SigHashType,
    ) -> Result<ClaimP2PKH>;

    // P2PKH signer with `SIGHASH_ALL` as default.
    fn sign_p2pkh(&self, utxo: &TxOut, hash: H256) -> Result<ClaimP2PKH> {
        <Self as TransactionSigner>::sign_p2pkh_with_sighash(self, utxo, hash, SigHashType::All)
    }
}

pub struct ClaimP2PKH {
    sig_len: u8,
    sig: Vec<u8>,
    sighash: SigHashType,
    pubkey_len: u8,
    pubkey: Vec<u8>,
}

impl TransactionSigner for secp256k1::KeyPair {
    fn sign_p2pkh_with_sighash(
        &self,
        utxo: &TxOut,
        hash: H256,
        sighash: SigHashType,
    ) -> Result<ClaimP2PKH> {
        // Sanity check; make sure the scriptPubKey is actually a `P2PKH` script.
        let script_pubkey = &utxo.script_pubkey;
        if script_pubkey.is_p2pkh() {
            return Err(Error::Todo);
        }

        // Extract the expected recipient from the scriptPubKey.
        let expected_recipient = &utxo.script_pubkey.as_bytes()[2..23];

        // The expected recipient is a RIPEMD160 hash, so first we check
        // whether it's a hash from the COMPRESSED public key.
        let mut hasher = Ripemd160::new();
        hasher.update(self.public().compressed().as_slice());
        let finalized = hasher.finalize();
        let hashed_pubkey = &finalized[..];

        debug_assert_eq!(expected_recipient.len(), hashed_pubkey.len());

        // If the expected recipient is a RIPEMD160 hash of the COMPRESSED
        // key...
        let pubkey = if expected_recipient == hashed_pubkey {
            self.public().compressed().to_vec()
        }
        // ... if not, then we check whether it is a RIPEMD160 hash of the
        // UNCOMPRESSEd public key.
        else {
            let mut hasher = Ripemd160::new();
            hasher.update(self.public().uncompressed().as_slice());
            let finalized = hasher.finalize();
            let hashed_pubkey = &finalized[..];

            if expected_recipient == hashed_pubkey {
                self.public().uncompressed().to_vec()
            } else {
                // Invalid, wrong signer!
                return Err(Error::Todo);
            }
        };

        // Sign the hash of the transaction data (OP_CHECKSIG).
        let sig = self.private().sign(hash).map_err(|_| Error::Todo)?;
        // Encode signature as DER.
        let der_sig = sig.to_der();

        debug_assert_eq!(der_sig.len(), 70);
        debug_assert!(
            pubkey.len() == secp256k1::PublicKey::COMPRESSED
                || pubkey.len() == secp256k1::PublicKey::UNCOMPRESSED
        );

        Ok(
            // The expected script to claim the P2PKH.
            ClaimP2PKH {
                // Signature length, one byte.
                sig_len: der_sig.len() as u8,
                // Signature, 70-72 bytes and DER-encoded.
                sig: der_sig.as_bytes().to_vec(),
                // SIGHASH type, one byte.
                sighash,
                // Public key length, one byte.
                pubkey_len: pubkey.len() as u8,
                // Public key itself, compressed (33 bytes) or uncompressed (65 bytes).
                pubkey,
            },
        )
    }
}

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
    // TODO: Add something like this later.
    /*
    fn add_input_from_utxo(mut self, utxo: TxOut, point: OutPoint) -> Self {
        match TxInputBuilder::from_utxo(utxo, point) {
            TxInputBuilder::P2pkh(builder) => {
                let input = builder
                    .my_pubkey_hash(self.keypair.public().hash())
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
    */
    /// Alias for `add_output_pk2pkh`.
    fn add_output_transfer(self, recipient: Recipient, satoshis: u64) -> Self {
        self.add_output_p2pkh(recipient, satoshis)
    }
    fn add_output_p2pkh(mut self, recipient: Recipient, satoshis: u64) -> Self {
        match recipient {
            Recipient::LegacyHash(hash) => {
                self.outputs.push(TxOutput::P2pkh {
                    satoshis,
                    script_pubkey: ScriptBuf::new_p2pkh(&hash.0),
                });
            },
            _ => todo!(),
        }

        self
    }
    fn signature_hashes(self) -> Result<Vec<(usize, TransactionSigHashType)>> {
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
        for output in self.outputs {
            let btc_txout = TxOut::from(output);
            tx.output.push(btc_txout);
        }

        let cache = SighashCache::new(tx);

        let mut sig_hashes = vec![];

        // For each input (index), we create a hash which is to be signed.
        for (index, input) in self.inputs.into_iter().enumerate() {
            match input {
                TxInput::P2pkh { ctx, hash: _ } => {
                    let legacy_hash = cache
                        .legacy_signature_hash(
                            index,
                            // TODO: Add note that this is same as `scriptPubKey`,
                            // handled somewhere else.
                            &ctx.script_pub_key,
                            SIGHASH_ALL,
                        )
                        .map_err(|_| Error::Todo)?;

                    let h256 = convert_legacy_btc_hash_to_h256(legacy_hash);

                    sig_hashes.push((index, TransactionSigHashType::Legacy(h256)))
                },
                // Skip.
                TxInput::NonStandard { ctx: _ } => continue,
            };
        }

        Ok(sig_hashes)
    }
}

pub enum TransactionSigHashType {
    Legacy(H256),
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
    // Inputs for `script_pub_key`.
    script_sig: ScriptBuf,
    // The condition for claiming the output.
    script_pub_key: ScriptBuf,
    // TODO: Document this.
    sequence: Sequence,
    // Witness data for Segwit/Taproot transactions.
    witness: Witness,
}

impl From<InputContext> for TxIn {
    fn from(ctx: InputContext) -> Self {
        TxIn {
            previous_output: ctx.previous_output,
            script_sig: ctx.script_sig,
            sequence: ctx.sequence,
            witness: ctx.witness,
        }
    }
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
            ctx: self.ctx,
            hash: self.hash.ok_or(Error::Todo)?,
        })
    }
}

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
