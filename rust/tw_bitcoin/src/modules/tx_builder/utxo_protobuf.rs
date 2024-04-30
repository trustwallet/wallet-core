// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::error::prelude::SigningResult;
use tw_memory::Data;
use tw_proto::BitcoinV3::Proto;
use tw_utxo::script::Script;
use tw_utxo::sighash_computer::UtxoToSign;
use tw_utxo::transaction::standard_transaction::builder::UtxoBuilder;
use tw_utxo::transaction::standard_transaction::TransactionInput;

pub struct UtxoProtobuf;

impl UtxoProtobuf {
    pub fn utxo_from_proto(_input: &Proto::Input) -> SigningResult<(TransactionInput, UtxoToSign)> {
        todo!()
    }

    pub fn p2sh(
        input: &Proto::Input,
        redeem_script: Data,
    ) -> SigningResult<(TransactionInput, UtxoToSign)> {
        let redeem_script = Script::from(redeem_script);

        let (utxo, arg) = UtxoBuilder::new()
            .prev_txid(input.txid.as_ref().try_into().unwrap())
            .prev_index(input.vout)
            .amount(input.value as i64)
            .p2sh(redeem_script)
            // TODO refactor `tw_utxo` to return `SigningError`.
            .unwrap();

        Ok((utxo, arg))
    }
}
