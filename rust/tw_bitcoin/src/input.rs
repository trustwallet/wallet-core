use crate::{tweak_pubkey, Error, InputContext, Recipient, Result, TaprootScript};
use bitcoin::key::{PublicKey, TweakedPublicKey};
use bitcoin::script::ScriptBuf;
use bitcoin::taproot::{ControlBlock, TapNodeHash, TaprootSpendInfo};
use bitcoin::{OutPoint, PubkeyHash, Sequence, TxIn, Txid, WPubkeyHash, Witness};

#[derive(Debug, Clone)]
pub enum TxInput {
    P2PKH(TxInputP2PKH),
    P2WPKH(TxInputP2WPKH),
    P2TRKeyPath(TxInputP2TRKeyPath),
    P2TRScriptPath(TxInputP2TRScriptPath),
    NonStandard { ctx: InputContext },
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
            // TODO: Comment on this:
            witness: Witness::default(),
        }
    }
}

impl TxInput {
    pub fn ctx(&self) -> &InputContext {
        match self {
            TxInput::P2PKH(t) => &t.ctx,
            TxInput::P2WPKH(t) => &t.ctx,
            TxInput::P2TRKeyPath(t) => &t.ctx,
            TxInput::P2TRScriptPath(t) => &t.ctx,
            TxInput::NonStandard { ctx } => ctx,
        }
    }
    pub fn satoshis(&self) -> Option<u64> {
        match self {
            TxInput::P2PKH(t) => t.ctx.value,
            TxInput::P2WPKH(t) => t.ctx.value,
            TxInput::P2TRKeyPath(t) => t.ctx.value,
            TxInput::P2TRScriptPath(t) => t.ctx.value,
            TxInput::NonStandard { ctx } => ctx.value,
        }
    }
}

#[derive(Debug, Clone)]
pub struct TxInputP2PKH {
    pub(crate) ctx: InputContext,
    pub(crate) recipient: Recipient<PubkeyHash>,
}

impl TxInputP2PKH {
    pub fn new(
        txid: Txid,
        vout: u32,
        recipient: impl Into<Recipient<PubkeyHash>>,
        satoshis: Option<u64>,
    ) -> Self {
        let recipient: Recipient<PubkeyHash> = recipient.into();

        TxInputP2PKH {
            ctx: InputContext {
                previous_output: OutPoint { txid, vout },
                value: satoshis,
                script_pubkey: ScriptBuf::new_p2pkh(recipient.pubkey_hash()),
                sequence: Sequence::default(),
                witness: Witness::new(),
            },
            recipient,
        }
    }
    pub fn builder() -> TxInputP2PKHBuilder {
        TxInputP2PKHBuilder::new()
    }
}

#[derive(Debug, Clone, Default)]
pub struct TxInputP2PKHBuilder {
    txid: Option<Txid>,
    vout: Option<u32>,
    recipient: Option<Recipient<PubkeyHash>>,
    satoshis: Option<u64>,
}

impl TxInputP2PKHBuilder {
    pub fn new() -> TxInputP2PKHBuilder {
        TxInputP2PKHBuilder {
            txid: None,
            vout: None,
            recipient: None,
            satoshis: None,
        }
    }
    pub fn txid(mut self, txid: Txid) -> TxInputP2PKHBuilder {
        self.txid = Some(txid);
        self
    }
    pub fn vout(mut self, vout: u32) -> TxInputP2PKHBuilder {
        self.vout = Some(vout);
        self
    }
    pub fn recipient(mut self, recipient: Recipient<PubkeyHash>) -> TxInputP2PKHBuilder {
        self.recipient = Some(recipient);
        self
    }
    pub fn satoshis(mut self, satoshis: u64) -> TxInputP2PKHBuilder {
        self.satoshis = Some(satoshis);
        self
    }
    pub fn build(self) -> Result<TxInputP2PKH> {
        Ok(TxInputP2PKH::new(
            self.txid.ok_or(Error::Todo)?,
            self.vout.ok_or(Error::Todo)?,
            self.recipient.ok_or(Error::Todo)?,
            self.satoshis,
        ))
    }
}

