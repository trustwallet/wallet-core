use super::ordinals::OrdinalsInscription;
use crate::{Error, Recipient, Result};
use bitcoin::PublicKey;
use serde::{Deserialize, Serialize};
use tw_proto::BitcoinV2::Proto;

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

// Convenience alias.
pub type BRC20TransferPayload = BRC20Payload<TransferPayload>;

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
    pub fn to_byte_array(&self) -> [u8; 4] {
        self.0
            .as_bytes()
            .try_into()
            .expect("ticker must be four bytes")
    }
}

impl BRC20TransferPayload {
    const OPERATION: &str = "transfer";

    pub fn new(ticker: Brc20Ticker, value: u64) -> Self {
        BRC20Payload {
            protocol: Self::PROTOCOL_ID.to_string(),
            operation: Self::OPERATION.to_string(),
            inner: TransferPayload {
                tick: ticker,
                amt: value.to_string(),
            },
        }
    }
}

#[derive(Serialize, Deserialize)]
pub struct TransferPayload {
    pub tick: Brc20Ticker,
    pub amt: String,
}

pub struct BRC20TransferInscription(OrdinalsInscription);

impl BRC20TransferInscription {
    pub fn new(
        recipient: Recipient<PublicKey>,
        ticker: Brc20Ticker,
        value: u64,
    ) -> Result<BRC20TransferInscription> {
        let data: BRC20Payload<TransferPayload> = BRC20TransferPayload::new(ticker, value);
        Self::from_payload(data, recipient)
    }
    pub fn from_payload(
        data: BRC20TransferPayload,
        recipient: Recipient<PublicKey>,
    ) -> Result<BRC20TransferInscription> {
        let inscription = OrdinalsInscription::new(
            BRC20Payload::<TransferPayload>::MIME,
            &serde_json::to_vec(&data).expect("badly constructed BRC20 payload"),
            recipient,
        )?;

        Ok(BRC20TransferInscription(inscription))
    }
    pub fn inscription(&self) -> &OrdinalsInscription {
        &self.0
    }
}
