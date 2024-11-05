// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::prelude::*;
use tw_proto::Polkadot::Proto;
use tw_substrate::*;

use crate::call_encoder::CallEncoder;

pub struct TxBuilder;

impl TxBuilder {
    pub fn unsigned_tx_from_proto(
        _coin: &dyn CoinContext,
        input: &Proto::SigningInput<'_>,
    ) -> SigningResult<UnsignedTransaction> {
        let call = CallEncoder::encode_input(&input)?;
        let era = match &input.era {
            Some(era) => Era::mortal(era.period, era.block_number),
            None => Era::immortal(),
        };
        let additional = AdditionalSigned {
            spec_version: input.spec_version,
            tx_version: input.transaction_version,
            genesis_hash: input
                .genesis_hash
                .as_ref()
                .try_into()
                .map_err(|_| SigningErrorType::Error_input_parse)?,
            current_hash: input
                .block_hash
                .as_ref()
                .try_into()
                .map_err(|_| SigningErrorType::Error_input_parse)?,
        };
        let extra = Extra::new(era, input.nonce as u32);
        Ok(UnsignedTransaction::new(additional, extra, call))
    }
}
