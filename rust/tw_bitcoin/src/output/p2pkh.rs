use crate::{Error, Recipient, Result};
use bitcoin::{PubkeyHash, ScriptBuf};

#[derive(Debug, Clone)]
pub struct TxOutputP2PKH {
    pub(super) satoshis: u64,
    pub(super) script_pubkey: ScriptBuf,
}

impl TxOutputP2PKH {
    pub fn new(satoshis: u64, recipient: impl Into<Recipient<PubkeyHash>>) -> Self {
        let recipient: Recipient<PubkeyHash> = recipient.into();

        TxOutputP2PKH {
            satoshis,
            script_pubkey: ScriptBuf::new_p2pkh(recipient.pubkey_hash()),
        }
    }
    pub fn from_bytes(bytes: Vec<u8>, satoshis: u64) -> Self {
        let script_pubkey = ScriptBuf::from_bytes(bytes);

        TxOutputP2PKH {
            satoshis,
            script_pubkey,
        }
    }
    pub fn builder() -> TxOutputP2PKHBuilder {
        TxOutputP2PKHBuilder::new()
    }
}

#[derive(Debug, Clone, Default)]
pub struct TxOutputP2PKHBuilder {
    satoshis: Option<u64>,
    recipient: Option<Recipient<PubkeyHash>>,
}

impl TxOutputP2PKHBuilder {
    pub fn new() -> TxOutputP2PKHBuilder {
        TxOutputP2PKHBuilder {
            satoshis: None,
            recipient: None,
        }
    }
    pub fn satoshis(mut self, satoshis: u64) -> TxOutputP2PKHBuilder {
        self.satoshis = Some(satoshis);
        self
    }
    pub fn recipient(
        mut self,
        recipient: impl Into<Recipient<PubkeyHash>>,
    ) -> TxOutputP2PKHBuilder {
        self.recipient = Some(recipient.into());
        self
    }
    pub fn build(self) -> Result<TxOutputP2PKH> {
        Ok(TxOutputP2PKH::new(
            self.satoshis.ok_or(Error::Todo)?,
            self.recipient.ok_or(Error::Todo)?,
        ))
    }
}
