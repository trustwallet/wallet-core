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
}

#[test]
fn test_zcash_address_is_valid() {
    test_address_valid(CoinType::Zcash, "t1RygJmrLdNGgi98gUgEJDTVaELTAYWoMBy");
    test_address_valid(CoinType::Zcash, "t1TWk2mmvESDnE4dmCfT7MQ97ij6ZqLpNVU");
    test_address_valid(CoinType::Zcash, "t3RD6RFKhWSotNbPEY4Vw7Ku9QCfKkzrbBL");
    test_address_valid(CoinType::Zcash, "t1Wg9uPPAfwhBWeRjtDPa5ZHNzyBx9rJVKY");
    test_address_valid(CoinType::Zcash, "t1TKCtCETHPrAdA6eY1fdhhnTkTmb371oPt");
    test_address_valid(CoinType::Zcash, "t1cWhcXydPYTG1pgHMsZ6JEPsWGxVMdJ5t6");
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
}

#[test]
fn test_zcash_address_get_data() {
    test_address_get_data(
        CoinType::Zcash,
        "t1Wg9uPPAfwhBWeRjtDPa5ZHNzyBx9rJVKY",
        "8c6f453157897ce2e6de413f329d995fe0d8f902",
    );
}
