use super::ordinal::OrdinalsInscription;
use bitcoin::hashes::Hash;
use std::ops::Deref;
use tw_coin_entry::error::prelude::*;
use tw_hash::{H256, H264};

#[derive(Debug, Clone)]
pub struct Brc20Ticker(String);

impl Brc20Ticker {
    pub fn new(string: String) -> SigningResult<Self> {
        // Brc20Ticker must be a 4-letter identifier.
        if string.len() != 4 {
            return SigningError::err(SigningErrorType::Error_invalid_params)
                .context("BRC20 ticker must be exactly 4 bytes length");
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
    ) -> SigningResult<BRC20TransferInscription> {
        let payload = format!(
            "{{\"p\":\"{protocol}\",\"op\":\"transfer\",\"tick\":\"{ticker}\",\"amt\":\"{amt}\"}}",
            protocol = BRC20_PROTOCOL_ID,
            ticker = ticker.0,
            amt = value
        );

        let inscription = OrdinalsInscription::new(BRC20_MIME, payload.as_bytes(), recipient)?;
        Ok(BRC20TransferInscription(inscription))
    }

    pub fn merkle_root(&self) -> SigningResult<H256> {
        self.spend_info
            .merkle_root()
            .map(|root| H256::from(root.to_byte_array()))
            .or_tw_err(SigningErrorType::Error_internal)
            .context("No merkle root of the BRC20 Transfer spend info")
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
