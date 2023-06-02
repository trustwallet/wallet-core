use crate::{tweak_pubkey, InputContext};
use bitcoin::key::{PublicKey, TweakedPublicKey as BTweakedPublicKey};
use bitcoin::script::ScriptBuf;
use bitcoin::{OutPoint, PubkeyHash, Sequence, TxIn, Txid, Witness};

#[derive(Debug, Clone)]
pub enum TxInput {
    P2PKH(TxInputP2PKH),
    P2TRKeyPath(TxInputP2TRKeyPath),
    NonStandard { ctx: InputContext },
}

impl From<TxInputP2PKH> for TxInput {
    fn from(input: TxInputP2PKH) -> Self {
        TxInput::P2PKH(input)
    }
}

impl From<TxInputP2TRKeyPath> for TxInput {
    fn from(input: TxInputP2TRKeyPath) -> Self {
        TxInput::P2TRKeyPath(input)
    }
}

impl From<TxInput> for TxIn {
    fn from(input: TxInput) -> Self {
        let ctx = input.ctx();

        TxIn {
            previous_output: ctx.previous_output,
            script_sig: ScriptBuf::default(),
            sequence: ctx.sequence,
            witness: ctx.witness.clone(),
        }
    }
}

impl TxInput {
    pub fn ctx(&self) -> &InputContext {
        match self {
            TxInput::P2PKH(t) => &t.ctx,
            TxInput::P2TRKeyPath(t) => &t.ctx,
            TxInput::NonStandard { ctx } => ctx,
        }
    }
    pub fn satoshis(&self) -> Option<u64> {
        match self {
            TxInput::P2PKH(p) => p.ctx.value,
            TxInput::P2TRKeyPath(p) => p.ctx.value,
            TxInput::NonStandard { ctx } => ctx.value,
        }
    }
}

#[derive(Debug, Clone)]
pub struct TxInputP2PKH {
    pub(crate) ctx: InputContext,
    pub(crate) recipient: PubkeyHash,
}

impl TxInputP2PKH {
    // TODO: `satoshis` should be mandatory.
    pub fn new(txid: Txid, vout: u32, recipient: PubkeyHash, satoshis: Option<u64>) -> Self {
        TxInputP2PKH {
            ctx: InputContext {
                previous_output: OutPoint { txid, vout },
                value: satoshis,
                script_pubkey: ScriptBuf::new_p2pkh(&recipient),
                sequence: Sequence::default(),
                witness: Witness::default(),
            },
            recipient,
        }
    }
}

#[derive(Debug, Clone)]
pub struct TxInputP2TRKeyPath {
    pub(crate) ctx: InputContext,
    pub(crate) recipient: BTweakedPublicKey,
}

impl TxInputP2TRKeyPath {
    pub fn new(txid: Txid, vout: u32, recipient: PublicKey, satoshis: u64) -> Self {
        let tweaked = tweak_pubkey(recipient);

        TxInputP2TRKeyPath {
            ctx: InputContext {
                previous_output: OutPoint { txid, vout },
                value: Some(satoshis),
                script_pubkey: ScriptBuf::new_v1_p2tr_tweaked(tweaked),
                sequence: Sequence::default(),
                witness: Witness::default(),
            },
            recipient: tweaked,
        }
    }
}
