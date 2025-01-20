// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::context::ZcashContext;
use crate::modules::zcash_fee_estimator::ZcashFeeEstimator;
use crate::transaction::TRANSACTION_VERSION_4;
use tw_bitcoin::modules::signing_request::SigningRequestBuilder;
use tw_bitcoin::modules::tx_builder::output_protobuf::OutputProtobuf;
use tw_bitcoin::modules::tx_builder::utxo_protobuf::UtxoProtobuf;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::prelude::{
    MapTWError, OrTWError, ResultContext, SigningError, SigningErrorType, SigningResult,
};
use tw_proto::BitcoinV2::Proto;
use tw_proto::Zcash::Proto as ZcashProto;
use tw_utxo::fee::fee_estimator::StandardFeeEstimator;
use tw_utxo::fee::FeePolicy;
use tw_utxo::modules::tx_planner::{PlanRequest, RequestType};

use crate::modules::transaction_builder::ZcashTransactionBuilder;
use tw_hash::H32;
use Proto::mod_TransactionBuilder::OneOffee_policy as ProtoFeePolicy;

type SigningRequestBuilderImpl = SigningRequestBuilder<ZcashContext>;

pub type ZcashSigningRequest = PlanRequest<ZcashContext>;

pub struct ZcashExtraData {
    pub branch_id: H32,
    pub zip_0317: bool,
}

pub struct ZcashSigningRequestBuilder;

impl ZcashSigningRequestBuilder {
    pub fn build(
        coin: &dyn CoinContext,
        input: &Proto::SigningInput,
        transaction_builder: &Proto::TransactionBuilder,
    ) -> SigningResult<ZcashSigningRequest> {
        let extra_data = Self::extra_data(&transaction_builder.zcash_extra_data)?;

        let chain_info = SigningRequestBuilderImpl::chain_info(coin, &input.chain_info)?;
        let dust_policy = SigningRequestBuilderImpl::dust_policy(&transaction_builder.dust_policy)?;
        let fee_estimator = Self::fee_estimator(&transaction_builder.fee_policy, &extra_data)?;
        let version = Self::transaction_version(&transaction_builder.version)?;

        let public_keys = SigningRequestBuilderImpl::get_public_keys(input)?;

        let mut builder = ZcashTransactionBuilder::default();
        builder
            .version(version)
            .lock_time(transaction_builder.lock_time)
            .branch_id(extra_data.branch_id);

        // Parse all UTXOs.
        for utxo_proto in transaction_builder.inputs.iter() {
            let utxo_builder =
                UtxoProtobuf::<ZcashContext>::new(&chain_info, utxo_proto, &public_keys);

            let (utxo, utxo_args) = utxo_builder
                .utxo_from_proto()
                .context("Error creating UTXO from Protobuf")?;
            builder.push_input(utxo, utxo_args);
        }

        // If `max_amount_output` is set, construct a transaction with only one output.
        if let Some(max_output_proto) = transaction_builder.max_amount_output.as_ref() {
            let output_builder = OutputProtobuf::<ZcashContext>::new(&chain_info, max_output_proto);

            let max_output = output_builder
                .output_from_proto()
                .context("Error creating Max Output from Protobuf")?;
            builder.push_output(max_output);

            let unsigned_tx = builder.build()?;
            return Ok(ZcashSigningRequest {
                ty: RequestType::SendMax { unsigned_tx },
                dust_policy,
                fee_estimator,
            });
        }

        // `max_amount_output` isn't set, parse all Outputs.
        for output_proto in transaction_builder.outputs.iter() {
            let output = OutputProtobuf::<ZcashContext>::new(&chain_info, output_proto)
                .output_from_proto()
                .context("Error creating Output from Proto")?;
            builder.push_output(output);
        }

        // Parse change output if it was provided.
        let change_output = transaction_builder
            .change_output
            .as_ref()
            .map(|change_output_proto| {
                OutputProtobuf::<ZcashContext>::new(&chain_info, change_output_proto)
                    .output_from_proto()
                    .context("Error creating Change Output from Proto")
            })
            .transpose()?;

        let input_selector =
            SigningRequestBuilderImpl::input_selector(&transaction_builder.input_selector);

        let unsigned_tx = builder.build()?;
        Ok(ZcashSigningRequest {
            ty: RequestType::SendExact {
                unsigned_tx,
                change_output,
                input_selector,
            },
            dust_policy,
            fee_estimator,
        })
    }

    pub fn transaction_version(proto: &Proto::TransactionVersion) -> SigningResult<i32> {
        match proto {
            Proto::TransactionVersion::UseDefault => Ok(TRANSACTION_VERSION_4),
            _ => SigningError::err(SigningErrorType::Error_invalid_params)
                .context("ZCash currently supports `UseDefault` transaction version only"),
        }
    }

    pub fn fee_estimator(
        proto: &ProtoFeePolicy,
        extra_data: &ZcashExtraData,
    ) -> SigningResult<ZcashFeeEstimator> {
        if extra_data.zip_0317 {
            return Ok(ZcashFeeEstimator::Zip0317);
        }

        let fee_per_vb = match proto {
            ProtoFeePolicy::fee_per_vb(fee_per_vb) => *fee_per_vb,
            ProtoFeePolicy::None => 0,
        };
        let standard = StandardFeeEstimator::new(FeePolicy::FeePerVb(fee_per_vb));
        Ok(ZcashFeeEstimator::Standard(standard))
    }

    pub fn extra_data(
        proto: &Option<ZcashProto::TransactionBuilderExtraData>,
    ) -> SigningResult<ZcashExtraData> {
        let extra_data = proto
            .as_ref()
            .or_tw_err(SigningErrorType::Error_invalid_params)
            .context("Expected 'TransactionBuilder.zcash_extra_data' to be set")?;

        let branch_id = H32::try_from(extra_data.branch_id.as_ref())
            .tw_err(|_| SigningErrorType::Error_invalid_params)
            .context("Invalid 'branchId', expected 4-byte array")?;
        Ok(ZcashExtraData {
            branch_id,
            zip_0317: extra_data.zip_0317,
        })
    }
}
