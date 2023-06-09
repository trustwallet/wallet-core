use crate::{Error, InputContext, Recipient, Result};
use bitcoin::{OutPoint, PubkeyHash, ScriptBuf, Sequence, Txid, Witness};

#[derive(Debug, Clone)]
pub struct TxInputP2PKH {
    ctx: InputContext,
    recipient: Recipient<PubkeyHash>,
}

impl TxInputP2PKH {
    pub fn new(
        txid: Txid,
        vout: u32,
        recipient: impl Into<Recipient<PubkeyHash>>,
        satoshis: u64,
    ) -> Self {
        let recipient: Recipient<PubkeyHash> = recipient.into();

        TxInputP2PKH {
            ctx: InputContext {
                previous_output: OutPoint { txid, vout },
                value: Some(satoshis),
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