#[derive(Debug, Clone)]
pub struct TxInputP2TRKeyPath {
    pub(crate) ctx: InputContext,
    pub(crate) recipient: Recipient<TweakedPublicKey>,
}

impl TxInputP2TRKeyPath {
    pub fn new(
        txid: Txid,
        vout: u32,
        recipient: impl Into<Recipient<TweakedPublicKey>>,
        satoshis: u64,
    ) -> Self {
        let recipient: Recipient<TweakedPublicKey> = recipient.into();

        TxInputP2TRKeyPath {
            ctx: InputContext {
                previous_output: OutPoint { txid, vout },
                value: Some(satoshis),
                script_pubkey: ScriptBuf::new_v1_p2tr_tweaked(recipient.t),
                sequence: Sequence::default(),
                witness: Witness::default(),
            },
            recipient,
        }
    }
    pub fn builder() -> TxInputP2TRKeyPathBuilder {
        TxInputP2TRKeyPathBuilder::new()
    }
}

#[derive(Debug, Clone, Default)]
pub struct TxInputP2TRKeyPathBuilder {
    txid: Option<Txid>,
    vout: Option<u32>,
    recipient: Option<Recipient<TweakedPublicKey>>,
    satoshis: Option<u64>,
}

impl TxInputP2TRKeyPathBuilder {
    pub fn new() -> TxInputP2TRKeyPathBuilder {
        Self::default()
    }
    pub fn txid(mut self, txid: Txid) -> TxInputP2TRKeyPathBuilder {
        self.txid = Some(txid);
        self
    }
    pub fn vout(mut self, vout: u32) -> TxInputP2TRKeyPathBuilder {
        self.vout = Some(vout);
        self
    }
    pub fn recipient(
        mut self,
        recipient: Recipient<TweakedPublicKey>,
    ) -> TxInputP2TRKeyPathBuilder {
        self.recipient = Some(recipient);
        self
    }
    pub fn satoshis(mut self, satoshis: u64) -> TxInputP2TRKeyPathBuilder {
        self.satoshis = Some(satoshis);
        self
    }
    pub fn build(self) -> Result<TxInputP2TRKeyPath> {
        Ok(TxInputP2TRKeyPath::new(
            self.txid.ok_or(Error::Todo)?,
            self.vout.ok_or(Error::Todo)?,
            self.recipient.ok_or(Error::Todo)?,
            self.satoshis.ok_or(Error::Todo)?,
        ))
    }
}

#[derive(Debug, Clone)]
pub struct TxInputP2TRScriptPath {
    pub(crate) ctx: InputContext,
    pub(crate) recipient: Recipient<TaprootScript>,
    pub(crate) script: ScriptBuf,
    pub(crate) spend_info: TaprootSpendInfo,
}

impl TxInputP2TRScriptPath {
    pub fn new(
        txid: Txid,
        vout: u32,
        recipient: impl Into<Recipient<TaprootScript>>,
        satoshis: u64,
        script: ScriptBuf,
        spend_info: TaprootSpendInfo,
    ) -> Self {
        let recipient: Recipient<TaprootScript> = recipient.into();

        TxInputP2TRScriptPath {
            ctx: InputContext {
                previous_output: OutPoint { txid, vout },
                value: Some(satoshis),
                script_pubkey: ScriptBuf::new_v1_p2tr(
                    &secp256k1::Secp256k1::new(),
                    recipient.untweaked_pubkey(),
                    Some(recipient.merkle_root()),
                ),
                sequence: Sequence::default(),
                witness: Witness::new(),
            },
            recipient,
            script,
            spend_info,
        }
    }
    pub fn builder() -> TxInputP2TRScriptPathBuilder {
        TxInputP2TRScriptPathBuilder::new()
    }
}

#[derive(Debug, Clone, Default)]
pub struct TxInputP2TRScriptPathBuilder {
    txid: Option<Txid>,
    vout: Option<u32>,
    recipient: Option<Recipient<TaprootScript>>,
    satoshis: Option<u64>,
    script: Option<ScriptBuf>,
    spend_info: Option<TaprootSpendInfo>,
}

