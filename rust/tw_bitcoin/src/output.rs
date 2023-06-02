use crate::{tweak_pubkey, Error, Result};
use bitcoin::address::Payload;
use bitcoin::script::ScriptBuf;
use bitcoin::{Address, PubkeyHash, PublicKey, TxOut};

#[derive(Debug, Clone)]
pub enum TxOutput {
    P2PKH(TxOutputP2PKH),
    P2TRKeyPath(TxOutputP2TKeyPath),
}

impl TxOutput {
    pub fn from_address(address: Address) -> Result<Self> {
        match address.payload {
            Payload::PubkeyHash(hash) => todo!(),
            Payload::ScriptHash(hash) => todo!(),
            Payload::WitnessProgram(program) => todo!(),
            _ => Err(Error::Todo),
        }
    }
    pub fn satoshis(&self) -> u64 {
        match self {
            TxOutput::P2PKH(p) => p.satoshis,
            TxOutput::P2TRKeyPath(p) => p.satoshis,
        }
    }
}

impl From<TxOutputP2PKH> for TxOutput {
    fn from(output: TxOutputP2PKH) -> Self {
        TxOutput::P2PKH(output)
    }
}

impl From<TxOutputP2TKeyPath> for TxOutput {
    fn from(output: TxOutputP2TKeyPath) -> Self {
        TxOutput::P2TRKeyPath(output)
    }
}

impl From<TxOutput> for TxOut {
    fn from(out: TxOutput) -> Self {
        match out {
            TxOutput::P2PKH(p2pkh) => TxOut {
                value: p2pkh.satoshis,
                script_pubkey: p2pkh.script_pubkey,
            },
            TxOutput::P2TRKeyPath(p2trkp) => TxOut {
                value: p2trkp.satoshis,
                script_pubkey: p2trkp.script_pubkey,
            },
            _ => todo!(),
        }
    }
}

#[derive(Debug, Clone)]
pub struct TxOutputP2PKH {
    satoshis: u64,
    script_pubkey: ScriptBuf,
}

impl TxOutputP2PKH {
    pub fn new(satoshis: u64, recipient: &PubkeyHash) -> Self {
        TxOutputP2PKH {
            satoshis,
            script_pubkey: ScriptBuf::new_p2pkh(recipient),
        }
    }
}

#[derive(Debug, Clone)]
pub struct TxOutputP2TKeyPath {
    satoshis: u64,
    script_pubkey: ScriptBuf,
}

impl TxOutputP2TKeyPath {
    pub fn new(satoshis: u64, recipient: PublicKey) -> Self {
        let tweaked = tweak_pubkey(recipient);

        TxOutputP2TKeyPath {
            satoshis,
            script_pubkey: ScriptBuf::new_v1_p2tr_tweaked(tweaked),
        }
    }
}
