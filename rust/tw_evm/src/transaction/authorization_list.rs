// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::Address;
use crate::rlp::buffer::RlpBuffer;
use crate::rlp::RlpEncode;
use tw_number::U256;

/// Authorization for 7702 txn support.
pub struct Authorization {
    /// The chain ID of the authorization.
    pub chain_id: U256,
    /// The address of the authorization.
    pub address: Address,
    /// The nonce for the authorization.
    pub nonce: U256,
}

/// Signed authorization for 7702 txn support.
pub struct SignedAuthorization {
    pub authorization: Authorization,
    /// y-parity of the signature.
    pub y_parity: u8,
    /// r part of the signature.
    pub r: U256,
    /// s part of the signature.
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

/// [EIP-2930](https://eips.ethereum.org/EIPS/eip-2930) access list.
#[derive(Default)]
pub struct AuthorizationList(Vec<SignedAuthorization>);

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
