// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::modules::protobuf_builder::standard_protobuf_builder::StandardProtobufBuilder;
use crate::modules::protobuf_builder::ProtobufBuilder;
use crate::modules::psbt_request::standard_psbt_request_builder::StandardPsbtRequestBuilder;
use crate::modules::psbt_request::PsbtRequestBuilder;
use crate::modules::signing_request::standard_signing_request::StandardSigningRequestBuilder;
use crate::modules::signing_request::SigningRequestBuilder;
use tw_coin_entry::error::prelude::SigningResult;
use tw_utxo::address::standard_bitcoin::StandardBitcoinAddress;
use tw_utxo::context::{AddressPrefixes, UtxoContext};
use tw_utxo::fee::fee_estimator::StandardFeeEstimator;
use tw_utxo::script::Script;
use tw_utxo::transaction::standard_transaction::Transaction;

/// A set of associated modules different from a chain to chain.
/// The modules are mainly used to generate a signing request from a [`BitcoinV2::Proto::SigningInput`],
/// and generate a [`Utxo::Proto::Transaction`] output when the transaction is signed.
pub trait BitcoinSigningContext: UtxoContext + Sized {
    type SigningRequestBuilder: SigningRequestBuilder<Self>;
    type ProtobufBuilder: ProtobufBuilder<Self>;
    type PsbtRequestBuilder: PsbtRequestBuilder<Self>;
}

#[derive(Default)]
pub struct StandardBitcoinContext;

impl UtxoContext for StandardBitcoinContext {
    type Address = StandardBitcoinAddress;
    type Transaction = Transaction;
    type FeeEstimator = StandardFeeEstimator<Self::Transaction>;

    fn addr_to_script_pubkey(
        addr: &Self::Address,
        prefixes: AddressPrefixes,
    ) -> SigningResult<Script> {
        match addr {
            StandardBitcoinAddress::Legacy(legacy) => {
                legacy.to_script_pubkey(prefixes.p2pkh_prefix, prefixes.p2sh_prefix)
            },
            StandardBitcoinAddress::Segwit(segwit) => segwit.to_script_pubkey(),
            StandardBitcoinAddress::Taproot(taproot) => taproot.to_script_pubkey(),
        }
    }
}

impl BitcoinSigningContext for StandardBitcoinContext {
    type SigningRequestBuilder = StandardSigningRequestBuilder;
    type ProtobufBuilder = StandardProtobufBuilder;
    type PsbtRequestBuilder = StandardPsbtRequestBuilder;
}
