use crate::{tweak_pubkey, Error, InputContext, Recipient, Result, TaprootScript};
use bitcoin::key::{PublicKey, TweakedPublicKey};
use bitcoin::script::ScriptBuf;
use bitcoin::taproot::{ControlBlock, TapNodeHash, TaprootSpendInfo};
use bitcoin::{OutPoint, PubkeyHash, Sequence, TxIn, Txid, WPubkeyHash, Witness};

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
