// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::rlp::list::RlpList;
use crate::transaction::authorization_list::{Authorization, SignedAuthorization};
use std::iter;
use tw_coin_entry::error::prelude::*;
use tw_hash::sha3::keccak256;
use tw_hash::H256;
use tw_keypair::ecdsa::secp256k1::PrivateKey;
use tw_keypair::traits::SigningKeyTrait;
use tw_number::U256;

pub const EIP_7702_MAGIC_PREFIX: u8 = 0x05;

pub struct AuthorizationSigner;

impl AuthorizationSigner {
    pub fn sign(
        private_key: &PrivateKey,
        authorization: Authorization,
    ) -> SigningResult<SignedAuthorization> {
        let hash = Self::get_authorization_hash(&authorization);
        let signature = private_key.sign(hash)?;

        Ok(SignedAuthorization {
            authorization,
            y_parity: signature.v(),
            r: U256::from_big_endian(signature.r()),
            s: U256::from_big_endian(signature.s()),
        })
    }

    fn get_authorization_hash(authorization: &Authorization) -> H256 {
        let mut list = RlpList::new();
        list.append(&authorization.chain_id)
            .append(&authorization.address)
            .append(&authorization.nonce);

        let to_hash: Vec<_> = iter::once(EIP_7702_MAGIC_PREFIX)
            .chain(list.finish())
            .collect();
        H256::try_from(keccak256(&to_hash).as_slice())
            .expect("keccak256 must return exactly 32 bytes")
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::address::Address;
    use std::str::FromStr;
    use tw_encoding::hex::ToHex;

    #[test]
    fn test_get_authorization_hash() {
        let authorization = Authorization {
            chain_id: U256::from(1_u32),
            address: Address::from_str("0xB91aaa96B138A1B1D94c9df4628187132c5F2bf1").unwrap(),
            nonce: U256::from(1_u32),
        };
        assert_eq!(
            AuthorizationSigner::get_authorization_hash(&authorization).to_hex(),
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
        let private_key = PrivateKey::try_from(
            "0x947dd69af402e7f48da1b845dfc1df6be593d01a0d8274bd03ec56712e7164e8",
        )
        .unwrap();
        let signed_authorization = AuthorizationSigner::sign(&private_key, authorization).unwrap();
        assert_eq!(
            signed_authorization.r.to_big_endian().to_hex(),
            "2c39f2f64441dd38c364ee175dc6b9a87f34ca330bce968f6c8e22811e3bb710"
        );
        assert_eq!(
            signed_authorization.s.to_big_endian().to_hex(),
            "5f1bcde93dee4b214e60bc0e63babcc13e4fecb8a23c4098fd89844762aa012c"
        );
        assert_eq!(signed_authorization.y_parity, 1);
    }
}
