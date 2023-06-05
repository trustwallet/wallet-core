use crate::ordinals::{new_ordinals_inscription, OrdinalsInscription};
use crate::{Error, Recipient, Result};
use bitcoin::PublicKey;
use serde::{Deserialize, Serialize};

#[derive(Serialize, Deserialize)]
pub struct BRC20Payload<T> {
    p: &'static str,
    op: &'static str,
    #[serde(flatten)]
    inner: T,
}

type BRC20DeployPayload = BRC20Payload<DeployPayload>;
type BRC20MintPayload = BRC20Payload<MintPayload>;
type BRC20TransferPayload = BRC20Payload<TransferPayload>;

impl<T> BRC20Payload<T> {
    const PROTOCOL_ID: &str = "brc-20";
    const MIME: &[u8] = b"text/plain;charset=utf-8";
}

impl BRC20DeployPayload {
    const OPERATION: &str = "deploy";

    pub fn new(ticker: String, max: usize, limit: Option<usize>, decimals: Option<usize>) -> Self {
        BRC20Payload {
            p: Self::PROTOCOL_ID,
            op: Self::OPERATION,
            inner: DeployPayload {
                tick: ticker,
                max,
                lim: limit,
                dec: decimals,
            },
        }
    }
}

impl BRC20TransferPayload {
    const OPERATION: &str = "transfer";

    pub fn new(ticker: String, amount: usize) -> Self {
        BRC20Payload {
            p: Self::PROTOCOL_ID,
            op: Self::OPERATION,
            inner: TransferPayload {
                tick: ticker,
                amt: amount,
            },
        }
    }
}

impl BRC20MintPayload {
    const OPERATION: &str = "mint";

    pub fn new(ticker: String, amount: usize) -> Self {
        BRC20Payload {
            p: Self::PROTOCOL_ID,
            op: Self::OPERATION,
            inner: MintPayload {
                tick: ticker,
                amt: amount,
            },
        }
    }
}

#[derive(Serialize, Deserialize)]
pub struct DeployPayload {
    pub tick: String,
    pub max: usize,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub lim: Option<usize>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub dec: Option<usize>,
}

pub struct BRC20DeployInscription(OrdinalsInscription);

impl BRC20DeployInscription {
    pub fn new(
        recipient: Recipient<PublicKey>,
        ticker: String,
        max: usize,
        limit: Option<usize>,
        decimals: Option<usize>,
    ) -> Result<BRC20DeployInscription> {
        // Ticker must be a 4-letter identifier.
        if ticker.len() != 4 {
            return Err(Error::Todo);
        }

        let data = BRC20Payload::<DeployPayload>::new(ticker, max, limit, decimals);

        Self::from_payload(data, recipient)
    }
    pub fn from_payload(
        data: BRC20DeployPayload,
        recipient: Recipient<PublicKey>,
    ) -> Result<BRC20DeployInscription> {
        let inscription = new_ordinals_inscription(
            BRC20Payload::<DeployPayload>::MIME,
            &serde_json::to_vec(&data).unwrap(),
            recipient,
        )?;

        Ok(BRC20DeployInscription(inscription))
    }
}

#[derive(Serialize, Deserialize)]
pub struct TransferPayload {
    pub tick: String,
    pub amt: usize,
}

pub struct BRC20TransferInscription(OrdinalsInscription);

impl BRC20TransferInscription {
    pub fn new(
        recipient: Recipient<PublicKey>,
        ticker: String,
        amount: usize,
    ) -> Result<BRC20TransferInscription> {
        // Ticker must be a 4-letter identifier.
        if ticker.len() != 4 {
            return Err(Error::Todo);
        }

        let data = BRC20Payload::<TransferPayload>::new(ticker, amount);
        Self::from_payload(data, recipient)
    }
    pub fn from_payload(
        data: BRC20TransferPayload,
        recipient: Recipient<PublicKey>,
    ) -> Result<BRC20TransferInscription> {
        let inscription = new_ordinals_inscription(
            BRC20Payload::<TransferPayload>::MIME,
            &serde_json::to_vec(&data).unwrap(),
            recipient,
        )?;

        Ok(BRC20TransferInscription(inscription))
    }
}

/// The structure is the same as `TransferPayload`, we'll keep it separate for
/// clarity.
#[derive(Serialize, Deserialize)]
pub struct MintPayload {
    pub tick: String,
    pub amt: usize,
}

pub struct BRC20MintInscription(OrdinalsInscription);

impl BRC20MintInscription {
    pub fn new(
        recipient: Recipient<PublicKey>,
        ticker: String,
        amount: usize,
    ) -> Result<BRC20MintInscription> {
        // Ticker must be a 4-letter identifier.
        if ticker.len() != 4 {
            return Err(Error::Todo);
        }

        let data = BRC20Payload::<MintPayload>::new(ticker, amount);
        Self::from_payload(data, recipient)
    }
    pub fn from_payload(
        data: BRC20MintPayload,
        recipient: Recipient<PublicKey>,
    ) -> Result<BRC20MintInscription> {
        let inscription = new_ordinals_inscription(
            BRC20Payload::<MintPayload>::MIME,
            &serde_json::to_vec(&data).unwrap(),
            recipient,
        )?;

        Ok(BRC20MintInscription(inscription))
    }
}
