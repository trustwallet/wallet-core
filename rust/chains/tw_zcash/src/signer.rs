// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::signing_output_error;
use tw_proto::BitcoinV2::Proto as BitcoinV2Proto;

pub struct ZcashSigner;

impl ZcashSigner {
    pub fn sign(
        coin: &dyn CoinContext,
        input: BitcoinV2Proto::SigningInput<'_>,
    ) -> BitcoinV2Proto::SigningOutput<'static> {
        Self::sign_impl(coin, input)
            .unwrap_or_else(|e| signing_output_error!(BitcoinV2Proto::SigningOutput, e))
    }

    fn sign_impl(
        _coin: &dyn CoinContext,
        _input: BitcoinV2Proto::SigningInput<'_>,
    ) -> SigningResult<BitcoinV2Proto::SigningOutput<'static>> {
        todo!()
    }
}
