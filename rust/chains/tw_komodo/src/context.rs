// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_bitcoin::context::BitcoinSigningContext;
use tw_bitcoin::modules::psbt_request::NoPsbtRequestBuilder;
use tw_coin_entry::error::prelude::SigningResult;
use tw_utxo::address::legacy::LegacyAddress;
use tw_utxo::context::{AddressPrefixes, UtxoContext};
use tw_utxo::script::Script;
use tw_zcash::modules::protobuf_builder::ZcashProtobufBuilder;
use tw_zcash::modules::signing_request::ZcashSigningRequestBuilder;
use tw_zcash::modules::zcash_fee_estimator::ZcashFeeEstimator;
use tw_zcash::transaction::ZcashTransaction;

/// Komodo chain has a similar context to ZCash but with a legacy address format only.
#[derive(Default)]
pub struct KomodoContext;

impl UtxoContext for KomodoContext {
    type Address = LegacyAddress;
    type Transaction = ZcashTransaction;
    type FeeEstimator = ZcashFeeEstimator;

    fn addr_to_script_pubkey(
        addr: &Self::Address,
        prefixes: AddressPrefixes,
    ) -> SigningResult<Script> {
        addr.to_script_pubkey(prefixes.p2pkh_prefix, prefixes.p2sh_prefix)
    }
}

impl BitcoinSigningContext for KomodoContext {
    type SigningRequestBuilder = ZcashSigningRequestBuilder;
    type ProtobufBuilder = ZcashProtobufBuilder;
    type PsbtRequestBuilder = NoPsbtRequestBuilder;
}
