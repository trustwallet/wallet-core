use crate::{Error, InputContext, Recipient, Result};
use bitcoin::{OutPoint, PubkeyHash, ScriptBuf, Sequence, Txid};

#[derive(Debug, Clone)]
pub struct TxInputP2PKH {
    ctx: InputContext,
    recipient: Recipient<PubkeyHash>,
}

impl TxInputP2PKH {
    pub fn new(txid: Txid, vout: u32, recipient: Recipient<PubkeyHash>, satoshis: u64) -> Self {
        let script = ScriptBuf::new_p2pkh(recipient.pubkey_hash());
        Self::new_with_script(txid, vout, recipient, satoshis, script)
    }
    pub fn new_with_script(
        txid: Txid,
        vout: u32,
        recipient: Recipient<PubkeyHash>,
        satoshis: u64,
        script: ScriptBuf,
    ) -> Self {
        TxInputP2PKH {
            ctx: InputContext {
                previous_output: OutPoint { txid, vout },
                value: satoshis,
                script_pubkey: script,
                sequence: Sequence::default(),
            },
            recipient,
        }
    }
    pub fn builder() -> TxInputP2PKHBuilder {
        TxInputP2PKHBuilder::new()
    }
    /// Read-only exposure to the context.
    pub fn ctx(&self) -> &InputContext {
        &self.ctx
    }
    /// Read-only exposure to the recipient.
    pub fn recipient(&self) -> &Recipient<PubkeyHash> {
        &self.recipient
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
    pub fn recipient(mut self, recipient: impl Into<Recipient<PubkeyHash>>) -> TxInputP2PKHBuilder {
        self.recipient = Some(recipient.into());
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
            self.satoshis.ok_or(Error::Todo)?,
        ))
    }
}
