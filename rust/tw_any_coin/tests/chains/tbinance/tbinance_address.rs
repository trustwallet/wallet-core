// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::test_utils::address_utils::{
    test_address_get_data, test_address_invalid, test_address_normalization, test_address_valid,
};
use tw_coin_registry::coin_type::CoinType;

#[test]
fn test_tbinance_address_normalization() {
    test_address_normalization(
        CoinType::TBinance,
        "tbnb18mtcq20875cr0p7l4upm0u5zx4r9jpj2kfu9f8",
        "tbnb18mtcq20875cr0p7l4upm0u5zx4r9jpj2kfu9f8",
    );
}

#[test]
fn test_tbinance_address_is_valid() {
    test_address_valid(
        CoinType::TBinance,
        "tbnb18mtcq20875cr0p7l4upm0u5zx4r9jpj2kfu9f8",
    );
}

#[test]
fn test_tbinance_address_invalid() {
    test_address_invalid(
        CoinType::TBinance,
        "tbnb18mtcq20875cr0p7l4upm0u5zx4r9jpj2kfu9aa",
    );
    // Mainnet.
    test_address_invalid(
        CoinType::TBinance,
        "bnb1h8xf9htasu9aclra954dnnve8fgcda4ae7qfa8",
    );
}

#[test]
fn test_tbinance_address_get_data() {
    test_address_get_data(
        CoinType::TBinance,
        "tbnb18mtcq20875cr0p7l4upm0u5zx4r9jpj2kfu9f8",
        "3ed78029e7f5303787dfaf03b7f282354659064a",
    );
}
