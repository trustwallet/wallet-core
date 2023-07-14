// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::coin_context::CoinContext;
use crate::error::SigningResult;
use std::marker::PhantomData;

#[derive(Clone)]
pub struct BuildSigningInputArgs {
    pub from: String,
    pub to: String,
    /// Base-10 decimal string.
    pub amount: String,
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
    ) -> SigningResult<Self::SigningInput>;
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
    ) -> SigningResult<Self::SigningInput> {
        panic!("`NoInputBuilder` should never be constructed")
    }
}
