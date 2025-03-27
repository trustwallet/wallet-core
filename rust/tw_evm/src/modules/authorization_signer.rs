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
