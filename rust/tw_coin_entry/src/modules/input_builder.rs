// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::coin_context::CoinContext;
use std::marker::PhantomData;
use tw_number::U256;

pub struct BuildSigningInputArgs {
    pub from: String,
    pub to: String,
    pub amount: U256,
    pub asset: String,
    pub memo: String,
    pub chain_id: String,
}

pub trait InputBuilder {
    type SigningInput;

    /// The helper function to prepare a `SigningInput` from simple parameters.
    /// Some parameters, like chain-specific fee/gas parameters, may need to be set in the `SigningInput`.
    fn build_signing_input(
        &self,
        coin: &dyn CoinContext,
        args: BuildSigningInputArgs,
    ) -> Self::SigningInput;
}

/// `NoInputBuilder` can't be created since there is no a public constructors.
pub struct NoInputBuilder<SigningInput> {
    _phantom: PhantomData<SigningInput>,
}

impl<SigningInput> InputBuilder for NoInputBuilder<SigningInput> {
    type SigningInput = SigningInput;

    fn build_signing_input(
        &self,
        _coin: &dyn CoinContext,
        _args: BuildSigningInputArgs,
    ) -> Self::SigningInput {
        panic!("`NoInputBuilder` should never be constructed")
    }
}
