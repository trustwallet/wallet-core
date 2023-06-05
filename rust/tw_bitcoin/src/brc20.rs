use crate::ordinals::{new_ordinals_inscription, OrdinalsInscription};
use crate::{Error, Recipient, Result};
use bitcoin::PublicKey;
use serde::{Deserialize, Serialize};

#[derive(Serialize, Deserialize)]
pub struct BRC20<T> {
    p: &'static str,
    op: &'static str,
    #[serde(flatten)]
    inner: T,
}

impl<T> BRC20<T> {
    const PROTOCOL_ID: &str = "brc-20";
    const MIME: &[u8] = b"text/plain;charset=utf-8";
}

impl BRC20<DeploymentInfo> {
    const OPERATION: &str = "deploy";

    pub fn new(ticker: String, max: usize, limit: Option<usize>, decimals: Option<usize>) -> Self {
        BRC20 {
            p: Self::PROTOCOL_ID,
            op: Self::OPERATION,
            inner: DeploymentInfo {
                tick: ticker,
                max,
                lim: limit,
                dec: decimals,
            },
        }
    }
}

impl BRC20<TransferInfo> {
    const OPERATION: &str = "transfer";

    pub fn new(ticker: String, amount: usize) -> Self {
        BRC20 {
            p: Self::PROTOCOL_ID,
            op: Self::OPERATION,
            inner: TransferInfo {
                tick: ticker,
                amt: amount,
            },
        }
    }
}

impl BRC20<MintInfo> {
    const OPERATION: &str = "mint";

    pub fn new(ticker: String, amount: usize) -> Self {
        BRC20 {
            p: Self::PROTOCOL_ID,
            op: Self::OPERATION,
            inner: MintInfo {
                tick: ticker,
                amt: amount,
            },
        }
    }
}

#[derive(Serialize, Deserialize)]
pub struct DeploymentInfo {
    pub tick: String,
    pub max: usize,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub lim: Option<usize>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub dec: Option<usize>,
}

pub struct BRC20Deploy(OrdinalsInscription);

impl BRC20Deploy {
    pub fn new(
        recipient: Recipient<PublicKey>,
        ticker: String,
        max: usize,
        limit: Option<usize>,
        decimals: Option<usize>,
    ) -> Result<BRC20Deploy> {
        // Ticker must be a 4-letter identifier.
        if ticker.len() != 4 {
            return Err(Error::Todo);
        }

        let data = BRC20::<DeploymentInfo>::new(ticker, max, limit, decimals);

        Self::from_payload(data, recipient)
    }
    pub fn from_payload(
        data: BRC20<DeploymentInfo>,
        recipient: Recipient<PublicKey>,
    ) -> Result<BRC20Deploy> {
        let inscription = new_ordinals_inscription(
            BRC20::<DeploymentInfo>::MIME,
            &serde_json::to_vec(&data).unwrap(),
            recipient,
        )?;

        Ok(BRC20Deploy(inscription))
    }
}

#[derive(Serialize, Deserialize)]
pub struct TransferInfo {
    pub tick: String,
    pub amt: usize,
}

pub struct BRC20Transfer(OrdinalsInscription);

impl BRC20Transfer {
    pub fn new(
        recipient: Recipient<PublicKey>,
        ticker: String,
        amount: usize,
    ) -> Result<BRC20Transfer> {
        // Ticker must be a 4-letter identifier.
        if ticker.len() != 4 {
            return Err(Error::Todo);
        }

        let data = BRC20::<TransferInfo>::new(ticker, amount);
        Self::from_payload(data, recipient)
    }
    pub fn from_payload(
        data: BRC20<TransferInfo>,
        recipient: Recipient<PublicKey>,
    ) -> Result<BRC20Transfer> {
        let inscription = new_ordinals_inscription(
            BRC20::<TransferInfo>::MIME,
            &serde_json::to_vec(&data).unwrap(),
            recipient,
        )?;

        Ok(BRC20Transfer(inscription))
    }
}

/// The structure is the same as `TransferInfo`, we'll keep it separate for
/// clarity.
#[derive(Serialize, Deserialize)]
pub struct MintInfo {
    pub tick: String,
    pub amt: usize,
}

pub struct BRC20Mint(OrdinalsInscription);

impl BRC20Mint {
    pub fn new(
        recipient: Recipient<PublicKey>,
        ticker: String,
        amount: usize,
    ) -> Result<BRC20Mint> {
        // Ticker must be a 4-letter identifier.
        if ticker.len() != 4 {
            return Err(Error::Todo);
        }

        let data = BRC20::<MintInfo>::new(ticker, amount);
        Self::from_payload(data, recipient)
    }
    pub fn from_payload(
        data: BRC20<MintInfo>,
        recipient: Recipient<PublicKey>,
    ) -> Result<BRC20Mint> {
        let inscription = new_ordinals_inscription(
            BRC20::<MintInfo>::MIME,
            &serde_json::to_vec(&data).unwrap(),
            recipient,
        )?;

        Ok(BRC20Mint(inscription))
    }
}
