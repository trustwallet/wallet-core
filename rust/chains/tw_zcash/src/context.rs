// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::modules::protobuf_builder::ZcashProtobufBuilder;
use crate::modules::signing_request::ZcashSigningRequestBuilder;
use crate::modules::zcash_fee_estimator::ZcashFeeEstimator;
use crate::transaction::ZcashTransaction;
use tw_bitcoin::context::{StandardBitcoinContext, BitcoinSigningContext};
use tw_bitcoin::modules::psbt_request::NoPsbtRequestBuilder;
use tw_coin_entry::error::prelude::SigningResult;
use tw_utxo::address::standard_bitcoin::StandardBitcoinAddress;
use tw_utxo::context::{AddressPrefixes, UtxoContext};
use tw_utxo::script::Script;

pub struct ZcashContext;

impl UtxoContext for ZcashContext {
    type Address = StandardBitcoinAddress;
    type Transaction = ZcashTransaction;
    type FeeEstimator = ZcashFeeEstimator;

    fn addr_to_script_pubkey(
        addr: &Self::Address,
        prefixes: AddressPrefixes,
    ) -> SigningResult<Script> {
        StandardBitcoinContext::addr_to_script_pubkey(addr, prefixes)
    }
}

impl BitcoinSigningContext for ZcashContext {
    type SigningRequestBuilder = ZcashSigningRequestBuilder;
    type ProtobufBuilder = ZcashProtobufBuilder;
    type PsbtRequestBuilder = NoPsbtRequestBuilder;
}
