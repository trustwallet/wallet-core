// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::test_utils::address_utils::{
    test_address_derive, test_address_get_data, test_address_invalid, test_address_normalization,
    test_address_valid, KeyType,
};
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_hash::ripemd::sha256_ripemd;

#[test]
fn test_ripple_address_derive() {
    test_address_derive(
        CoinType::XRP,
        KeyType::PublicKey("0303E20EC6B4A39A629815AE02C0A1393B9225E3B890CAE45B59F42FA29BE9668D"),
        "rnBFvgZphmN39GWzUJeUitaP22Fr9be75H",
    );
}

#[test]
fn test_ripple_address_normalization() {
    test_address_normalization(
        CoinType::XRP,
        "rnBFvgZphmN39GWzUJeUitaP22Fr9be75H",
        "rnBFvgZphmN39GWzUJeUitaP22Fr9be75H",
    );
    test_address_normalization(
        CoinType::XRP,
        "X76UnYEMbQfEs3mUqgtjp4zFy9exgThRj7XVZ6UxsdrBptF",
        "X76UnYEMbQfEs3mUqgtjp4zFy9exgThRj7XVZ6UxsdrBptF",
    );
}

#[test]
fn test_ripple_address_is_valid() {
    // Classic address.
    test_address_valid(CoinType::XRP, "r36yxStAh7qgTQNHTzjZvXybCTzUFhrfav");
    // X addresses.
    test_address_valid(
        CoinType::XRP,
        "X76UnYEMbQfEs3mUqgtjp4zFy9exgThRj7XVZ6UxsdrBptF",
    );
    test_address_valid(
        CoinType::XRP,
        "X76UnYEMbQfEs3mUqgtjp4zFy9exgTsM93nriVZAPufrpE3",
    );
    test_address_valid(
        CoinType::XRP,
        "XVfvixWZQKkcenFRYApCjpTUyJ4BePTe3jJv7beatUZvQYh",
    );
}

#[test]
fn test_ripple_address_invalid() {
    // Bitcoin address.
    test_address_invalid(CoinType::XRP, "1Ma2DrB78K7jmAwaomqZNRMCvgQrNjE2QC");
}

#[test]
fn test_ripple_address_get_data() {
    let public_key = "034AADB09CFF4A4804073701EC53C3510CDC95917C2BB0150FB742D0C66E6CEE9E"
        .decode_hex()
        .unwrap();
    let public_key_hash = sha256_ripemd(&public_key);

    test_address_get_data(
        CoinType::XRP,
        "r3kmLJN5D28dHuH8vZNUZpMC43pEHpaocV",
        &public_key_hash.to_hex(),
    );

    test_address_get_data(
        CoinType::XRP,
        "X7tFPvjMH7nDxP8nTGkeeggcUpCZj8UbyT2QoiRHGDfjqrB",
        &public_key_hash.to_hex(),
    );
}
