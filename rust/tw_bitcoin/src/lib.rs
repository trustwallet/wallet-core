use bitcoin::blockdata::locktime::absolute::{Height, LockTime};
use bitcoin::blockdata::script::ScriptBuf;
use bitcoin::blockdata::transaction::OutPoint;
use bitcoin::hashes::Hash;
use bitcoin::sighash::{LegacySighash, SighashCache};
use bitcoin::transaction::Transaction;
use bitcoin::{Sequence, TxIn, TxOut, Txid, Witness};
use claim::TransactionSigner;
//use secp256k1::{generate_keypair, KeyPair, Secp256k1};
use bitcoin::consensus::{Decodable, Encodable};
use ripemd::{Ripemd160, Digest};
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

#[derive(Debug, Clone, Eq, PartialEq)]
pub struct RecipientHash160([u8; 20]);
pub struct ScriptHash;

pub struct TransactionBuilder {
    version: i32,
    lock_time: LockTime,
    inputs: Vec<TxInput>,
    outputs: Vec<TxOutput>,
    btc_tx: Option<Transaction>,
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
            btc_tx: None,
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
    pub fn add_input(mut self, input: TxInput) -> Self {
        self.inputs.push(input);
        self
    }
    pub fn add_output(mut self, output: TxOutput) -> Self {
        self.outputs.push(output);
        self
    }
    pub fn sign_inputs<S>(self, signer: S) -> Result<Self>
    where
        S: TransactionSigner,
    {
        self.sign_inputs_fn(|input, sighash| match input {
            TxInput::P2PKH(p2pkh) => signer
                .claim_p2pkh(p2pkh, sighash)
                // TODO: Should not convert into ScriptBuf here.
                .map(|claim| claim.into_script_buf()),
            TxInput::NonStandard { ctx: _ } => {
                panic!()
            },
        })
    }
    pub fn sign_inputs_fn<F>(mut self, signer: F) -> Result<Self>
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
                TxInput::P2PKH(p2pkh) => TxIn::from(p2pkh.ctx),
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
            // Default value of `0xFFFFFFFF = 4294967295`.
            sequence: Sequence::default(),
            // Empty witness.
            witness: Witness::new(),
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
            witness: Witness::default(),
        })
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
    P2PKH(TxInputP2PKH),
    NonStandard { ctx: InputContext },
}

#[derive(Debug, Clone)]
pub struct TxInputP2PKH {
    ctx: InputContext,
    recipient: RecipientHash160,
}

impl TxInput {
    pub fn new_p2pkh() -> Self {
        todo!()
    }
    pub fn from_slice(slice: &[u8]) -> Result<Self> {
        let ctx = InputContext::from_slice(slice)?;

        if ctx.script_pubkey.is_p2pkh() {
            // Extract the expected recipient from the scriptPubKey.
            //
            // script[0] == OP_DUP
            // script[1] == OP_HASH160
            // script[2] == OP_PUSHBYTES_20
            // script[3..23] == <PUBKEY-RIPEMD160>
            // script[23] == OP_EQUALVERIFY
            // script[24] == OP_CHECKSIG
            let mut hash = [0; 20];
            hash.clone_from_slice(&ctx.script_pubkey.as_bytes()[3..23]);

            Ok(TxInput::P2PKH(TxInputP2PKH {
                ctx,
                recipient: RecipientHash160(hash),
            }))
        } else {
            //Ok(TxInput::NonStandard { ctx })
            panic!()
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::str::FromStr;
    use tw_encoding::hex;

    #[test]
    fn temp() {
        let script_pubkey = "76a9143836741d8f4a925483cba7634aa3ed0ddd37c54e88ac";
        let var_int = (script_pubkey.len() / 2) as u8;
        println!(">> {var_int}");
        let hex = hex::encode(var_int.to_le_bytes(), false);
        println!(">> {hex}");
    }

    #[test]
    fn tx_input_p2pkh_from_slice() {
        // One byte VarInt, size of scriptPubKey (25 bytes, hex representation of little endian encoding).
        let var_int = "19";
        // 25 bytes, the scriptPubKey for claiming.
        let script_pubkey = "76a9143836741d8f4a925483cba7634aa3ed0ddd37c54e88ac";
        // Four bytes, using default value
        let sequence = "FFFFFFFF";
        // 32 bytes, hash (ID) of the input transaction.
        let txid = "f342ee6bd3fd94e528103d7a0cc95d4882b8284fed9c727e2a221f9bd34fe466";
        // Four bytes, vout value of zero.
        let vout = "00000000";
        // 20 bytes, RIPEMD160 hash of the recipient.
        let recipient_hash = "3836741d8f4a925483cba7634aa3ed0ddd37c54e";

        // RAW TX INPUT
        let raw_tx_input =
            hex::decode(&format!("{txid}{vout}{var_int}{script_pubkey}{sequence}")).unwrap();

        // Decode the `TxInput` from the slice. We expect a `P2PKH` variant.
        let tx_input = TxInput::from_slice(&raw_tx_input).unwrap();
        if let TxInput::P2PKH(p2pkh) = tx_input {
            // Expected `previous_output`
            assert_eq!(
                p2pkh.ctx.previous_output,
                OutPoint {
                    txid: Txid::from_byte_array(hex::decode(txid).unwrap().try_into().unwrap()),
                    vout: 0,
                }
            );
            // Expected `script_pubkey`
            assert_eq!(
                p2pkh.ctx.script_pubkey,
                ScriptBuf::from_hex(script_pubkey).unwrap()
            );
            // Expected sequence
            assert_eq!(p2pkh.ctx.sequence, Sequence::default());
            // Expected witness
            assert_eq!(p2pkh.ctx.witness, Witness::default());
            // Expected recipient.
            assert_eq!(
                p2pkh.recipient,
                RecipientHash160(hex::decode(recipient_hash).unwrap().try_into().unwrap())
            );
        }

        //println!("{:?}", x);
    }
}
