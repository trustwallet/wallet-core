use crate::{Result, Error, InputContext, Recipient};
use bitcoin::{OutPoint, Sequence, Txid, WPubkeyHash, Witness, ScriptBuf};

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
