// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::ripple_tx;
use crate::transaction::common_fields::CommonFields;
use serde::{Deserialize, Serialize};
use tw_encoding::hex::as_hex::AsHex;
use tw_hash::H256;

/// Accept offers to buy or sell an NFToken.
///
/// See NFTokenAcceptOffer:
/// `<https://xrpl.org/nftokenacceptoffer.html>`
#[derive(Clone, Debug, Deserialize, Eq, PartialEq, Serialize)]
#[serde(rename_all = "PascalCase")]
pub struct NFTokenAcceptOffer {
    /// The base fields for all transaction models.
    ///
    /// See Transaction Types:
    /// `<https://xrpl.org/transaction-types.html>`
    ///
    /// See Transaction Common Fields:
    /// `<https://xrpl.org/transaction-common-fields.html>`
    /// The type of transaction.
    #[serde(flatten)]
    pub common_fields: CommonFields,
    /// Identifies the NFTokenOffer that offers to sell the NFToken.
    #[serde(rename = "NFTokenSellOffer")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub nftoken_sell_offer: Option<AsHex<H256>>,
    /// Identifies the NFTokenOffer that offers to buy the NFToken.
    #[serde(rename = "NFTokenBuyOffer")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub nftoken_buy_offer: Option<AsHex<H256>>,
}

ripple_tx!(NFTokenAcceptOffer);
