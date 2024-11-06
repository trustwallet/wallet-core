// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::signer::network_id_from_tw;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::prelude::*;
use tw_proto::Polkadot::Proto;
use tw_ss58_address::NetworkId;
use tw_substrate::*;

use crate::call_encoder::CallEncoder;

fn require_check_metadata(network_id: NetworkId, spec_version: u32) -> bool {
    match network_id {
        NetworkId::POLKADOT | NetworkId::KUSAMA if spec_version == 1_002_005 => true,
        _ => false,
    }
}

pub struct TxBuilder;

impl TxBuilder {
    pub fn unsigned_tx_from_proto(
        _coin: &dyn CoinContext,
        input: &Proto::SigningInput<'_>,
    ) -> SigningResult<UnsignedTransaction> {
        let network_id = network_id_from_tw(&input)?;
        let check_metadata = require_check_metadata(network_id, input.spec_version);
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
        let (additional, extra) = if check_metadata {
            // For now disable CheckMetadata extension.
            (
                Encoded::new(additional.with_check_metadata(None)),
                Encoded::new(extra.with_check_metadata(false)),
            )
        } else {
            (Encoded::new(additional), Encoded::new(extra))
        };
        Ok(UnsignedTransaction::new(additional, extra, call))
    }
}
