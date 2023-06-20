use crate::ordinals::OrdinalsInscription;
use crate::{Error, Recipient, Result};
use bitcoin::PublicKey;
use serde::{Deserialize, Serialize};

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct BRC20Payload<T> {
    #[serde(rename = "p")]
    protocol: String,
    #[serde(rename = "op")]
    operation: String,
    #[serde(flatten)]
    inner: T,
}

impl<T> BRC20Payload<T> {
    const PROTOCOL_ID: &str = "brc-20";
    const MIME: &[u8] = b"text/plain;charset=utf-8";
}

// Convenience aliases.
pub type BRC20DeployPayload = BRC20Payload<DeployPayload>;
pub type BRC20MintPayload = BRC20Payload<MintPayload>;
pub type BRC20TransferPayload = BRC20Payload<TransferPayload>;

#[derive(Clone, Debug, Serialize, Deserialize)]
pub struct Ticker(String);

impl Ticker {
    pub fn new(string: String) -> Result<Self> {
        // Ticker must be a 4-letter identifier.
        if string.len() != 4 {
            return Err(Error::Todo);
        }

        Ok(Ticker(string))
    }
    pub fn to_byte_array(&self) -> [u8; 4] {
        self.0
            .as_bytes()
            .try_into()
            .expect("ticker must be four bytes")
    }
}

impl TryFrom<String> for Ticker {
    type Error = Error;

    fn try_from(string: String) -> Result<Self> {
        Self::new(string)
    }
}

impl BRC20DeployPayload {
    const OPERATION: &str = "deploy";

    pub fn new(ticker: Ticker, max: usize, limit: Option<usize>, decimals: Option<usize>) -> Self {
        BRC20Payload {
            protocol: Self::PROTOCOL_ID.to_string(),
            operation: Self::OPERATION.to_string(),
            inner: DeployPayload {
                tick: ticker,
                max: max.to_string(),
                lim: limit.map(|l| l.to_string()),
                dec: decimals.map(|d| d.to_string()),
            },
        }
    }
}

impl BRC20TransferPayload {
    const OPERATION: &str = "transfer";

    pub fn new(ticker: Ticker, amount: u64) -> Self {
        BRC20Payload {
            protocol: Self::PROTOCOL_ID.to_string(),
            operation: Self::OPERATION.to_string(),
            inner: TransferPayload {
                tick: ticker,
                amt: amount.to_string(),
            },
        }
    }
}

impl BRC20MintPayload {
    const OPERATION: &str = "mint";

    pub fn new(ticker: Ticker, amount: u64) -> Self {
        BRC20Payload {
            protocol: Self::PROTOCOL_ID.to_string(),
            operation: Self::OPERATION.to_string(),
            inner: MintPayload {
                tick: ticker,
                amt: amount.to_string(),
            },
        }
    }
}

#[derive(Clone, Debug, Serialize, Deserialize)]
pub struct DeployPayload {
    pub tick: Ticker,
    pub max: String,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub lim: Option<String>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub dec: Option<String>,
}

#[derive(Debug, Clone)]
pub struct BRC20DeployInscription(OrdinalsInscription);

impl BRC20DeployInscription {
    pub fn new(
        recipient: Recipient<PublicKey>,
        ticker: Ticker,
        max: usize,
        limit: Option<usize>,
        decimals: Option<usize>,
    ) -> Result<BRC20DeployInscription> {
        let data = BRC20DeployPayload::new(ticker, max, limit, decimals);

        Self::from_payload(data, recipient)
    }
    pub fn from_payload(
        data: BRC20DeployPayload,
        recipient: Recipient<PublicKey>,
    ) -> Result<BRC20DeployInscription> {
        let inscription = OrdinalsInscription::new(
            BRC20Payload::<DeployPayload>::MIME,
            &serde_json::to_vec(&data).unwrap(),
            recipient,
        )?;

        Ok(BRC20DeployInscription(inscription))
    }
    pub fn inscription(&self) -> &OrdinalsInscription {
        &self.0
    }
}

#[derive(Serialize, Deserialize)]
pub struct TransferPayload {
    pub tick: Ticker,
    pub amt: String,
}

pub struct BRC20TransferInscription(OrdinalsInscription);

impl BRC20TransferInscription {
    pub fn new(
        recipient: Recipient<PublicKey>,
        ticker: Ticker,
        amount: u64,
    ) -> Result<BRC20TransferInscription> {
        let data = BRC20TransferPayload::new(ticker, amount);
        Self::from_payload(data, recipient)
    }
    pub fn from_payload(
        data: BRC20TransferPayload,
        recipient: Recipient<PublicKey>,
    ) -> Result<BRC20TransferInscription> {
        let inscription = OrdinalsInscription::new(
            BRC20Payload::<TransferPayload>::MIME,
            &serde_json::to_vec(&data).unwrap(),
            recipient,
        )?;

        Ok(BRC20TransferInscription(inscription))
    }
    pub fn inscription(&self) -> &OrdinalsInscription {
        &self.0
    }
}

/// The structure is the same as `TransferPayload`, but we'll keep it separate
/// for clarity.
#[derive(Serialize, Deserialize)]
pub struct MintPayload {
    pub tick: Ticker,
    pub amt: String,
}

pub struct BRC20MintInscription(OrdinalsInscription);

impl BRC20MintInscription {
    pub fn new(
        recipient: Recipient<PublicKey>,
        ticker: Ticker,
        amount: u64,
    ) -> Result<BRC20MintInscription> {
        let data = BRC20MintPayload::new(ticker, amount);
        Self::from_payload(data, recipient)
    }
    pub fn from_payload(
        data: BRC20MintPayload,
        recipient: Recipient<PublicKey>,
    ) -> Result<BRC20MintInscription> {
        let inscription = OrdinalsInscription::new(
            BRC20Payload::<MintPayload>::MIME,
            &serde_json::to_vec(&data).unwrap(),
            recipient,
        )?;

        Ok(BRC20MintInscription(inscription))
    }
    pub fn inscription(&self) -> &OrdinalsInscription {
        &self.0
    }
}
