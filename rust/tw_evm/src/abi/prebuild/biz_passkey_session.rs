// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::abi::contract::Contract;
use crate::abi::token::Token;
use crate::abi::AbiResult;
use lazy_static::lazy_static;
use tw_hash::H520;
use tw_memory::Data;
use tw_number::U256;

const BIZ_PASSKEY_SESSION_ACCOUNT_ABI: &str = include_str!("resource/biz_passkey_session.abi.json");

lazy_static! {
    static ref BIZ_PASSKEY_SESSION_ACCOUNT: Contract =
        serde_json::from_str(BIZ_PASSKEY_SESSION_ACCOUNT_ABI).unwrap();
}

pub struct BizPasskeySessionAccount;

impl BizPasskeySessionAccount {
    pub fn register_session(
        session_passkey_public_key: H520,
        valid_until_timestamp: U256,
    ) -> AbiResult<Data> {
        let func = BIZ_PASSKEY_SESSION_ACCOUNT.function("registerSession")?;
        func.encode_input(&[
            Token::Bytes(session_passkey_public_key.to_vec()),
            Token::u256(valid_until_timestamp),
        ])
    }
}
