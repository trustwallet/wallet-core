// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::test_utils::address_utils::{
    test_address_derive, test_address_get_data, test_address_invalid, test_address_normalization,
    test_address_valid,
};
use tw_coin_registry::coin_type::CoinType;

#[test]
fn test_bitcoincash_address_derive() {
    test_address_derive(
        CoinType::BitcoinCash,
        "28071bf4e2b0340db41b807ed8a5514139e5d6427ff9d58dbd22b7ed187103a4",
        "bitcoincash:qruxj7zq6yzpdx8dld0e9hfvt7u47zrw9gfr5hy0vh",
    );
}

#[test]
fn test_bitcoincash_address_normalization() {
    test_address_normalization(
        CoinType::BitcoinCash,
        "BitCoinCash:QRPLWYX7KUEQKRH6DMD3FCLTA6U32HAFP5TNPKCHX2",
        "bitcoincash:qrplwyx7kueqkrh6dmd3fclta6u32hafp5tnpkchx2",
    );
    // Already normalized.
    test_address_normalization(
        CoinType::BitcoinCash,
        "bitcoincash:qruxj7zq6yzpdx8dld0e9hfvt7u47zrw9gfr5hy0vh",
        "bitcoincash:qruxj7zq6yzpdx8dld0e9hfvt7u47zrw9gfr5hy0vh",
    );
}

#[test]
fn test_bitcoincash_address_is_valid() {
    test_address_valid(
        CoinType::BitcoinCash,
        "bitcoincash:qr6m7j9njldwwzlg9v7v53unlr4jkmx6eylep8ekg2",
    );
    test_address_valid(
        CoinType::BitcoinCash,
        "bitcoincash:qqa2qx0d8tegw32xk8u75ws055en4x3h2u0e6k46y4",
    );
    test_address_valid(
        CoinType::BitcoinCash,
        "bitcoincash:pqx578nanz2h2estzmkr53zqdg6qt8xyqvwhn6qeyc",
    );
    test_address_valid(
        CoinType::BitcoinCash,
        "pqx578nanz2h2estzmkr53zqdg6qt8xyqvwhn6qeyc",
    );
    test_address_valid(
        CoinType::BitcoinCash,
        "bitcoincash:qr6m7j9njldwwzlg9v7v53unlr4jkmx6eylep8ekg2",
    );
}

#[test]
fn test_bitcoincash_address_invalid() {
    // Wrong checksum
    test_address_invalid(
        CoinType::BitcoinCash,
        "pqx578nanz2h2estzmkr53zqdg6qt8xyqvffffffff",
    );
    test_address_invalid(
        CoinType::BitcoinCash,
        "bitcoincash:pqx578nanz2h2estzmkr53zqdg6qt8xyqvffffffff",
    );

    // Valid eCash addresses are invalid for BCH
    test_address_invalid(
        CoinType::BitcoinCash,
        "pqx578nanz2h2estzmkr53zqdg6qt8xyqvh683mrz0",
    );
    test_address_invalid(
        CoinType::BitcoinCash,
        "ecash:pqx578nanz2h2estzmkr53zqdg6qt8xyqvh683mrz0",
    );

    // Wrong prefix
    test_address_invalid(
        CoinType::BitcoinCash,
        "bcash:pqx578nanz2h2estzmkr53zqdg6qt8xyqvwhn6qeyc",
    );

    // Wrong base 32 (characters o, i)
    test_address_invalid(
        CoinType::BitcoinCash,
        "poi578nanz2h2estzmkr53zqdg6qt8xyqvwhn6qeyc",
    );
}

#[test]
fn test_bitcoincash_address_get_data() {
    test_address_get_data(
        CoinType::BitcoinCash,
        "bitcoincash:qpk05r5kcd8uuzwqunn8rlx5xvuvzjqju5rch3tc0u",
        "6cfa0e96c34fce09c0e4e671fcd43338c14812e5",
    );
}
