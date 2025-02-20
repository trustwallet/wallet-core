// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::modules::protobuf_builder::DecredProtobufBuilder;
use crate::modules::signing_request::DecredSigningRequestBuilder;
use crate::transaction::DecredTransaction;
use tw_bitcoin::context::BitcoinSigningContext;
use tw_bitcoin::modules::psbt_request::NoPsbtRequestBuilder;
use tw_coin_entry::error::prelude::SigningResult;
use tw_utxo::address::legacy::LegacyAddress;
use tw_utxo::context::{AddressPrefixes, UtxoContext};
use tw_utxo::fee::fee_estimator::StandardFeeEstimator;
use tw_utxo::script::Script;

pub struct DecredContext;

impl UtxoContext for DecredContext {
    type Address = LegacyAddress;
    type Transaction = DecredTransaction;
    type FeeEstimator = StandardFeeEstimator<DecredTransaction>;

    fn addr_to_script_pubkey(
        _addr: &Self::Address,
        _prefixes: AddressPrefixes,
    ) -> SigningResult<Script> {
        todo!()
    }
}

impl BitcoinSigningContext for DecredContext {
    type SigningRequestBuilder = DecredSigningRequestBuilder;
    type ProtobufBuilder = DecredProtobufBuilder;
    type PsbtRequestBuilder = NoPsbtRequestBuilder;
}
