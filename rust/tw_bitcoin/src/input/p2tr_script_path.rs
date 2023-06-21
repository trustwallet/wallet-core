use crate::{Error, InputContext, Recipient, Result, TaprootScript};
use bitcoin::script::ScriptBuf;
use bitcoin::taproot::TaprootSpendInfo;
use bitcoin::{OutPoint, Sequence, Txid};

#[derive(Debug, Clone)]
pub struct TxInputP2TRScriptPath {
    ctx: InputContext,
    recipient: Recipient<TaprootScript>,
    witness: ScriptBuf,
    spend_info: TaprootSpendInfo,
}

impl TxInputP2TRScriptPath {
    pub fn new(
        txid: Txid,
        vout: u32,
        recipient: Recipient<TaprootScript>,
        satoshis: u64,
        witness: ScriptBuf,
        spend_info: TaprootSpendInfo,
    ) -> Self {
        let script = ScriptBuf::new_v1_p2tr(
            &secp256k1::Secp256k1::new(),
            recipient.untweaked_pubkey(),
            Some(recipient.merkle_root()),
        );

        Self::new_with_script(txid, vout, recipient, satoshis, script, witness, spend_info)
    }
    pub fn new_with_script(
        txid: Txid,
        vout: u32,
        recipient: Recipient<TaprootScript>,
        satoshis: u64,
        script: ScriptBuf,
        witness: ScriptBuf,
        spend_info: TaprootSpendInfo,
    ) -> Self {
        TxInputP2TRScriptPath {
            ctx: InputContext {
                previous_output: OutPoint { txid, vout },
                value: satoshis,
                script_pubkey: script,
                sequence: Sequence::default(),
            },
            recipient,
            witness,
            spend_info,
        }
    }
    pub fn builder() -> TxInputP2TRScriptPathBuilder {
        TxInputP2TRScriptPathBuilder::new()
    }
    pub fn ctx(&self) -> &InputContext {
        &self.ctx
    }
    pub fn recipient(&self) -> &Recipient<TaprootScript> {
        &self.recipient
    }
    pub fn witness(&self) -> &ScriptBuf {
        &self.witness
    }
    pub fn spend_info(&self) -> &TaprootSpendInfo {
        &self.spend_info
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
