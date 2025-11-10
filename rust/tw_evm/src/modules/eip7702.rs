// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::Address;
use crate::message::EthMessage;
use crate::modules::barz::error::BarzResult;
use crate::transaction::authorization_list::{Authorization, SignedAuthorization};
use std::str::FromStr;
use tw_keypair::ecdsa::secp256k1::PrivateKey;
use tw_keypair::traits::SigningKeyTrait;
use tw_number::U256;

pub fn get_authorization_hash(
    chain_id: &[u8],
    contract_address: &str,
    nonce: &[u8],
) -> BarzResult<Vec<u8>> {
    let authorization = Authorization {
        chain_id: U256::from_big_endian_slice(chain_id)?,
        address: Address::from_str(contract_address)?,
        nonce: U256::from_big_endian_slice(nonce)?,
    };

    Ok(authorization.hash()?.to_vec())
}

pub fn sign_authorization(
    chain_id: &[u8],
    contract_address: &str,
    nonce: &[u8],
    private_key: &str,
) -> BarzResult<String> {
    let authorization = Authorization {
        chain_id: U256::from_big_endian_slice(chain_id)?,
        address: Address::from_str(contract_address)?,
        nonce: U256::from_big_endian_slice(nonce)?,
    };
    let authorization_hash = authorization.hash()?;
    let private_key = PrivateKey::try_from(private_key)?;
    let signature = private_key.sign(authorization_hash)?;

    let signed_authorization = SignedAuthorization {
        authorization,
        y_parity: signature.v(),
        r: U256::from_big_endian(signature.r()),
        s: U256::from_big_endian(signature.s()),
    };

    Ok(serde_json::to_string(&signed_authorization)?)
}
