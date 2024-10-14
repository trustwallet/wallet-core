// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::test_utils::address_utils::{
    test_address_invalid, test_address_normalization, test_address_valid,
};
use tw_coin_registry::coin_type::CoinType;

#[test]
fn test_bitcoincash_address_normalization() {
    test_address_normalization(
        CoinType::eCash,
        "pqx578nanz2h2estzmkr53zqdg6qt8xyqvh683mrz0",
        "ecash:pqx578nanz2h2estzmkr53zqdg6qt8xyqvh683mrz0",
    );
}

#[test]
fn test_bitcoincash_address_is_valid() {
    test_address_valid(
        CoinType::eCash,
        "pqx578nanz2h2estzmkr53zqdg6qt8xyqvh683mrz0",
    );
    test_address_valid(
        CoinType::eCash,
        "ecash:qqra3amvnyyhrltyn5h97klwe68cuw3sfcgry9hl9k",
    );
}

#[test]
fn test_bitcoincash_address_invalid() {
    // Wrong checksum
    test_address_invalid(
        CoinType::eCash,
        "pqx578nanz2h2estzmkr53zqdg6qt8xyqvffffffff",
    );
    test_address_invalid(
        CoinType::eCash,
        "ecash:pqx578nanz2h2estzmkr53zqdg6qt8xyqvffffffff",
    );

    // Valid BCH addresses are invalid for eCash
    test_address_invalid(
        CoinType::eCash,
        "pqx578nanz2h2estzmkr53zqdg6qt8xyqvwhn6qeyc",
    );
    test_address_invalid(
        CoinType::eCash,
        "bitcoincash:pqx578nanz2h2estzmkr53zqdg6qt8xyqvwhn6qeyc",
    );

    // Wrong prefix
    test_address_invalid(
        CoinType::eCash,
        "fcash:pqx578nanz2h2estzmkr53zqdg6qt8xyqvh683mrz0",
    );

    // Wrong base 32 (characters o, i)
    test_address_invalid(
        CoinType::eCash,
        "poi578nanz2h2estzmkr53zqdg6qt8xyqvwhn6qeyc",
    );
}
