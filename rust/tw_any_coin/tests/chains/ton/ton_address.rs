// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::test_utils::address_utils::{
    test_address_get_data, test_address_invalid, test_address_normalization, test_address_valid,
};
use tw_coin_registry::coin_type::CoinType;

const WALLET_1_DATA: &str = "8a8627861a5dd96c9db3ce0807b122da5ed473934ce7568a5b4b1c361cbb28ae";
const WALLET_1_RAW: &str = "0:8a8627861a5dd96c9db3ce0807b122da5ed473934ce7568a5b4b1c361cbb28ae";
const WALLET_1_BOUNCEABLE: &str = "EQCKhieGGl3ZbJ2zzggHsSLaXtRzk0znVopbSxw2HLsorkdl";
const WALLET_1_NON_BOUNCEABLE: &str = "UQCKhieGGl3ZbJ2zzggHsSLaXtRzk0znVopbSxw2HLsorhqg";
const WALLET_1_BOUNCEABLE_TESTNET: &str = "kQCKhieGGl3ZbJ2zzggHsSLaXtRzk0znVopbSxw2HLsorvzv";
const WALLET_1_NON_BOUNCEABLE_TESTNET: &str = "0QCKhieGGl3ZbJ2zzggHsSLaXtRzk0znVopbSxw2HLsorqEq";

#[test]
fn test_ton_address_normalization() {
    test_address_normalization(CoinType::TON, WALLET_1_RAW, WALLET_1_NON_BOUNCEABLE);
    test_address_normalization(CoinType::TON, WALLET_1_BOUNCEABLE, WALLET_1_NON_BOUNCEABLE);
    test_address_normalization(
        CoinType::TON,
        WALLET_1_NON_BOUNCEABLE,
        WALLET_1_NON_BOUNCEABLE,
    );
    test_address_normalization(
        CoinType::TON,
        WALLET_1_BOUNCEABLE_TESTNET,
        WALLET_1_NON_BOUNCEABLE,
    );
    test_address_normalization(
        CoinType::TON,
        WALLET_1_NON_BOUNCEABLE_TESTNET,
        WALLET_1_NON_BOUNCEABLE,
    );
}

#[test]
fn test_ton_address_is_valid() {
    // Raw, hex-encoded
    // For now, we allow to specify master-chain addresses.
    test_address_valid(
        CoinType::TON,
        "-1:8a8627861a5dd96c9db3ce0807b122da5ed473934ce7568a5b4b1c361cbb28ae",
    );

    // User-friendly, url-safe
    test_address_valid(
        CoinType::TON,
        "EQCKhieGGl3ZbJ2zzggHsSLaXtRzk0znVopbSxw2HLsorkdl",
    );
    test_address_valid(
        CoinType::TON,
        "EQBGqFmKe3oY8PChYN9g92ZEV2ybkzVB-hCiesQRn5mFnrNv",
    );
    test_address_valid(
        CoinType::TON,
        "Ef8JfFPRpHBV_tZpCurvxMJW69nt2js3SuGEWojGnOpCVPRe",
    );
    test_address_valid(
        CoinType::TON,
        "Ef_drj6m7jcME0fWTA-OwFC-6F0Le2SuOUQ6ibRc3Vz8HL8H",
    );

    // User-friendly
    test_address_valid(
        CoinType::TON,
        "EQAN6Dr3vziti1Kp9D3aEFqJX4bBVfCaV57Z+9jwKTBXICv8",
    );
    test_address_valid(
        CoinType::TON,
        "EQCmGW+z+UL00FmnhWaMvJq/i86YY5GlJP3uJW19KC5Tzq4C",
    );
}

#[test]
fn test_ton_address_invalid() {
    test_address_invalid(CoinType::TON, "random string");

    // Invalid size
    test_address_invalid(
        CoinType::TON,
        "EQIcIZpPoMnWXd8FbC1KaLtcyIgVUlwsbFK_3P6f5uf_YyzoE",
    );
    test_address_invalid(
        CoinType::TON,
        "EQIcIZpPoMnWXd8FbC1KaLtcyIgVUlwsbFK_3P6f5uf_YyE",
    );

    // Invalid size after decode
    test_address_invalid(
        CoinType::TON,
        "EQIcIZpPoMnWXd8FbC1KaLtcyIgVUlwsbFK_3P6f5uf_Yyw=",
    );

    // Invalid workchain
    test_address_invalid(
        CoinType::TON,
        "1:0ccd5119f27f7fe4614476c34f7e5e93c7ae098e577cf2012f8b8043165cb809",
    );
    test_address_invalid(
        CoinType::TON,
        "EQEMzVEZ8n9_5GFEdsNPfl6Tx64Jjld88gEvi4BDFly4CSyl",
    );
    test_address_invalid(
        CoinType::TON,
        "-2:e0e98cfcf743292298ad9e379a3c2e6401797b9cbfc0fe98b4e14fd0ce07ecdf",
    );
    test_address_invalid(
        CoinType::TON,
        "Ef7g6Yz890MpIpitnjeaPC5kAXl7nL_A_pi04U_Qzgfs3-Cj",
    );

    // Invalid tag
    test_address_invalid(
        CoinType::TON,
        "MwCKhieGGl3ZbJ2zzggHsSLaXtRzk0znVopbSxw2HLsorsn8",
    ); // 0x33
    test_address_invalid(
        CoinType::TON,
        "swCKhieGGl3ZbJ2zzggHsSLaXtRzk0znVopbSxw2HLsornJ2",
    ); // 0x80 + 0x33
    test_address_invalid(
        CoinType::TON,
        "EQCKhieGGl3ZbJ2zzggHsSLaXtRzk0znVopbSxw2HLsormVH",
    ); // crc[a, b] = crc[b, a]
    test_address_invalid(
        CoinType::TON,
        "EQCKhieGGl3ZbJ2zzggHsSLaXtRzk0znVopbSxw2HLsorpcF",
    ); // crc=0x9705
}

#[test]
fn test_ton_address_get_data() {
    test_address_get_data(CoinType::TON, WALLET_1_NON_BOUNCEABLE, WALLET_1_DATA);
}
