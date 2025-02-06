// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::test_utils::transaction_calc_tx_hash_utils::TransactionUtilHelper;
use tw_coin_registry::coin_type::CoinType;

#[test]
fn test_polkadot_transaction_util_calc_tx_hash() {
    let encoded_tx = "3502849dca538b7a925b8ea979cc546464a3c5f81d2398a3a272f6f93bdf4803f2f7830073e59cef381aedf56d7af076bafff9857ffc1e3bd7d1d7484176ff5b58b73f1211a518e1ed1fd2ea201bd31869c0798bba4ffe753998c409d098b65d25dff801a5030c0005007120f76076bcb0efdf94c7219e116899d0163ea61cb428183d71324eb33b2bce0300943577";
    let tx_hash = TransactionUtilHelper::calc_tx_hash(CoinType::Polkadot, encoded_tx);

    assert_eq!(
        tx_hash,
        "0x9fd06208a6023e489147d8d93f0182b0cb7e45a40165247319b87278e08362d8"
    );
}
