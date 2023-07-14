// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::modules::input_builder::{BuildSigningInputArgs, InputBuilder};
use tw_proto::Ethereum::Proto;

pub struct EthInputBuilder;

impl InputBuilder for EthInputBuilder {
    type SigningInput = Proto::SigningInput<'static>;

    fn build_signing_input(
        &self,
        _coin: &dyn CoinContext,
        _args: BuildSigningInputArgs,
    ) -> Self::SigningInput {
        todo!()
    }
}
