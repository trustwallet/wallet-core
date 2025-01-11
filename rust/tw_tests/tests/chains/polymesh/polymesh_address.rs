// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::test_utils::address_utils::{
    test_address_ss58_is_invalid, test_address_ss58_is_valid,
};
use tw_coin_registry::coin_type::CoinType;

#[test]
fn test_polymesh_address_is_valid() {
    // Polymesh
    test_address_ss58_is_valid(
        CoinType::Polkadot,
        "2DxwekgWwK7sqVeuXGmaXLZUvwnewLTs2rvU2CFKLgvvYwCG",
        12, // Polymesh ss58
    );
}

#[test]
fn test_polymesh_address_invalid() {
    // Substrate ed25519
    test_address_ss58_is_invalid(
        CoinType::Polkadot,
        "5FqqU2rytGPhcwQosKRtW1E3ha6BJKAjHgtcodh71dSyXhoZ",
        12, // Polymesh ss58
    );
    test_address_ss58_is_invalid(
        CoinType::Polkadot,
        "JCViCkwMdGWKpf7Wogb8EFtDmaYTEZGEg6ah4svUPGnnpc7A",
        12, // Polymesh ss58
    );
}
