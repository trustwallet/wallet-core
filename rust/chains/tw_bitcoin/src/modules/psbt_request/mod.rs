// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::marker::PhantomData;
use tw_coin_entry::error::prelude::*;
use tw_memory::Data;
use tw_proto::BitcoinV2::Proto;
use tw_utxo::context::UtxoContext;
use tw_utxo::transaction::unsigned_transaction::UnsignedTransaction;

pub mod output_psbt;
pub mod standard_psbt_request_handler;
pub mod utxo_psbt;

pub trait PsbtRequestHandler<Context: UtxoContext> {
    /// Parses a PSBT request from Protobuf.
    fn parse_request(
        input: &Proto::SigningInput,
        psbt_input: &Proto::Psbt,
    ) -> SigningResult<PsbtRequest<Context>>;

    /// Finalizes the [Partially Signed Bitcoin Transaction](Psbt)
    /// by updating the final `script_sig` and/or `witness`.
    fn update_signed(
        psbt: &mut Context::Psbt,
        signed_tx: &Context::Transaction,
    ) -> SigningResult<()>;

    /// Serializes the PSBT into bytes.
    fn serialize_psbt(psbt: &Context::Psbt) -> SigningResult<Data>;
}

pub struct PsbtRequest<Context: UtxoContext> {
    pub psbt: Context::Psbt,
    pub unsigned_tx: UnsignedTransaction<Context::Transaction>,
    pub _phantom: PhantomData<Context>,
}

pub struct NoPsbtRequestBuilder;

impl<Context: UtxoContext> PsbtRequestHandler<Context> for NoPsbtRequestBuilder {
    fn parse_request(
        _input: &Proto::SigningInput,
        _psbt_input: &Proto::Psbt,
    ) -> SigningResult<PsbtRequest<Context>> {
        SigningError::err(SigningErrorType::Error_not_supported)
            .context("PSBT signing is not supported")
    }

    fn update_signed(
        _psbt: &mut Context::Psbt,
        _signed_tx: &Context::Transaction,
    ) -> SigningResult<()> {
        SigningError::err(SigningErrorType::Error_not_supported)
            .context("PSBT signing is not supported")
    }

    fn serialize_psbt(_psbt: &Context::Psbt) -> SigningResult<Data> {
        SigningError::err(SigningErrorType::Error_not_supported)
            .context("PSBT signing is not supported")
    }
}
