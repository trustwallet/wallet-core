// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::Address;
use crate::message::{EthMessage, MessageSigningErrorKind, MessageSigningResult};
use crate::rlp::list::RlpList;
use crate::transaction::authorization_list::Authorization;
use serde::{Deserialize, Deserializer};
use std::iter;
use std::str::FromStr;
use tw_coin_entry::error::prelude::*;
use tw_hash::sha3::keccak256;
use tw_hash::H256;
use tw_number::U256;

pub const EIP_7702_MAGIC_PREFIX: u8 = 0x05;

impl<'de> Deserialize<'de> for Authorization {
    fn deserialize<D>(deserializer: D) -> Result<Self, D::Error>
    where
        D: Deserializer<'de>,
    {
        use serde::de::{self, MapAccess, Visitor};
        use std::fmt;

        struct AuthorizationVisitor;

        impl<'de> Visitor<'de> for AuthorizationVisitor {
            type Value = Authorization;

            fn expecting(&self, formatter: &mut fmt::Formatter) -> fmt::Result {
                formatter.write_str("struct Authorization")
            }

            fn visit_map<V>(self, mut map: V) -> Result<Self::Value, V::Error>
            where
                V: MapAccess<'de>,
            {
                let mut chain_id = None;
                let mut address = None;
                let mut nonce = None;

                while let Some(key) = map.next_key::<&str>()? {
                    match key {
                        "chainId" => {
                            let value: String = map.next_value()?;
                            chain_id = Some(U256::from_str(&value).map_err(de::Error::custom)?);
                        },
                        "address" => {
                            let value: String = map.next_value()?;
                            address = Some(Address::from_str(&value).map_err(de::Error::custom)?);
                        },
                        "nonce" => {
                            let value: String = map.next_value()?;
                            nonce = Some(U256::from_str(&value).map_err(de::Error::custom)?);
                        },
                        // Skip unknown field.
                        _ => {},
                    }
                }

                let chain_id = chain_id.ok_or_else(|| de::Error::missing_field("chainId"))?;
                let address = address.ok_or_else(|| de::Error::missing_field("address"))?;
                let nonce = nonce.ok_or_else(|| de::Error::missing_field("nonce"))?;

                Ok(Authorization {
                    chain_id,
                    address,
                    nonce,
                })
            }
        }

        deserializer.deserialize_struct(
            "Authorization",
            &["chainId", "address", "nonce"],
            AuthorizationVisitor,
        )
    }
}

impl Authorization {
    /// Tries to construct an EIP7702 authorization tuple from the given string.
    pub fn new<S: AsRef<str>>(message_to_sign: S) -> MessageSigningResult<Authorization> {
        serde_json::from_str(message_to_sign.as_ref())
            .map_err(|_| MessageSigningErrorKind::TypeValueMismatch.into())
            .context("Error deserializing EIP7702 authorization as JSON")
    }
}

impl EthMessage for Authorization {
    fn hash(&self) -> MessageSigningResult<H256> {
        let mut list = RlpList::new();
        list.append(&self.chain_id)
            .append(&self.address)
            .append(&self.nonce);

        let to_hash: Vec<_> = iter::once(EIP_7702_MAGIC_PREFIX)
            .chain(list.finish())
            .collect();

        let hash_data = keccak256(&to_hash);
        Ok(H256::try_from(hash_data.as_slice()).expect("Expected 32-byte hash"))
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::address::Address;
    use std::str::FromStr;
    use tw_encoding::hex::ToHex;
    use tw_keypair::ecdsa::secp256k1;
    use tw_keypair::traits::SigningKeyTrait;

    #[test]
    fn test_pre_hash() {
        let authorization = Authorization {
            chain_id: U256::from(1_u32),
            address: Address::from_str("0xB91aaa96B138A1B1D94c9df4628187132c5F2bf1").unwrap(),
            nonce: U256::from(1_u32),
        };
        assert_eq!(
            Authorization::hash(&authorization).unwrap().to_hex(),
            "3ae543b2fa103a39a6985d964a67caed05f6b9bb2430ad6d498cda743fe911d9"
        );
    }

    #[test]
    fn test_sign_authorization() {
        let authorization = Authorization {
            chain_id: U256::from(1_u32),
            address: Address::from_str("0xB91aaa96B138A1B1D94c9df4628187132c5F2bf1").unwrap(),
            nonce: U256::from(1_u32),
        };
        let private_key = secp256k1::PrivateKey::try_from(
            "0x947dd69af402e7f48da1b845dfc1df6be593d01a0d8274bd03ec56712e7164e8",
        )
        .unwrap();
        let pre_hash = authorization.hash().unwrap();
        let signature = private_key.sign(pre_hash).unwrap();

        assert_eq!(
            signature.r().to_hex(),
            "2c39f2f64441dd38c364ee175dc6b9a87f34ca330bce968f6c8e22811e3bb710"
        );
        assert_eq!(
            signature.s().to_hex(),
            "5f1bcde93dee4b214e60bc0e63babcc13e4fecb8a23c4098fd89844762aa012c"
        );
        assert_eq!(signature.v(), 1);
    }
}
