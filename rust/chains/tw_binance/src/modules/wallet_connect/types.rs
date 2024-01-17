// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::transaction::UnsignedTransaction;
use serde::Deserialize;

#[derive(Deserialize)]
pub struct SignAminoRequest {
    #[serde(rename = "signDoc")]
    pub sign_doc: UnsignedTransaction,
}
