// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::error::SigningResult;
use tw_coin_registry::coin_type::CoinType;
use tw_coin_registry::dispatcher::coin_dispatcher;
use tw_memory::Data;

/// Represents a message signer to sign regular or typed structured data for any blockchain.
pub struct MessageSigner;

impl MessageSigner {
    /// Signs a message.
    #[inline]
    pub fn sign_message(input: &[u8], coin: CoinType) -> SigningResult<Data> {
        let (ctx, entry) = coin_dispatcher(coin)?;
        entry.sign_message(&ctx, input)
    }

    /// Computes preimage hashes of a message.
    #[inline]
    pub fn message_preimage_hashes(input: &[u8], coin: CoinType) -> SigningResult<Data> {
        let (ctx, entry) = coin_dispatcher(coin)?;
        entry.message_preimage_hashes(&ctx, input)
    }

    /// Verifies a signature for a message.
    #[inline]
    pub fn verify_message(input: &[u8], coin: CoinType) -> SigningResult<bool> {
        let (ctx, entry) = coin_dispatcher(coin)?;
        entry.verify_message(&ctx, input)
    }
}
