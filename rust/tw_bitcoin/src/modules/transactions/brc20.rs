use super::ordinals::OrdinalsInscription;
use crate::{Error, Recipient, Result};
use bitcoin::PublicKey;
use serde::{Deserialize, Serialize};
use tw_proto::BitcoinV2::Proto;

#[derive(Clone, Debug, Serialize, Deserialize)]
pub struct Brc20Ticker(String);

impl Brc20Ticker {
    pub fn new(string: String) -> Result<Self> {
        // Brc20Ticker must be a 4-letter identifier.
        if string.len() != 4 {
            return Err(Error::from(Proto::Error::Error_invalid_brc20_ticker));
        }

        Ok(Brc20Ticker(string))
    }
}

#[derive(Debug, Clone, Serialize, Deserialize)]
struct BRC20TransferPayload {
    #[serde(rename = "p")]
    protocol: String,
    #[serde(rename = "op")]
    operation: String,
    #[serde(rename = "tick")]
    ticker: Brc20Ticker,
    #[serde(rename = "amt")]
    amount: String,
}

impl BRC20TransferPayload {
    const PROTOCOL_ID: &str = "brc-20";
    const MIME: &[u8] = b"text/plain;charset=utf-8";
}

impl BRC20TransferPayload {
    const OPERATION: &str = "transfer";

    fn new(ticker: Brc20Ticker, value: u64) -> Self {
        BRC20TransferPayload {
            protocol: Self::PROTOCOL_ID.to_string(),
            operation: Self::OPERATION.to_string(),
            ticker,
            amount: value.to_string(),
        }
    }
}

pub struct BRC20TransferInscription(OrdinalsInscription);

impl BRC20TransferInscription {
    pub fn new(
        recipient: Recipient<PublicKey>,
        ticker: Brc20Ticker,
        value: u64,
    ) -> Result<BRC20TransferInscription> {
        let data: BRC20TransferPayload = BRC20TransferPayload::new(ticker, value);

        let inscription = OrdinalsInscription::new(
            BRC20TransferPayload::MIME,
            &serde_json::to_vec(&data).expect("badly constructed BRC20 payload"),
            recipient,
        )?;

        Ok(BRC20TransferInscription(inscription))
    }
    pub fn inscription(&self) -> &OrdinalsInscription {
        &self.0
    }
}
