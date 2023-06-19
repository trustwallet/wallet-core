use crate::{Error, InputContext, Recipient, Result};
use bitcoin::{OutPoint, ScriptBuf, Sequence, Txid, WPubkeyHash};

#[derive(Debug, Clone)]
pub struct TxInputP2WPKH {
    ctx: InputContext,
    recipient: Recipient<WPubkeyHash>,
}

impl TxInputP2WPKH {
    pub fn new(txid: Txid, vout: u32, recipient: Recipient<WPubkeyHash>, satoshis: u64) -> Self {
        let script = ScriptBuf::new_v0_p2wpkh(recipient.wpubkey_hash());
        Self::new_with_script(txid, vout, recipient, satoshis, script)
    }
    pub fn new_with_script(
        txid: Txid,
        vout: u32,
        recipient: Recipient<WPubkeyHash>,
        satoshis: u64,
        script: ScriptBuf,
    ) -> Self {
        TxInputP2WPKH {
            ctx: InputContext {
                previous_output: OutPoint { txid, vout },
                value: satoshis,
                script_pubkey: script,
                sequence: Sequence::default(),
            },
            recipient,
        }
    }
    pub fn builder() -> TxInputP2WPKHBuilder {
        TxInputP2WPKHBuilder::new()
    }
    /// Read-only exposure to the context.
    pub fn ctx(&self) -> &InputContext {
        &self.ctx
    }
    /// Read-only exposure to the recipient.
    pub fn recipient(&self) -> &Recipient<WPubkeyHash> {
        &self.recipient
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
            self.satoshis.ok_or(Error::Todo)?,
        ))
    }
}
