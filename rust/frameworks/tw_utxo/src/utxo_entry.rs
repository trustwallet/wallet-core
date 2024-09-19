// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::coin_context::CoinContext;
use tw_memory::Data;
use tw_proto::{deserialize, serialize, MessageRead, MessageWrite, ProtoResult};

pub trait UtxoEntry {
    type PsbtSigningInput<'a>: MessageRead<'a>;
    type PsbtSigningOutput: MessageWrite;
    type PsbtTransactionPlan: MessageWrite;

    fn sign_psbt(
        &self,
        coin: &dyn CoinContext,
        input: Self::PsbtSigningInput<'_>,
    ) -> Self::PsbtSigningOutput;

    fn plan_psbt(
        &self,
        coin: &dyn CoinContext,
        input: Self::PsbtSigningInput<'_>,
    ) -> Self::PsbtTransactionPlan;
}

pub trait UtxoEntryExt {
    fn sign_psbt(&self, coin: &dyn CoinContext, input: &[u8]) -> ProtoResult<Data>;

    fn plan_psbt(&self, coin: &dyn CoinContext, input: &[u8]) -> ProtoResult<Data>;
}

impl<T: UtxoEntry> UtxoEntryExt for T {
    fn sign_psbt(&self, coin: &dyn CoinContext, input: &[u8]) -> ProtoResult<Data> {
        let input: T::PsbtSigningInput<'_> = deserialize(input)?;
        let output = <Self as UtxoEntry>::sign_psbt(self, coin, input);
        serialize(&output)
    }

    fn plan_psbt(&self, coin: &dyn CoinContext, input: &[u8]) -> ProtoResult<Data> {
        let input: T::PsbtSigningInput<'_> = deserialize(input)?;
        let output = <Self as UtxoEntry>::plan_psbt(self, coin, input);
        serialize(&output)
    }
}
