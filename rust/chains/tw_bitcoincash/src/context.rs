// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::Address;
use tw_bitcoin::context::BitcoinSigningContext;
use tw_bitcoin::modules::protobuf_builder::standard_protobuf_builder::StandardProtobufBuilder;
use tw_bitcoin::modules::psbt_request::standard_psbt_request_builder::StandardPsbtRequestBuilder;
use tw_bitcoin::modules::signing_request::standard_signing_request::StandardSigningRequestBuilder;
use tw_coin_entry::error::prelude::*;
use tw_utxo::context::{AddressPrefixes, UtxoContext};
use tw_utxo::fee::fee_estimator::StandardFeeEstimator;
use tw_utxo::script::Script;
use tw_utxo::transaction::standard_transaction::Transaction;

#[derive(Default)]
pub struct BitcoinCashContext;

impl UtxoContext for BitcoinCashContext {
    type Address = Address;
    type Transaction = Transaction;
    type FeeEstimator = StandardFeeEstimator<Transaction>;

    fn addr_to_script_pubkey(
        addr: &Self::Address,
        prefixes: AddressPrefixes,
    ) -> SigningResult<Script> {
        match addr {
            Address::Cash(cash) => cash
                .to_legacy(prefixes.p2pkh_prefix, prefixes.p2sh_prefix)
                .into_tw()
                .context("Error converting 'CashAddress' to 'LegacyAddress'")?
                .to_script_pubkey(prefixes.p2pkh_prefix, prefixes.p2sh_prefix),
            Address::Legacy(legacy) => {
                legacy.to_script_pubkey(prefixes.p2pkh_prefix, prefixes.p2sh_prefix)
            },
        }
    }
}

impl BitcoinSigningContext for BitcoinCashContext {
    type SigningRequestBuilder = StandardSigningRequestBuilder;
    type ProtobufBuilder = StandardProtobufBuilder;
    type PsbtRequestBuilder = StandardPsbtRequestBuilder;
}
