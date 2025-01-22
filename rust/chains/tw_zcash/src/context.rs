// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::modules::protobuf_builder::ZcashProtobufBuilder;
use crate::modules::signing_request::ZcashSigningRequestBuilder;
use crate::modules::zcash_fee_estimator::ZcashFeeEstimator;
use crate::t_address::TAddress;
use crate::transaction::ZcashTransaction;
use tw_bitcoin::context::BitcoinSigningContext;
use tw_bitcoin::modules::psbt_request::NoPsbtRequestBuilder;
use tw_coin_entry::error::prelude::SigningResult;
use tw_utxo::context::{AddressPrefixes, UtxoContext};
use tw_utxo::script::Script;

#[derive(Default)]
pub struct ZcashContext;

impl UtxoContext for ZcashContext {
    type Address = TAddress;
    type Transaction = ZcashTransaction;
    type FeeEstimator = ZcashFeeEstimator;

    fn addr_to_script_pubkey(
        addr: &Self::Address,
        prefixes: AddressPrefixes,
    ) -> SigningResult<Script> {
        addr.to_script_pubkey(prefixes.p2pkh_prefix, prefixes.p2sh_prefix)
    }
}

impl BitcoinSigningContext for ZcashContext {
    type SigningRequestBuilder = ZcashSigningRequestBuilder;
    type ProtobufBuilder = ZcashProtobufBuilder;
    type PsbtRequestBuilder = NoPsbtRequestBuilder;
}
