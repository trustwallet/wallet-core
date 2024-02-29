use std::ops::Deref;

use super::ordinal::OrdinalsInscription;
use crate::Result;
use tw_hash::H264;

#[derive(Debug, Clone)]
pub struct Brc20Ticker(String);

impl Brc20Ticker {
    pub fn new(string: String) -> Result<Self> {
        // Brc20Ticker must be a 4-letter identifier.
        if string.len() != 4 {
            // TODO:
            return Err(crate::Error::from(
                tw_proto::Utxo::Proto::Error::Error_internal,
            ));
        }

        Ok(Brc20Ticker(string))
    }
}

const BRC20_PROTOCOL_ID: &str = "brc-20";
const BRC20_MIME: &[u8] = b"text/plain;charset=utf-8";

pub struct BRC20TransferInscription(OrdinalsInscription);

impl Deref for BRC20TransferInscription {
    type Target = OrdinalsInscription;

    fn deref(&self) -> &Self::Target {
        &self.0
    }
}

impl BRC20TransferInscription {
    pub fn new(
        recipient: &H264,
        ticker: &Brc20Ticker,
        value: &str,
    ) -> Result<BRC20TransferInscription> {
        let payload = format!(
            "{{\"p\":\"{protocol}\",\"op\":\"transfer\",\"tick\":\"{ticker}\",\"amt\":\"{amt}\"}}",
            protocol = BRC20_PROTOCOL_ID,
            ticker = ticker.0,
            amt = value
        );

        println!("{payload}");

        let inscription = OrdinalsInscription::new(BRC20_MIME, payload.as_bytes(), recipient)?;

        Ok(BRC20TransferInscription(inscription))
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
