// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use serde::Deserialize;
use tw_encoding::base64::Base64Encoded;

/// `solana_signTransaction` request payload without legacy fields.
/// https://docs.walletconnect.com/advanced/rpc-reference/solana-rpc#solana_signtransaction
#[derive(Deserialize)]
#[serde(rename_all = "camelCase")]
pub struct SignTransactionRequest {
    pub transaction: Base64Encoded,
}
