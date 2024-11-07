// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::prelude::*;
use tw_number::U256;
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
        ctx: &SubstrateContext,
        _coin: &dyn CoinContext,
        input: &Proto::SigningInput<'_>,
    ) -> SigningResult<PrepareTransaction> {
        let encoder = CallEncoder::from_ctx(ctx)?;
        let call = encoder.encode_call(&input.message_oneof)?;
        let check_metadata = require_check_metadata(ctx.network, input.spec_version);
        let era = match &input.era {
            Some(era) => Era::mortal(era.period, era.block_number),
            None => Era::immortal(),
        };
        let genesis_hash = input
            .genesis_hash
            .as_ref()
            .try_into()
            .map_err(|_| SigningErrorType::Error_input_parse)?;
        let current_hash = input
            .block_hash
            .as_ref()
            .try_into()
            .map_err(|_| SigningErrorType::Error_input_parse)?;
        let tip = U256::from_big_endian_slice(&input.tip)
            .map_err(|_| EncodeError::InvalidValue)?
            .try_into()
            .map_err(|_| EncodeError::InvalidValue)?;

        let mut builder = TransactionBuilder::new(call);
        // Add chain extensions.
        builder.extension(CheckVersion(input.spec_version));
        builder.extension(CheckVersion(input.transaction_version));
        builder.extension(CheckGenesis(genesis_hash));
        builder.extension(CheckEra { era, current_hash });
        builder.extension(CheckNonce::new(input.nonce as u32));
        if let Some(fee_asset_id) = ctx.fee_asset_id {
            builder.extension(ChargeAssetTxPayment::new(tip, fee_asset_id));
        } else {
            builder.extension(ChargeTransactionPayment::new(tip));
        }
        if check_metadata {
            builder.extension(CheckMetadataHash::default());
        }
        Ok(builder.build())
    }
}
