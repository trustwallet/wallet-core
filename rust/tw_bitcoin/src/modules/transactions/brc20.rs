use super::ordinals::OrdinalsInscription;
use crate::{Error, Result};
use bitcoin::PublicKey;
use serde::Serialize;
use tw_proto::BitcoinV2::Proto;

#[derive(Debug, Clone, Serialize)]
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

#[derive(Serialize)]
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
    const PROTOCOL_ID: &'static str = "brc-20";
    const MIME: &'static [u8] = b"text/plain;charset=utf-8";
}

impl BRC20TransferPayload {
    const OPERATION: &'static str = "transfer";

    fn new(ticker: Brc20Ticker, amount: String) -> Self {
        BRC20TransferPayload {
            protocol: Self::PROTOCOL_ID.to_string(),
            operation: Self::OPERATION.to_string(),
            ticker,
            amount,
        }
    }
}

pub struct BRC20TransferInscription(OrdinalsInscription);

impl BRC20TransferInscription {
    pub fn new(
        recipient: PublicKey,
        ticker: Brc20Ticker,
        value: String,
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

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn brc20_ticker_validity() {
        // Must be four characters.
        let ticker = Brc20Ticker::new("invalid".to_string());
        assert!(ticker.is_err());

        let ticker = Brc20Ticker::new("asdf".to_string());
        assert!(ticker.is_ok());

        // Cover clone implemenation.
        let ticker = ticker.unwrap();

        let _cloned = ticker.clone();
        let _ticker = ticker;
    }
}
