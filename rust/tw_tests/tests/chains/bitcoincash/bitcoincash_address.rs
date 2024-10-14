// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::test_utils::address_utils::{
    test_address_base58_is_valid, test_address_create_base58_with_public_key, test_address_derive,
    test_address_get_data, test_address_invalid, test_address_normalization, test_address_valid,
    AddressBase58IsValid, AddressCreateBase58WithPublicKey,
};
use tw_coin_registry::coin_type::CoinType;
use tw_keypair::tw::PublicKeyType;

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
    test_address_normalization(
        CoinType::BitcoinCash,
        "qqslmu0jxk4st3ldjyuazfpf5thd6vlgfuggqd3re4",
        "bitcoincash:qqslmu0jxk4st3ldjyuazfpf5thd6vlgfuggqd3re4",
    );
    // Legacy address should be normalized as Legacy.
    test_address_normalization(
        CoinType::BitcoinCash,
        "1BvBMSEYstWetqTFn5Au4m4GFg7xJaNVN2",
        "1BvBMSEYstWetqTFn5Au4m4GFg7xJaNVN2",
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
    test_address_valid(
        CoinType::BitcoinCash,
        "qq07l6rr5lsdm3m80qxw80ku2ex0tj76vvsxpvmgme",
    );
    test_address_valid(
        CoinType::BitcoinCash,
        "qqslmu0jxk4st3ldjyuazfpf5thd6vlgfuggqd3re4",
    );
    test_address_valid(CoinType::BitcoinCash, "1BvBMSEYstWetqTFn5Au4m4GFg7xJaNVN2");
}

#[test]
fn test_bitcoincash_address_is_valid_base58() {
    // Litecoin P2PKH
    test_address_base58_is_valid(AddressBase58IsValid {
        coin: CoinType::BitcoinCash,
        address: "LW6HjAU6GL9fK2LZWUA6VZCzomTdrpx3nr",
        p2pkh: 48,
        p2sh: 50,
    });
    // BitcoinCash P2SH
    test_address_base58_is_valid(AddressBase58IsValid {
        coin: CoinType::Bitcoin,
        address: "35XRC5HRZjih1sML23UXv1Ry1SzTDKSmfQ",
        p2pkh: 0,
        p2sh: 5,
    });
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

#[test]
fn test_bitcoincash_address_create_base58_with_public_key() {
    test_address_create_base58_with_public_key(AddressCreateBase58WithPublicKey {
        coin: CoinType::BitcoinCash,
        public_key: "03b49081a4d7ad24b20e209bc6fe10491aadb5607777baf0509a036cce96025db0",
        public_key_type: PublicKeyType::Secp256k1,
        p2pkh: 48,
        p2sh: 50,
        expected: "LW6HjAU6GL9fK2LZWUA6VZCzomTdrpx3nr",
    });
}
