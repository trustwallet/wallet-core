use bitcoin::{OutPoint, ScriptBuf, Sequence, TxIn, TxOut, Witness};

mod p2pkh;
mod p2tr_key_path;
mod p2tr_script_path;
mod p2wpkh;

pub use p2pkh::*;
pub use p2tr_key_path::*;
pub use p2tr_script_path::*;
pub use p2wpkh::*;

#[derive(Debug, Clone)]
pub struct InputContext {
    pub previous_output: OutPoint,
    pub value: u64,
    // The condition for claiming the output.
    pub script_pubkey: ScriptBuf,
    pub sequence: Sequence,
    // Witness data for Segwit/Taproot transactions.
}

impl InputContext {
    pub fn new(utxo: TxOut, point: OutPoint) -> Self {
        InputContext {
            previous_output: point,
            value: utxo.value,
            script_pubkey: utxo.script_pubkey,
            // Default value of `0xFFFFFFFF = 4294967295`.
            sequence: Sequence::default(),
        }
    }
}

#[derive(Debug, Clone)]
pub enum TxInput {
    P2PKH(TxInputP2PKH),
    P2WPKH(TxInputP2WPKH),
    P2TRKeyPath(TxInputP2TRKeyPath),
    P2TRScriptPath(TxInputP2TRScriptPath),
}

impl From<TxInputP2PKH> for TxInput {
    fn from(input: TxInputP2PKH) -> Self {
        TxInput::P2PKH(input)
    }
}

impl From<TxInputP2WPKH> for TxInput {
    fn from(input: TxInputP2WPKH) -> Self {
        TxInput::P2WPKH(input)
    }
}

impl From<TxInputP2TRKeyPath> for TxInput {
    fn from(input: TxInputP2TRKeyPath) -> Self {
        TxInput::P2TRKeyPath(input)
    }
}

impl From<TxInputP2TRScriptPath> for TxInput {
    fn from(input: TxInputP2TRScriptPath) -> Self {
        TxInput::P2TRScriptPath(input)
    }
}

impl From<TxInput> for TxIn {
    fn from(input: TxInput) -> Self {
        let ctx = input.ctx();

        TxIn {
            previous_output: ctx.previous_output,
            script_sig: ScriptBuf::new(),
            sequence: ctx.sequence,
            witness: Witness::default(),
        }
    }
}

impl TxInput {
    pub fn ctx(&self) -> &InputContext {
        match self {
            TxInput::P2PKH(t) => t.ctx(),
            TxInput::P2WPKH(t) => t.ctx(),
            TxInput::P2TRKeyPath(t) => t.ctx(),
            TxInput::P2TRScriptPath(t) => t.ctx(),
        }
    }
    pub fn satoshis(&self) -> u64 {
        self.ctx().value
    }
}
