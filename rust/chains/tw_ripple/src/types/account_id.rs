// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::classic_address::ClassicAddress;
use crate::address::RippleAddress;
use crate::encode::encoder::Encoder;
use crate::encode::Encodable;
use std::fmt;
use std::str::FromStr;
use tw_coin_entry::error::prelude::*;
use tw_hash::H160;
use tw_misc::serde_as_string;

#[derive(Debug, PartialEq, Eq, Clone)]
pub struct AccountId(ClassicAddress);

serde_as_string!(AccountId);

impl Encodable for AccountId {
    fn encode(&self, dst: &mut Encoder) -> SigningResult<()> {
        dst.append_raw_slice(self.0.public_key_hash().as_slice());
        Ok(())
    }
}

impl FromStr for AccountId {
    type Err = SigningError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        // Try to decode as hex.
        if let Ok(bytes) = H160::from_str(s) {
            return ClassicAddress::new(bytes.as_slice())
                .into_tw()
                .map(AccountId);
        }

        // Decode as an address.
        match RippleAddress::from_str(s)
            .into_tw()
            .with_context(|| format!("Invalid AccountId: '{s}'"))?
        {
            RippleAddress::Classic(classic) => Ok(AccountId(classic)),
            RippleAddress::X(x_addr) => x_addr.to_classic().into_tw().map(AccountId),
        }
    }
}

impl fmt::Display for AccountId {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.0)
    }
}
