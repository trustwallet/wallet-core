use crate::{Error, Recipient, Result};
use bitcoin::key::PublicKey;
use bitcoin::script::ScriptBuf;
use bitcoin::secp256k1;
use bitcoin::taproot::{TapNodeHash, TaprootSpendInfo};

#[derive(Debug, Clone, Eq, PartialEq)]
pub struct TaprootScript {
    pub pubkey: PublicKey,
    pub merkle_root: TapNodeHash,
}

#[derive(Debug, Clone)]
pub struct TaprootProgram {
    pub script: ScriptBuf,
    pub spend_info: TaprootSpendInfo,
}

#[derive(Debug, Clone)]
pub struct TXOutputP2TRScriptPath {
    pub(crate) satoshis: u64,
    pub(crate) script_pubkey: ScriptBuf,
}

impl TXOutputP2TRScriptPath {
    pub fn new(satoshis: u64, recipient: &Recipient<TaprootScript>) -> Self {
        let script_pubkey = ScriptBuf::new_v1_p2tr(
            &secp256k1::Secp256k1::new(),
            recipient.untweaked_pubkey(),
            Some(recipient.merkle_root()),
        );

        TXOutputP2TRScriptPath {
            satoshis,
            script_pubkey,
        }
    }
    pub fn new_with_script(satoshis: u64, script_pubkey: ScriptBuf) -> Self {
        TXOutputP2TRScriptPath {
            satoshis,
            script_pubkey,
        }
    }
    pub fn builder() -> TxOutputP2TRScriptPathBuilder {
        TxOutputP2TRScriptPathBuilder::new()
    }
}

#[derive(Debug, Clone, Default)]
pub struct TxOutputP2TRScriptPathBuilder {
    satoshis: Option<u64>,
    recipient: Option<Recipient<TaprootScript>>,
}

impl TxOutputP2TRScriptPathBuilder {
    pub fn new() -> Self {
        Self::default()
    }
    pub fn satoshis(mut self, satoshis: u64) -> TxOutputP2TRScriptPathBuilder {
        self.satoshis = Some(satoshis);
        self
    }
    pub fn recipient(
        mut self,
        recipient: Recipient<TaprootScript>,
    ) -> TxOutputP2TRScriptPathBuilder {
        self.recipient = Some(recipient);
        self
    }
    pub fn build(self) -> Result<TXOutputP2TRScriptPath> {
        let recipient = self.recipient.ok_or(Error::Todo)?;
        Ok(TXOutputP2TRScriptPath::new(
            self.satoshis.ok_or(Error::Todo)?,
            &recipient,
        ))
    }
}
