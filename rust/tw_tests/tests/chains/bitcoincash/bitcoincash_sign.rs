// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::bitcoincash::test_cases::transfer_96ee20;
use crate::chains::common::bitcoin::{btc_info, sign, TransactionOneof};
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::DecodeHex;
use tw_proto::BitcoinV2::Proto;

/// Original test: https://github.com/trustwallet/wallet-core/blob/cd5a27481d2181e63362cb57e2b2160506cce163/tests/chains/BitcoinCash/TWBitcoinCashTests.cpp#L122-L164
/// *Note* that if we enable change output, current selection algorithm will calculate
/// different change and fee amounts than in the original test.
#[test]
fn test_bitcoincash_sign_input_p2pkh_from_to_address() {
    let signing = Proto::SigningInput {
        private_keys: vec![transfer_96ee20::PRIVATE_KEY.decode_hex().unwrap().into()],
        chain_info: btc_info(),
        transaction: TransactionOneof::builder(transfer_96ee20::transaction_builder()),
        ..Default::default()
    };

    // Successfully broadcasted:
    // https://blockchair.com/bitcoin-cash/transaction/96ee20002b34e468f9d3c5ee54f6a8ddaa61c118889c4f35395c2cd93ba5bbb4
    sign::BitcoinSignHelper::new(&signing)
        .coin(CoinType::BitcoinCash)
        .sign(sign::Expected {
            encoded: transfer_96ee20::ENCODED_TX,
            txid: transfer_96ee20::TX_ID,
            inputs: vec![5151],
            outputs: vec![600, 4325],
            // `vsize` is different from the estimated value due to the signatures der serialization.
            vsize: 226,
            weight: 904,
            fee: 226,
        });
}
