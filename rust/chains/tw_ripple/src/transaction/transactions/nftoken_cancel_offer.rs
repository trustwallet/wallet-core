// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::ripple_tx;
use crate::transaction::common_fields::CommonFields;
use crate::types::vector256::Vector256;
use serde::{Deserialize, Serialize};

/// Cancels existing token offers created using NFTokenCreateOffer.
///
/// See NFTokenCancelOffer:
/// `<https://xrpl.org/nftokencanceloffer.html>`
#[derive(Clone, Debug, Deserialize, Eq, PartialEq, Serialize)]
#[serde(rename_all = "PascalCase")]
pub struct NFTokenCancelOffer {
    /// See Transaction Common Fields:
    /// `<https://xrpl.org/transaction-common-fields.html>`
    #[serde(flatten)]
    pub common_fields: CommonFields,
    /// Lifetime issue
    /// An array of IDs of the NFTokenOffer objects to cancel (not the IDs of NFToken
    /// objects, but the IDs of the NFTokenOffer objects). Each entry must be a
    /// different object ID of an NFTokenOffer object; the transaction is invalid
    /// if the array contains duplicate entries.
    #[serde(rename = "NFTokenOffers")]
    pub nftoken_offers: Vector256,
}

ripple_tx!(NFTokenCancelOffer);
