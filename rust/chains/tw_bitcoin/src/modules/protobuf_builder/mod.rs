// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_utxo::context::UtxoContext;

pub use tw_proto::BitcoinV2::Proto::mod_SigningOutput::OneOftransaction as ProtobufTransaction;

pub mod standard_protobuf_builder;

pub trait ProtobufBuilder<Context: UtxoContext> {
    fn tx_to_proto(tx: &Context::Transaction) -> ProtobufTransaction<'static>;
}
