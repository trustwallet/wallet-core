use crate::{Error, InputContext, Recipient, Result};
use bitcoin::key::TweakedPublicKey;
use bitcoin::{OutPoint, ScriptBuf, Sequence, Txid};

#[derive(Debug, Clone)]
pub struct TxInputP2TRKeyPath {
    ctx: InputContext,
    recipient: Recipient<TweakedPublicKey>,
}

impl TxInputP2TRKeyPath {
    pub fn new(
        txid: Txid,
        vout: u32,
        recipient: Recipient<TweakedPublicKey>,
        satoshis: u64,
    ) -> Self {
        let script = ScriptBuf::new_v1_p2tr_tweaked(recipient.tweaked_pubkey());
        Self::new_with_script(txid, vout, recipient, satoshis, script)
    }
    pub fn new_with_script(
        txid: Txid,
        vout: u32,
        recipient: Recipient<TweakedPublicKey>,
        satoshis: u64,
        script: ScriptBuf,
    ) -> Self {
        TxInputP2TRKeyPath {
            ctx: InputContext {
                previous_output: OutPoint { txid, vout },
                value: satoshis,
                script_pubkey: script,
                sequence: Sequence::default(),
            },
            recipient,
        }
    }
    pub fn builder() -> TxInputP2TRKeyPathBuilder {
        TxInputP2TRKeyPathBuilder::new()
    }
    /// Read-only exposure to the context.
    pub fn ctx(&self) -> &InputContext {
        &self.ctx
    }
    /// Read-only exposure to the recipient.
    pub fn recipient(&self) -> &Recipient<TweakedPublicKey> {
        &self.recipient
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
        recipient: impl Into<Recipient<TweakedPublicKey>>,
    ) -> TxInputP2TRKeyPathBuilder {
        self.recipient = Some(recipient.into());
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
