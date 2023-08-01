// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_coin_entry::error::{SigningError, SigningResult};
use tw_coin_registry::coin_type::CoinType;
use tw_coin_registry::dispatcher::coin_dispatcher;
use tw_memory::Data;

/// Represents a signer to sign transactions for any blockchain.
pub struct AnySigner;

impl AnySigner {
    /// Signs a transaction specified by the signing input and coin type.
    #[inline]
    pub fn sign(input: &[u8], coin: CoinType) -> SigningResult<Data> {
        let (ctx, entry) = coin_dispatcher(coin)?;
        entry.sign(&ctx, input).map_err(SigningError::from)
    }

    /// Planning, for UTXO chains, in preparation for signing
    /// It is optional, only UTXO chains need it, default impl. leaves empty result.
    #[inline]
    pub fn plan(input: &[u8], coin: CoinType) -> SigningResult<Data> {
        let (ctx, entry) = coin_dispatcher(coin)?;
        entry.plan(&ctx, input)
    }
}
