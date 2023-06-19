use crate::{Error, Recipient, Result};
use bitcoin::key::TweakedPublicKey;
use bitcoin::script::ScriptBuf;

#[derive(Debug, Clone)]
pub struct TxOutputP2TRKeyPath {
    pub(crate) satoshis: u64,
    pub(crate) script_pubkey: ScriptBuf,
}

impl TxOutputP2TRKeyPath {
    pub fn new(satoshis: u64, recipient: Recipient<TweakedPublicKey>) -> Self {
        TxOutputP2TRKeyPath {
            satoshis,
            script_pubkey: ScriptBuf::new_v1_p2tr_tweaked(recipient.tweaked_pubkey()),
        }
    }
    pub fn new_with_script(satoshis: u64, script_pubkey: ScriptBuf) -> Self {
        TxOutputP2TRKeyPath {
            satoshis,
            script_pubkey,
        }
    }
    pub fn builder() -> TxOutputP2TRKeyPathBuilder {
        TxOutputP2TRKeyPathBuilder::new()
    }
}

#[derive(Debug, Clone, Default)]
pub struct TxOutputP2TRKeyPathBuilder {
    satoshis: Option<u64>,
    recipient: Option<Recipient<TweakedPublicKey>>,
}

impl TxOutputP2TRKeyPathBuilder {
    pub fn new() -> Self {
        Self::default()
    }
    pub fn satoshis(mut self, satoshis: u64) -> TxOutputP2TRKeyPathBuilder {
        self.satoshis = Some(satoshis);
        self
    }
    pub fn recipient(
        mut self,
        recipient: impl Into<Recipient<TweakedPublicKey>>,
    ) -> TxOutputP2TRKeyPathBuilder {
        self.recipient = Some(recipient.into());
        self
    }
    pub fn build(self) -> Result<TxOutputP2TRKeyPath> {
        Ok(TxOutputP2TRKeyPath::new(
            self.satoshis.ok_or(Error::Todo)?,
            self.recipient.ok_or(Error::Todo)?,
        ))
    }
}
