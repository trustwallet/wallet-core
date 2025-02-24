// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::DecredAddress;
use crate::modules::protobuf_builder::DecredProtobufBuilder;
use crate::modules::signing_request::DecredSigningRequestBuilder;
use crate::transaction::DecredTransaction;
use tw_bitcoin::context::BitcoinSigningContext;
use tw_bitcoin::modules::psbt_request::NoPsbtRequestBuilder;
use tw_coin_entry::error::prelude::SigningResult;
use tw_hash::hasher::Hasher;
use tw_utxo::context::{AddressPrefixes, UtxoContext};
use tw_utxo::fee::fee_estimator::StandardFeeEstimator;
use tw_utxo::script::Script;

#[derive(Default)]
pub struct DecredContext;

impl UtxoContext for DecredContext {
    type Address = DecredAddress;
    type Transaction = DecredTransaction;
    type FeeEstimator = StandardFeeEstimator<DecredTransaction>;

    const PUBLIC_KEY_HASHER: Hasher = Hasher::Blake256ripemd;
    const TX_HASHER: Hasher = Hasher::Blake256;

    fn addr_to_script_pubkey(
        addr: &Self::Address,
        prefixes: AddressPrefixes,
    ) -> SigningResult<Script> {
        addr.to_script_pubkey(prefixes.p2pkh_prefix, prefixes.p2sh_prefix)
    }
}

impl BitcoinSigningContext for DecredContext {
    type SigningRequestBuilder = DecredSigningRequestBuilder;
    type ProtobufBuilder = DecredProtobufBuilder;
    type PsbtRequestBuilder = NoPsbtRequestBuilder;
}
