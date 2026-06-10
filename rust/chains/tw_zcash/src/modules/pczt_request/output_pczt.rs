// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::ZcashAddress;
use crate::context::ZcashContext;
use crate::modules::pczt;
use std::str::FromStr;
use tw_coin_entry::error::prelude::{MapTWError, ResultContext, SigningErrorType, SigningResult};
use tw_utxo::context::{AddressPrefixes, UtxoContext};
use tw_utxo::script::Script;
use tw_utxo::transaction::standard_transaction::TransactionOutput;

const ZCASH_P2PKH_PREFIX: u8 = 0xB8;
const ZCASH_P2SH_PREFIX: u8 = 0xBD;

/// Currently, we rely on `pczt` crate to build our own [`TransactionOutput`].
pub struct OutputPczt<'a> {
    output: &'a pczt::transparent::Output,
}

impl<'a> OutputPczt<'a> {
    pub fn new(output: &'a pczt::transparent::Output) -> Self {
        OutputPczt { output }
    }

    pub fn build(self) -> SigningResult<TransactionOutput> {
        let value = self
            .output
            .value
            .try_into()
            .tw_err(SigningErrorType::Error_invalid_requested_token_amount)
            .context("PCZT Output amount is too large")?;
        let script_pubkey = Script::from(self.output.script_pubkey.clone());

        if let Some(ref addr_str) = self.output.user_address {
            let addr = ZcashAddress::from_str(addr_str)
                .tw_err(SigningErrorType::Error_invalid_address)
                .context("PCZT Output user_address is not a valid Zcash address")?;
            let expected_script = ZcashContext::addr_to_script_pubkey(
                &addr,
                AddressPrefixes {
                    p2pkh_prefix: ZCASH_P2PKH_PREFIX,
                    p2sh_prefix: ZCASH_P2SH_PREFIX,
                },
            )
            .context("PCZT Output user_address cannot be converted to a script")?;
            if expected_script != script_pubkey {
                return tw_coin_entry::error::prelude::SigningError::err(
                    SigningErrorType::Error_invalid_address,
                )
                .context("PCZT Output user_address does not match script_pubkey");
            }
        }

        Ok(TransactionOutput {
            value,
            script_pubkey,
        })
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::collections::BTreeMap;

    fn make_output(
        script_pubkey: Vec<u8>,
        user_address: Option<&str>,
    ) -> pczt::transparent::Output {
        pczt::transparent::Output {
            value: 100_000,
            script_pubkey,
            redeem_script: None,
            bip32_derivation: BTreeMap::new(),
            user_address: user_address.map(str::to_owned),
            proprietary: BTreeMap::new(),
        }
    }

    const ATTACKER_SCRIPT: &str = "76a91449964a736f3713d64283fd0018626ba50091c7e988ac";

    const MERCHANT_ADDR: &str = "t1gWVE2uyrET2CxSmCaBiKzmWxQdHhnvMSz";
    const MERCHANT_SCRIPT: &str = "76a914f84c7f4dd3c3dc311676444fdead6e6d290d50e388ac";

    fn hex_to_bytes(hex: &str) -> Vec<u8> {
        (0..hex.len())
            .step_by(2)
            .map(|i| u8::from_str_radix(&hex[i..i + 2], 16).unwrap())
            .collect()
    }

    #[test]
    fn test_no_user_address_passes() {
        let output = make_output(hex_to_bytes(MERCHANT_SCRIPT), None);
        assert!(OutputPczt::new(&output).build().is_ok());
    }

    #[test]
    fn test_matching_user_address_passes() {
        let output = make_output(hex_to_bytes(MERCHANT_SCRIPT), Some(MERCHANT_ADDR));
        assert!(OutputPczt::new(&output).build().is_ok());
    }

    #[test]
    fn test_mismatched_user_address_rejected() {
        let output = make_output(hex_to_bytes(ATTACKER_SCRIPT), Some(MERCHANT_ADDR));
        let result = OutputPczt::new(&output).build();
        assert!(result.is_err());
        assert_eq!(
            *result.unwrap_err().error_type(),
            tw_coin_entry::error::prelude::SigningErrorType::Error_invalid_address,
        );
    }

    #[test]
    fn test_invalid_user_address_rejected() {
        let output = make_output(hex_to_bytes(MERCHANT_SCRIPT), Some("not_a_valid_address"));
        let result = OutputPczt::new(&output).build();
        assert!(result.is_err());
    }
}
