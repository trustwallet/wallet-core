// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::abi::contract::Contract;
use crate::abi::prebuild::biz::batch_calls_into_array_token;
use crate::abi::prebuild::ExecuteArgs;
use crate::abi::token::Token;
use crate::abi::{AbiErrorKind, AbiResult};
use lazy_static::lazy_static;
use tw_coin_entry::error::prelude::{OrTWError, ResultContext};
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

    pub fn remove_session(session_passkey_public_key: H520) -> AbiResult<Data> {
        let func = BIZ_PASSKEY_SESSION_ACCOUNT.function("removeSession")?;
        func.encode_input(&[Token::Bytes(session_passkey_public_key.to_vec())])
    }

    pub fn execute_with_passkey_session<I>(
        executions: I,
        validity_timestamp: U256,
        signature: Data,
    ) -> AbiResult<Data>
    where
        I: IntoIterator<Item = ExecuteArgs>,
    {
        let func = BIZ_PASSKEY_SESSION_ACCOUNT.function("executeWithPasskeySession")?;

        // `tuple[]`, where each item is a tuple of (address, uint256, bytes).
        let array_param = func
            .inputs
            .first()
            .or_tw_err(AbiErrorKind::Error_internal)
            .context("'Biz.execute4337Ops()' should contain only one argument")?;
        let array_token = batch_calls_into_array_token(array_param, executions)?;

        func.encode_input(&[
            array_token,
            Token::u256(validity_timestamp),
            Token::Bytes(signature),
        ])
    }
}
