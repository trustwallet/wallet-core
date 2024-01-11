// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

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
