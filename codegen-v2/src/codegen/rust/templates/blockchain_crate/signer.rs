// Copyright © 2017-{YEAR} Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::SigningResult;
use tw_coin_entry::signing_output_error;
use tw_proto::{BLOCKCHAIN}::Proto;

pub struct {BLOCKCHAIN}Signer;

impl {BLOCKCHAIN}Signer {
    pub fn sign(
        coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
    ) -> Proto::SigningOutput<'static> {
        Self::sign_impl(coin, input)
            .unwrap_or_else(|e| signing_output_error!(Proto::SigningOutput, e))
    }

    fn sign_impl(
        _coin: &dyn CoinContext,
        _input: Proto::SigningInput<'_>,
    ) -> SigningResult<Proto::SigningOutput<'static>> {
        todo!()
    }
}
