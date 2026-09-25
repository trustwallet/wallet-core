// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::test_utils::address_utils::{
    test_address_derive, test_address_get_data, test_address_invalid, test_address_normalization,
    test_address_valid, KeyType,
};
use tw_coin_registry::coin_type::CoinType;

#[test]
fn test_zcash_address_derive() {
    test_address_derive(
        CoinType::Zcash,
        KeyType::PrivateKey("2d8f68944bdbfbc0769542fba8fc2d2a3de67393334471624364c7006da2aa54"),
        "t1Wg9uPPAfwhBWeRjtDPa5ZHNzyBx9rJVKY",
    );
}

#[test]
fn test_zcash_address_normalization() {
    test_address_normalization(
        CoinType::Zcash,
        "t1RygJmrLdNGgi98gUgEJDTVaELTAYWoMBy",
        "t1RygJmrLdNGgi98gUgEJDTVaELTAYWoMBy",
    );
    test_address_normalization(
        CoinType::Zcash,
        "tex1auz6gx89x2wcku6gswdvaz2nf9x3seex6px6v0",
        "tex1auz6gx89x2wcku6gswdvaz2nf9x3seex6px6v0",
    );
}

#[test]
fn test_zcash_address_is_valid() {
    test_address_valid(CoinType::Zcash, "t1RygJmrLdNGgi98gUgEJDTVaELTAYWoMBy");
    test_address_valid(CoinType::Zcash, "t1TWk2mmvESDnE4dmCfT7MQ97ij6ZqLpNVU");
    test_address_valid(CoinType::Zcash, "t3RD6RFKhWSotNbPEY4Vw7Ku9QCfKkzrbBL");
    test_address_valid(CoinType::Zcash, "t1Wg9uPPAfwhBWeRjtDPa5ZHNzyBx9rJVKY");
    test_address_valid(CoinType::Zcash, "t1TKCtCETHPrAdA6eY1fdhhnTkTmb371oPt");
    test_address_valid(CoinType::Zcash, "t1cWhcXydPYTG1pgHMsZ6JEPsWGxVMdJ5t6");
    test_address_valid(
        CoinType::Zcash,
        "tex1auz6gx89x2wcku6gswdvaz2nf9x3seex6px6v0",
    );
    // https://zips.z.cash/zip-0320#reference-implementation
    test_address_valid(
        CoinType::Zcash,
        "tex1s2rt77ggv6q989lr49rkgzmh5slsksa9khdgte",
    );
}

#[test]
fn test_zcash_address_invalid() {
    test_address_invalid(CoinType::Zcash, "abc");
    test_address_invalid(
        CoinType::Zcash,
        "0x5aAeb6053F3E94C9b9A09f33669435E7Ef1BeAed",
    );
    test_address_invalid(CoinType::Zcash, "175tWpb8K1S7NmH4Zx6rewF9WQrcZv245W");
    // Invalid Base58
    test_address_invalid(CoinType::Zcash, "t1RygJmrLdNGgi98+UgEJDTVaELTAYWoMBy");
    // Too short
    test_address_invalid(CoinType::Zcash, "t1RygJmrLdNGgi98gUgEJDTVaELTAYW");
    // Bad checksum
    test_address_invalid(CoinType::Zcash, "t1RygJmrLdNGgi98gUgEJDTVaELTAYWoMBz");
    // Too short
    test_address_invalid(CoinType::Zcash, "TJRyWwFs9wTFGZg3JbrVriFbNfCug5tDeC");
    // Invalid T-prefix
    test_address_invalid(CoinType::Zcash, "2NRbuP5YfzRNEa1RibT5kXay1VgvQHnydZY1");
    // Invalid Tex-address checksum
    test_address_invalid(
        CoinType::Zcash,
        "textest1auz6gx89x2wcku6gswdvaz2nf9x3seex6px6v0",
    );
}

#[test]
fn test_zcash_address_get_data() {
    test_address_get_data(
        CoinType::Zcash,
        "t1Wg9uPPAfwhBWeRjtDPa5ZHNzyBx9rJVKY",
        "8c6f453157897ce2e6de413f329d995fe0d8f902",
    );
    test_address_get_data(
        CoinType::Zcash,
        "tex1auz6gx89x2wcku6gswdvaz2nf9x3seex6px6v0",
        "ef05a418e5329d8b7348839ace8953494d186726",
    );
}
