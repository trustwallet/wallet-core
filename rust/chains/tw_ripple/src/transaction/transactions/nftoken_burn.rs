// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::classic_address::ClassicAddress;
use crate::ripple_tx;
use crate::transaction::common_fields::CommonFields;
use serde::{Deserialize, Serialize};
use tw_encoding::hex::as_hex;
use tw_hash::H256;

/// Removes a NFToken object from the NFTokenPage in which it is being held,
/// effectively removing the token from the ledger (burning it).
///
/// See NFTokenBurn:
/// `<https://xrpl.org/nftokenburn.html>`
#[derive(Clone, Debug, Deserialize, Eq, PartialEq, Serialize)]
#[serde(rename_all = "PascalCase")]
pub struct NFTokenBurn {
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
    /// The NFToken to be removed by this transaction.
    #[serde(rename = "NFTokenID")]
    #[serde(with = "as_hex")]
    pub nftoken_id: H256,
    /// The owner of the NFToken to burn. Only used if that owner is
    /// different than the account sending this transaction. The
    /// issuer or authorized minter can use this field to burn NFTs
    /// that have the lsfBurnable flag enabled.
    #[serde(skip_serializing_if = "Option::is_none")]
    pub owner: Option<ClassicAddress>,
}

ripple_tx!(NFTokenBurn);
