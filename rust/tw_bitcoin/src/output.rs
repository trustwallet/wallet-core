use crate::{tweak_pubkey, Error, Recipient, Result, TaprootScript};
use bitcoin::address::Payload;
use bitcoin::key::TweakedPublicKey;
use bitcoin::script::ScriptBuf;
use bitcoin::taproot::TaprootSpendInfo;
use bitcoin::{Address, PubkeyHash, PublicKey, TxOut, WPubkeyHash};

#[derive(Debug, Clone)]
pub enum TxOutput {
    P2PKH(TxOutputP2PKH),
    P2WPKH(TxOutputP2WPKH),
    P2TRKeyPath(TxOutputP2TRKeyPath),
    P2TRScriptPath(TXOutputP2TRScriptPath),
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

impl From<TxOutput> for TxOut {
    fn from(out: TxOutput) -> Self {
        match out {
            TxOutput::P2PKH(p) => TxOut {
                value: p.satoshis,
                script_pubkey: p.script_pubkey,
            },
            TxOutput::P2WPKH(p) => TxOut {
                value: p.satoshis,
                script_pubkey: p.script_pubkey,
            },
            TxOutput::P2TRKeyPath(p) => TxOut {
                value: p.satoshis,
                script_pubkey: p.script_pubkey,
            },
            TxOutput::P2TRScriptPath(p) => TxOut {
                value: p.satoshis,
                script_pubkey: p.script_pubkey,
            },
        }
    }
}

#[derive(Debug, Clone)]
pub struct TxOutputP2PKH {
    satoshis: u64,
    script_pubkey: ScriptBuf,
}

impl TxOutputP2PKH {
    pub fn new(satoshis: u64, recipient: impl Into<Recipient<PubkeyHash>>) -> Self {
        let recipient: Recipient<PubkeyHash> = recipient.into();

        TxOutputP2PKH {
            satoshis,
            script_pubkey: ScriptBuf::new_p2pkh(recipient.pubkey_hash()),
        }
    }
}

#[derive(Debug, Clone)]
pub struct TxOutputP2WPKH {
    satoshis: u64,
    script_pubkey: ScriptBuf,
}

impl TxOutputP2WPKH {
    pub fn new(satoshis: u64, recipient: impl Into<Recipient<WPubkeyHash>>) -> Self {
        let recipient: Recipient<WPubkeyHash> = recipient.into();

        TxOutputP2WPKH {
            satoshis,
            script_pubkey: ScriptBuf::new_v0_p2wpkh(recipient.wpubkey_hash()),
        }
    }
}

#[derive(Debug, Clone)]
pub struct TxOutputP2TRKeyPath {
    satoshis: u64,
    script_pubkey: ScriptBuf,
}

impl TxOutputP2TRKeyPath {
    pub fn new(satoshis: u64, recipient: impl Into<Recipient<TweakedPublicKey>>) -> Self {
        let recipient: Recipient<TweakedPublicKey> = recipient.into();

        TxOutputP2TRKeyPath {
            satoshis,
            script_pubkey: ScriptBuf::new_v1_p2tr_tweaked(recipient.t),
        }
    }
}

#[derive(Debug, Clone)]
pub struct TXOutputP2TRScriptPath {
    satoshis: u64,
    script_pubkey: ScriptBuf,
}

impl TXOutputP2TRScriptPath {
    pub fn new(satoshis: u64, recipient: &Recipient<TaprootScript>) -> Self {
        let script_pubkey = ScriptBuf::new_v1_p2tr(
            &secp256k1::Secp256k1::new(),
            recipient.untweaked_pubkey(),
            Some(recipient.merkle_root()),
        );

        TXOutputP2TRScriptPath {
            satoshis,
            script_pubkey,
        }
    }
}
