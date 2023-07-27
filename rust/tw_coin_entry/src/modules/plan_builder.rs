// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::coin_context::CoinContext;
use tw_proto::{DummyMessage, MessageRead, MessageWrite, NoMessage};

pub trait PlanBuilder {
    type SigningInput<'a>: MessageRead<'a>;
    type Plan: MessageWrite;

    /// Planning, for UTXO chains, in preparation for signing.
    fn plan(&self, coin: &dyn CoinContext, input: Self::SigningInput<'_>) -> Self::Plan;
}

/// `NoInputBuilder` can't be created since there are no enum variants.
pub enum NoPlanBuilder {}

impl PlanBuilder for NoPlanBuilder {
    type SigningInput<'a> = DummyMessage;
    type Plan = NoMessage;

    /// [`PlanBuilder::plan`] should never be called.
    fn plan(&self, _coin: &dyn CoinContext, _input: Self::SigningInput<'_>) -> Self::Plan {
        panic!("`NoPlanBuilder` should never be constructed and used")
    }
}
