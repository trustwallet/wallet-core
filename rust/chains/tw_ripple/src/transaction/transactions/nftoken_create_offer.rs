// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::classic_address::ClassicAddress;
use crate::ripple_tx;
use crate::transaction::common_fields::CommonFields;
use crate::types::amount::Amount;
use serde::{Deserialize, Serialize};
use tw_encoding::hex::as_hex;
use tw_hash::H256;

/// Creates either a new Sell offer for an NFToken owned by
/// the account executing the transaction, or a new Buy
/// offer for an NFToken owned by another account.
///
/// See NFTokenCreateOffer:
/// `<https://xrpl.org/nftokencreateoffer.html>`
#[derive(Clone, Debug, Deserialize, Eq, PartialEq, Serialize)]
#[serde(rename_all = "PascalCase")]
pub struct NFTokenCreateOffer {
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
    /// Identifies the NFToken object that the offer references.
    #[serde(rename = "NFTokenID")]
    #[serde(with = "as_hex")]
    pub nftoken_id: H256,
    /// Indicates the amount expected or offered for the corresponding NFToken.
    /// The amount must be non-zero, except where this is an offer to sell and the
    /// asset is XRP; then, it is legal to specify an amount of zero, which means
    /// that the current owner of the token is giving it away, gratis, either to
    /// anyone at all, or to the account identified by the Destination field.
    pub amount: Amount,
    /// Who owns the corresponding NFToken. If the offer is to buy a token, this field
    /// must be present and it must be different than the Account field (since an offer
    /// to buy a token one already holds is meaningless). If the offer is to sell a token,
    /// this field must not be present, as the owner is, implicitly, the same as the
    /// Account (since an offer to sell a token one doesn't already hold is meaningless)
    #[serde(skip_serializing_if = "Option::is_none")]
    pub owner: Option<ClassicAddress>,
    /// Time after which the offer is no longer active, in seconds since the Ripple Epoch.
    #[serde(skip_serializing_if = "Option::is_none")]
    pub expiration: Option<u32>,
    /// If present, indicates that this offer may only be accepted by the specified account.
    /// Attempts by other accounts to accept this offer MUST fail.
    #[serde(skip_serializing_if = "Option::is_none")]
    pub destination: Option<ClassicAddress>,
}

ripple_tx!(NFTokenCreateOffer);
