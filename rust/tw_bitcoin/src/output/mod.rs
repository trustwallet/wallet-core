mod p2pkh;
mod p2tr_key_path;
mod p2tr_script_path;
mod p2wpkh;

pub use p2pkh::*;
pub use p2tr_key_path::*;
pub use p2tr_script_path::*;
pub use p2wpkh::*;

#[derive(Debug, Clone)]
pub enum TxOutput {
    P2PKH(TxOutputP2PKH),
    P2WPKH(TxOutputP2WPKH),
    P2TRKeyPath(TxOutputP2TRKeyPath),
    P2TRScriptPath(TXOutputP2TRScriptPath),
}

impl TxOutput {
    pub fn satoshis(&self) -> u64 {
        match self {
            TxOutput::P2PKH(p) => p.satoshis,
            TxOutput::P2WPKH(p) => p.satoshis,
            TxOutput::P2TRKeyPath(p) => p.satoshis,
            TxOutput::P2TRScriptPath(p) => p.satoshis,
        }
    }
}

impl From<TxOutputP2PKH> for TxOutput {
    fn from(output: TxOutputP2PKH) -> Self {
        TxOutput::P2PKH(output)
    }
}

impl From<TxOutputP2TRKeyPath> for TxOutput {
    fn from(output: TxOutputP2TRKeyPath) -> Self {
        TxOutput::P2TRKeyPath(output)
    }
}

impl From<TxOutputP2WPKH> for TxOutput {
    fn from(output: TxOutputP2WPKH) -> Self {
        TxOutput::P2WPKH(output)
    }
}

impl From<TXOutputP2TRScriptPath> for TxOutput {
    fn from(output: TXOutputP2TRScriptPath) -> Self {
        TxOutput::P2TRScriptPath(output)
    }
}

impl From<TxOutput> for bitcoin::TxOut {
    fn from(out: TxOutput) -> Self {
        match out {
            TxOutput::P2PKH(p) => Self {
                value: p.satoshis,
                script_pubkey: p.script_pubkey,
            },
            TxOutput::P2WPKH(p) => Self {
                value: p.satoshis,
                script_pubkey: p.script_pubkey,
            },
            TxOutput::P2TRKeyPath(p) => Self {
                value: p.satoshis,
                script_pubkey: p.script_pubkey,
            },
            TxOutput::P2TRScriptPath(p) => Self {
                value: p.satoshis,
                script_pubkey: p.script_pubkey,
            },
        }
    }
}