impl TxInputP2TRScriptPathBuilder {
    pub fn new() -> TxInputP2TRScriptPathBuilder {
        TxInputP2TRScriptPathBuilder {
            txid: None,
            vout: None,
            recipient: None,
            satoshis: None,
            script: None,
            spend_info: None,
        }
    }
    pub fn txid(mut self, txid: Txid) -> TxInputP2TRScriptPathBuilder {
        self.txid = Some(txid);
        self
    }
    pub fn vout(mut self, vout: u32) -> TxInputP2TRScriptPathBuilder {
        self.vout = Some(vout);
        self
    }
    pub fn recipient(
        mut self,
        recipient: Recipient<TaprootScript>,
    ) -> TxInputP2TRScriptPathBuilder {
        self.recipient = Some(recipient);
        self
    }
    pub fn satoshis(mut self, satoshis: u64) -> TxInputP2TRScriptPathBuilder {
        self.satoshis = Some(satoshis);
        self
    }
    pub fn script(mut self, script: ScriptBuf) -> TxInputP2TRScriptPathBuilder {
        self.script = Some(script);
        self
    }
    pub fn spend_info(mut self, spend_info: TaprootSpendInfo) -> TxInputP2TRScriptPathBuilder {
        self.spend_info = Some(spend_info);
        self
    }
    pub fn build(self) -> Result<TxInputP2TRScriptPath> {
        Ok(TxInputP2TRScriptPath::new(
            self.txid.ok_or(Error::Todo)?,
            self.vout.ok_or(Error::Todo)?,
            self.recipient.ok_or(Error::Todo)?,
            self.satoshis.ok_or(Error::Todo)?,
            self.script.ok_or(Error::Todo)?,
            self.spend_info.ok_or(Error::Todo)?,
        ))
    }
}

#[derive(Debug, Clone)]
pub struct TxInputP2WPKH {
    pub(crate) ctx: InputContext,
    pub(crate) recipient: Recipient<WPubkeyHash>,
}

impl TxInputP2WPKH {
    pub fn new(
        txid: Txid,
        vout: u32,
        recipient: impl Into<Recipient<WPubkeyHash>>,
        satoshis: Option<u64>,
    ) -> Self {
        let recipient: Recipient<WPubkeyHash> = recipient.into();

        TxInputP2WPKH {
            ctx: InputContext {
                previous_output: OutPoint { txid, vout },
                value: satoshis,
                script_pubkey: ScriptBuf::new_v0_p2wpkh(recipient.wpubkey_hash()),
                sequence: Sequence::default(),
                witness: Witness::new(),
            },
            recipient,
        }
    }
    pub fn builder() -> TxInputP2WPKHBuilder {
        TxInputP2WPKHBuilder::new()
    }
}

#[derive(Debug, Clone, Default)]
pub struct TxInputP2WPKHBuilder {
    txid: Option<Txid>,
    vout: Option<u32>,
    recipient: Option<Recipient<WPubkeyHash>>,
    satoshis: Option<u64>,
}

impl TxInputP2WPKHBuilder {
    pub fn new() -> TxInputP2WPKHBuilder {
        Self::default()
    }
    pub fn txid(mut self, txid: Txid) -> TxInputP2WPKHBuilder {
        self.txid = Some(txid);
        self
    }
    pub fn vout(mut self, vout: u32) -> TxInputP2WPKHBuilder {
        self.vout = Some(vout);
        self
    }
    pub fn recipient(mut self, recipient: Recipient<WPubkeyHash>) -> TxInputP2WPKHBuilder {
        self.recipient = Some(recipient);
        self
    }
    pub fn satoshis(mut self, satoshis: u64) -> TxInputP2WPKHBuilder {
        self.satoshis = Some(satoshis);
        self
    }
    pub fn build(self) -> Result<TxInputP2WPKH> {
        Ok(TxInputP2WPKH::new(
            self.txid.ok_or(Error::Todo)?,
            self.vout.ok_or(Error::Todo)?,
            self.recipient.ok_or(Error::Todo)?,
            self.satoshis,
        ))
    }
}
