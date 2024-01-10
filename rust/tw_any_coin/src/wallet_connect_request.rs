// Copyright © 2017-2024 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_coin_entry::error::{SigningError, SigningResult};
use tw_coin_registry::coin_type::CoinType;
use tw_coin_registry::dispatcher::coin_dispatcher;
use tw_memory::Data;

/// Represents a signer to sign transactions for any blockchain.
pub struct WalletConnectRequest;

impl WalletConnectRequest {
    /// Parses the WalletConnect signing request as a `SigningInput`.
    /// It is optional. Returns an error if the chain does not support WalletConnect signing.
    #[inline]
    pub fn parse(coin: CoinType, input: &[u8]) -> SigningResult<Data> {
        let (ctx, entry) = coin_dispatcher(coin)?;
        entry
            .wallet_connect_parse_request(&ctx, input)
            .map_err(SigningError::from)
    }
}
