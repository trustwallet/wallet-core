use crate::{tweak_pubkey, Error, Recipient, Result, TaprootScript};
use bitcoin::address::Payload;
use bitcoin::key::TweakedPublicKey;
use bitcoin::script::ScriptBuf;
use bitcoin::taproot::TaprootSpendInfo;
use bitcoin::{Address, PubkeyHash, PublicKey, TxOut, WPubkeyHash};

#[derive(Debug, Clone)]
pub enum TxOutput {
    P2PKH(TxOutputP2PKH),
    P2WPKH(TxOutputP2WPKH),
    P2TRKeyPath(TxOutputP2TRKeyPath),
    P2TRScriptPath(TXOutputP2TRScriptPath),
}

impl TxOutput {
    pub fn from_address(address: Address) -> Result<Self> {
        match address.payload {
            Payload::PubkeyHash(hash) => todo!(),
            Payload::ScriptHash(hash) => todo!(),
            Payload::WitnessProgram(program) => todo!(),
            _ => Err(Error::Todo),
        }
    }
    pub fn satoshis(&self) -> u64 {
        match self {
            TxOutput::P2PKH(p) => p.satoshis,
            TxOutput::P2WPKH(p) => p.satoshis,
            TxOutput::P2TRKeyPath(p) => p.satoshis,
            TxOutput::P2TRScriptPath(p) => p.satoshis,
        }
    }
}

impl From<TxOutputP2PKH> for TxOutput {
    fn from(output: TxOutputP2PKH) -> Self {
        TxOutput::P2PKH(output)
    }
}

impl From<TxOutputP2TRKeyPath> for TxOutput {
    fn from(output: TxOutputP2TRKeyPath) -> Self {
        TxOutput::P2TRKeyPath(output)
    }
}

impl From<TxOutputP2WPKH> for TxOutput {
    fn from(output: TxOutputP2WPKH) -> Self {
        TxOutput::P2WPKH(output)
    }
}

impl From<TXOutputP2TRScriptPath> for TxOutput {
    fn from(output: TXOutputP2TRScriptPath) -> Self {
        TxOutput::P2TRScriptPath(output)
    }
}

impl From<TxOutput> for TxOut {
    fn from(out: TxOutput) -> Self {
        match out {
            TxOutput::P2PKH(p) => TxOut {
                value: p.satoshis,
                script_pubkey: p.script_pubkey,
            },
            TxOutput::P2WPKH(p) => TxOut {
                value: p.satoshis,
                script_pubkey: p.script_pubkey,
            },
            TxOutput::P2TRKeyPath(p) => TxOut {
                value: p.satoshis,
                script_pubkey: p.script_pubkey,
            },
            TxOutput::P2TRScriptPath(p) => TxOut {
                value: p.satoshis,
                script_pubkey: p.script_pubkey,
            },
        }
    }
}

#[derive(Debug, Clone)]
pub struct TxOutputP2PKH {
    satoshis: u64,
    script_pubkey: ScriptBuf,
}

impl TxOutputP2PKH {
    pub fn new(satoshis: u64, recipient: impl Into<Recipient<PubkeyHash>>) -> Self {
        let recipient: Recipient<PubkeyHash> = recipient.into();

        TxOutputP2PKH {
            satoshis,
            script_pubkey: ScriptBuf::new_p2pkh(recipient.pubkey_hash()),
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
    pub fn recipient(mut self, recipient: Recipient<PubkeyHash>) -> TxOutputP2PKHBuilder {
        self.recipient = Some(recipient);
        self
    }
    pub fn build(self) -> Result<TxOutputP2PKH> {
        Ok(TxOutputP2PKH::new(
            self.satoshis.ok_or(Error::Todo)?,
            self.recipient.ok_or(Error::Todo)?,
        ))
    }
}

#[derive(Debug, Clone)]
pub struct TxOutputP2WPKH {
    satoshis: u64,
    script_pubkey: ScriptBuf,
}

impl TxOutputP2WPKH {
    pub fn new(satoshis: u64, recipient: impl Into<Recipient<WPubkeyHash>>) -> Self {
        let recipient: Recipient<WPubkeyHash> = recipient.into();

        TxOutputP2WPKH {
            satoshis,
            script_pubkey: ScriptBuf::new_v0_p2wpkh(recipient.wpubkey_hash()),
        }
    }
    pub fn builder() -> TxOutputP2WPKHBuilder {
        TxOutputP2WPKHBuilder::new()
    }
}

#[derive(Debug, Clone, Default)]
pub struct TxOutputP2WPKHBuilder {
    satoshis: Option<u64>,
    recipient: Option<Recipient<WPubkeyHash>>,
}

impl TxOutputP2WPKHBuilder {
    pub fn new() -> TxOutputP2WPKHBuilder {
        Self::default()
    }
    pub fn satoshis(mut self, satoshis: u64) -> TxOutputP2WPKHBuilder {
        self.satoshis = Some(satoshis);
        self
    }
    pub fn recipient(mut self, recipient: Recipient<WPubkeyHash>) -> TxOutputP2WPKHBuilder {
        self.recipient = Some(recipient);
        self
    }
    pub fn build(self) -> Result<TxOutputP2WPKH> {
        Ok(TxOutputP2WPKH::new(
            self.satoshis.ok_or(Error::Todo)?,
            self.recipient.ok_or(Error::Todo)?,
        ))
    }
}

#[derive(Debug, Clone)]
pub struct TxOutputP2TRKeyPath {
    satoshis: u64,
    script_pubkey: ScriptBuf,
}

impl TxOutputP2TRKeyPath {
    pub fn new(satoshis: u64, recipient: impl Into<Recipient<TweakedPublicKey>>) -> Self {
        let recipient: Recipient<TweakedPublicKey> = recipient.into();

        TxOutputP2TRKeyPath {
            satoshis,
            script_pubkey: ScriptBuf::new_v1_p2tr_tweaked(recipient.t),
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
        recipient: Recipient<TweakedPublicKey>,
    ) -> TxOutputP2TRKeyPathBuilder {
        self.recipient = Some(recipient);
        self
    }
    pub fn build(self) -> Result<TxOutputP2TRKeyPath> {
        Ok(TxOutputP2TRKeyPath::new(
            self.satoshis.ok_or(Error::Todo)?,
            self.recipient.ok_or(Error::Todo)?,
        ))
    }
}

#[derive(Debug, Clone)]
pub struct TXOutputP2TRScriptPath {
    satoshis: u64,
    script_pubkey: ScriptBuf,
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
