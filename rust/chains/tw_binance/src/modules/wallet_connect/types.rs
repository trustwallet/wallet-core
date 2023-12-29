// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::address::BinanceAddress;
use crate::transaction::UnsignedTransaction;
use serde::{Deserialize, Serialize};
use tw_cosmos_sdk::modules::serializer::json_serializer::SignatureJson;
use tw_encoding::base64::Base64Encoded;

#[derive(Deserialize)]
pub struct SignAminoRequest {
    #[serde(rename = "signerAddress")]
    pub signer_address: Option<BinanceAddress>,
    #[serde(rename = "signDoc")]
    pub sign_doc: UnsignedTransaction,
}

#[derive(Serialize)]
pub struct SignAminoResponse {
    pub signature: SignatureJson,
    pub signed: UnsignedTransaction,
}
