// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use bitcoin::psbt::Psbt;
use std::marker::PhantomData;
use tw_coin_entry::error::prelude::*;
use tw_proto::BitcoinV2::Proto;
use tw_utxo::context::UtxoContext;
use tw_utxo::transaction::unsigned_transaction::UnsignedTransaction;

pub mod output_psbt;
pub mod standard_psbt_request_builder;
pub mod utxo_psbt;

pub trait PsbtRequestBuilder<Context: UtxoContext> {
    fn build(
        input: &Proto::SigningInput,
        psbt_input: &Proto::Psbt,
    ) -> SigningResult<PsbtRequest<Context>>;
}

pub struct PsbtRequest<Context: UtxoContext> {
    pub psbt: Psbt,
    pub unsigned_tx: UnsignedTransaction<Context::Transaction>,
    _phantom: PhantomData<Context>,
}

pub struct NoPsbtRequestBuilder;

impl<Context: UtxoContext> PsbtRequestBuilder<Context> for NoPsbtRequestBuilder {
    fn build(
        _input: &Proto::SigningInput,
        _psbt_input: &Proto::Psbt,
    ) -> SigningResult<PsbtRequest<Context>> {
        SigningError::err(SigningErrorType::Error_not_supported)
            .context("PSBT signing is not supported")
    }
}
