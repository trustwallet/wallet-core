// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::modules::tx_builder::output_protobuf::OutputProtobuf;
use crate::modules::tx_builder::public_keys::PublicKeys;
use crate::modules::tx_builder::utxo_protobuf::UtxoProtobuf;
use tw_coin_entry::error::prelude::*;
use tw_proto::BitcoinV3::Proto;
use tw_utxo::dust::DustPolicy;
use tw_utxo::modules::utxo_selector::InputSelector;
use tw_utxo::transaction::standard_transaction::builder::TransactionBuilder;
use tw_utxo::transaction::standard_transaction::{Transaction, TransactionOutput};
use tw_utxo::transaction::transaction_parts::Amount;
use tw_utxo::transaction::unsigned_transaction::UnsignedTransaction;
use Proto::mod_SigningInput::OneOfdust_policy as ProtoDustPolicy;

const TX_VERSION: u32 = 2;

pub struct SigningRequest {
    pub ty: RequestType,
    pub dust_policy: DustPolicy,
    pub fee_per_vbyte: Amount,
}

pub enum RequestType {
    SendMax {
        unsigned_tx: UnsignedTransaction<Transaction>,
    },
    SendExact {
        unsigned_tx: UnsignedTransaction<Transaction>,
        change_output: Option<TransactionOutput>,
        input_selector: InputSelector,
    },
}

impl SigningRequest {
    pub fn from_proto(input: &Proto::SigningInput) -> SigningResult<SigningRequest> {
        let chain_info = input
            .chain_info
            .as_ref()
            .or_tw_err(SigningErrorType::Error_invalid_params)
            .context("No chain info specified")?;

        let dust_policy = Self::dust_policy(&input.dust_policy)?;

        let fee_per_vbyte: Amount = input
            .fee_per_vb
            .try_into()
            .tw_err(|_| SigningErrorType::Error_invalid_params)
            .context("Invalid feePerVb, it must fit int64")?;

        let public_keys = Self::get_public_keys(input)?;

        let mut builder = TransactionBuilder::default();
        builder.version(TX_VERSION).lock_time(input.lock_time);

        // Parse all UTXOs.
        for utxo_proto in input.inputs.iter() {
            let utxo_builder = UtxoProtobuf::new(chain_info, utxo_proto, &public_keys);

            let (utxo, utxo_args) = utxo_builder
                .utxo_from_proto()
                .context("Error creating UTXO from Protobuf")?;
            builder.push_input(utxo, utxo_args);
        }

        // If `max_amount_output` is set, construct a transaction with only one output.
        if let Some(max_output_proto) = input.max_amount_output.as_ref() {
            let output_builder = OutputProtobuf::new(chain_info, max_output_proto);

            let max_output = output_builder
                .output_from_proto()
                .context("Error creating Max Output from Protobuf")?;
            builder.push_output(max_output);

            let unsigned_tx = builder.build()?;
            return Ok(SigningRequest {
                ty: RequestType::SendMax { unsigned_tx },
                dust_policy,
                fee_per_vbyte,
            });
        }

        // `max_amount_output` isn't set, parse all Outputs.
        for output_proto in input.outputs.iter() {
            let output = OutputProtobuf::new(chain_info, output_proto)
                .output_from_proto()
                .context("Error creating Output from Proto")?;
            builder.push_output(output);
        }

        // Parse change output if it was provided.
        let change_output = input
            .change_output
            .as_ref()
            .map(|change_output_proto| {
                OutputProtobuf::new(chain_info, change_output_proto)
                    .output_from_proto()
                    .context("Error creating Change Output from Proto")
            })
            .transpose()?;

        let input_selector = Self::input_selector(&input.input_selector);

        let unsigned_tx = builder.build()?;
        Ok(SigningRequest {
            ty: RequestType::SendExact {
                unsigned_tx,
                change_output,
                input_selector,
            },
            dust_policy,
            fee_per_vbyte,
        })
    }

    fn get_public_keys(input: &Proto::SigningInput) -> SigningResult<PublicKeys> {
        let mut public_keys = PublicKeys::default();

        if input.private_keys.is_empty() {
            for public in input.public_keys.iter() {
                public_keys.add_public_key(public.to_vec());
            }
        } else {
            for private in input.private_keys.iter() {
                public_keys.add_public_with_ecdsa_private(private)?;
            }
        }

        Ok(public_keys)
    }

    fn input_selector(selector: &Proto::InputSelector) -> InputSelector {
        match selector {
            Proto::InputSelector::SelectAscending => InputSelector::Ascending,
            Proto::InputSelector::SelectInOrder => InputSelector::InOrder,
            Proto::InputSelector::SelectDescending => InputSelector::Descending,
            Proto::InputSelector::UseAll => InputSelector::UseAll,
        }
    }

    fn dust_policy(proto: &ProtoDustPolicy) -> SigningResult<DustPolicy> {
        match proto {
            ProtoDustPolicy::fixed_dust_threshold(fixed) => Ok(DustPolicy::FixedAmount(*fixed)),
            ProtoDustPolicy::None => SigningError::err(SigningErrorType::Error_invalid_params)
                .context("No dust policy provided"),
        }
    }
}
