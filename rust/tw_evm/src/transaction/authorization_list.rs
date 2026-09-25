// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::Address;
use crate::rlp::buffer::RlpBuffer;
use crate::rlp::RlpEncode;
use serde::{Deserialize, Serialize};
use tw_encoding::hex::u8_as_hex;
use tw_number::U256;

/// Authorization for 7702 txn support.
#[derive(Serialize, Deserialize, Clone)]
#[serde(rename_all = "camelCase")]
pub struct Authorization {
    /// The chain ID of the authorization.
    #[serde(deserialize_with = "U256::from_hex_or_decimal_str")]
    #[serde(serialize_with = "U256::as_hex")]
    pub chain_id: U256,
    /// The address of the authorization.
    pub address: Address,
    /// The nonce for the authorization.
    #[serde(deserialize_with = "U256::from_hex_or_decimal_str")]
    #[serde(serialize_with = "U256::as_hex")]
    pub nonce: U256,
}

/// Signed authorization for 7702 txn support.
/// See: https://eips.ethereum.org/EIPS/eip-4337#support-for-eip-7702-authorizations
#[derive(Serialize, Deserialize, Clone)]
#[serde(rename_all = "camelCase")]
pub struct SignedAuthorization {
    #[serde(flatten)]
    pub authorization: Authorization,
    /// y-parity of the signature.
    #[serde(with = "u8_as_hex")]
    pub y_parity: u8,
    /// r part of the signature.
    #[serde(deserialize_with = "U256::from_hex_or_decimal_str")]
    #[serde(serialize_with = "U256::as_hex")]
    pub r: U256,
    /// s part of the signature.
    #[serde(deserialize_with = "U256::from_hex_or_decimal_str")]
    #[serde(serialize_with = "U256::as_hex")]
    pub s: U256,
}

impl RlpEncode for SignedAuthorization {
    fn rlp_append(&self, buf: &mut RlpBuffer) {
        buf.begin_list();

        self.authorization.chain_id.rlp_append(buf);
        self.authorization.address.rlp_append(buf);
        self.authorization.nonce.rlp_append(buf);
        self.y_parity.rlp_append(buf);
        self.r.rlp_append(buf);
        self.s.rlp_append(buf);

        buf.finalize_list();
    }
}

/// [EIP-7702](https://eips.ethereum.org/EIPS/eip-7702) authorization list.
#[derive(Default)]
pub struct AuthorizationList(pub(crate) Vec<SignedAuthorization>);

impl From<Vec<SignedAuthorization>> for AuthorizationList {
    fn from(value: Vec<SignedAuthorization>) -> Self {
        AuthorizationList(value)
    }
}

impl RlpEncode for AuthorizationList {
    fn rlp_append(&self, buf: &mut RlpBuffer) {
        buf.begin_list();

        for access in self.0.iter() {
            access.rlp_append(buf);
        }

        buf.finalize_list();
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::str::FromStr;
    use tw_encoding::hex::ToHex;

    #[test]
    fn test_encode_signed_authorization() {
        let authorization = SignedAuthorization {
            authorization: Authorization {
                chain_id: U256::from(123_u32),
                address: Address::from_str("0x0101010101010101010101010101010101010101").unwrap(),
                nonce: U256::from(321_u32),
            },
            y_parity: 3,
            r: U256::from(222_u32),
            s: U256::from(333_u32),
        };

        let mut buf = RlpBuffer::new();
        authorization.rlp_append(&mut buf);
        let encoded = buf.finish();
        assert_eq!(
            encoded.to_hex(),
            "df7b9401010101010101010101010101010101010101018201410381de82014d"
        );
    }
}
