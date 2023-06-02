use crate::{tweak_pubkey, Error, InputContext, Result};
use bitcoin::address::Payload;
use bitcoin::key::{
    PublicKey, TapTweak, TweakedPublicKey as BTweakedPublicKey, UntweakedPublicKey,
};
use bitcoin::script::ScriptBuf;
use bitcoin::secp256k1::{self, XOnlyPublicKey};
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
    // TODO: Cleanup.
    pub fn from_slice(slice: &[u8], value: Option<u64>) -> Result<Self> {
        let ctx = InputContext::from_slice(slice, value)?;

        if ctx.script_pubkey.is_p2pkh() {
            let recipient =
                match Payload::from_script(&ctx.script_pubkey).map_err(|_| Error::Todo)? {
                    Payload::PubkeyHash(hash) => hash,
                    // Never panics given that `is_p2pkh` passed.
                    _ => panic!(),
                };

            Ok(TxInput::P2PKH(TxInputP2PKH { ctx, recipient }))
        } else if ctx.script_pubkey.is_v1_p2tr() {
            // Skip the first byte, which is the version indicator.
            let raw = &ctx.script_pubkey.as_bytes()[1..];
            // Assume untweaked, tweak manually.
            let untweaked: UntweakedPublicKey =
                XOnlyPublicKey::from_slice(raw).map_err(|_| Error::Todo)?;

            let (recipient, _) = untweaked.tap_tweak(&secp256k1::Secp256k1::new(), None);

            Ok(TxInput::P2TRKeyPath(TxInputP2TRKeyPath { ctx, recipient }))
        } else {
            Err(Error::Todo)
        }
    }
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
    pub ctx: InputContext,
    pub recipient: PubkeyHash,
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
    // TODO: Needed?
    pub fn new_with_ctx(ctx: InputContext, recipient: PubkeyHash) -> Self {
        TxInputP2PKH { ctx, recipient }
    }
}

#[derive(Debug, Clone)]
pub struct TxInputP2TRKeyPath {
    pub ctx: InputContext,
    pub recipient: BTweakedPublicKey,
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
