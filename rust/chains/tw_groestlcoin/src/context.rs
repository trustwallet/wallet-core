// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::GroestlAddress;
use tw_bitcoin::context::BitcoinSigningContext;
use tw_bitcoin::modules::protobuf_builder::standard_protobuf_builder::StandardProtobufBuilder;
use tw_bitcoin::modules::psbt_request::standard_psbt_request_builder::StandardPsbtRequestBuilder;
use tw_bitcoin::modules::signing_request::standard_signing_request::StandardSigningRequestBuilder;
use tw_coin_entry::error::prelude::SigningResult;
use tw_hash::hasher::Hasher;
use tw_hash::hasher::Hasher::Sha256;
use tw_utxo::context::{AddressPrefixes, UtxoContext};
use tw_utxo::fee::fee_estimator::StandardFeeEstimator;
use tw_utxo::script::Script;
use tw_utxo::transaction::standard_transaction::Transaction;

#[derive(Default)]
pub struct GroestlContext;

impl UtxoContext for GroestlContext {
    type Address = GroestlAddress;
    type Transaction = Transaction;
    type FeeEstimator = StandardFeeEstimator<Self::Transaction>;

    /// Groestlcoin uses a different hash algorithm.
    const TX_HASHER: Hasher = Sha256;

    fn addr_to_script_pubkey(
        addr: &Self::Address,
        prefixes: AddressPrefixes,
    ) -> SigningResult<Script> {
        match addr {
            GroestlAddress::Legacy(legacy) => {
                legacy.to_script_pubkey(prefixes.p2pkh_prefix, prefixes.p2sh_prefix)
            },
            GroestlAddress::Segwit(segwit) => segwit.to_script_pubkey(),
        }
    }
}

impl BitcoinSigningContext for GroestlContext {
    type SigningRequestBuilder = StandardSigningRequestBuilder;
    type ProtobufBuilder = StandardProtobufBuilder;
    type PsbtRequestBuilder = StandardPsbtRequestBuilder;
}
