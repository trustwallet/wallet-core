// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::test_utils::address_utils::{
    test_address_get_data, test_address_invalid, test_address_normalization, test_address_valid,
};
use tw_coin_registry::coin_type::CoinType;

#[test]
fn test_solana_address_normalization() {
    test_address_normalization(
        CoinType::Solana,
        "2gVkYWexTHR5Hb2aLeQN3tnngvWzisFKXDUPrgMHpdST",
        "2gVkYWexTHR5Hb2aLeQN3tnngvWzisFKXDUPrgMHpdST",
    );
}

#[test]
fn test_solana_address_is_valid() {
    test_address_valid(
        CoinType::Solana,
        "2gVkYWexTHR5Hb2aLeQN3tnngvWzisFKXDUPrgMHpdST",
    );
}

#[test]
fn test_solana_address_invalid() {
    // Contains invalid base-58 character
    test_address_invalid(
        CoinType::Solana,
        "2gVkYWexTHR5Hb2aLeQN3tnngvWzisFKXDUPrgMHpdSl",
    );
    // Is invalid length
    test_address_invalid(
        CoinType::Solana,
        "2gVkYWexTHR5Hb2aLeQN3tnngvWzisFKXDUPrgMHpd",
    );
}

#[test]
fn test_solana_address_get_data() {
    test_address_get_data(
        CoinType::Solana,
        "2gVkYWexTHR5Hb2aLeQN3tnngvWzisFKXDUPrgMHpdST",
        "18f9d8d877393bbbe8d697a8a2e52879cc7e84f467656d1cce6bab5a8d2637ec",
    );
}
