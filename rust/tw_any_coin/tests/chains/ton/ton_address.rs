// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::test_utils::address_utils::{
    test_address_derive, test_address_get_data, test_address_invalid, test_address_normalization,
    test_address_valid,
};
use tw_coin_registry::coin_type::CoinType;

const WALLET_1_DATA: &str = "8a8627861a5dd96c9db3ce0807b122da5ed473934ce7568a5b4b1c361cbb28ae";
const WALLET_1_RAW: &str = "0:8a8627861a5dd96c9db3ce0807b122da5ed473934ce7568a5b4b1c361cbb28ae";
const WALLET_1_BOUNCEABLE: &str = "EQCKhieGGl3ZbJ2zzggHsSLaXtRzk0znVopbSxw2HLsorkdl";
const WALLET_1_NON_BOUNCEABLE: &str = "UQCKhieGGl3ZbJ2zzggHsSLaXtRzk0znVopbSxw2HLsorhqg";
const WALLET_1_BOUNCEABLE_TESTNET: &str = "kQCKhieGGl3ZbJ2zzggHsSLaXtRzk0znVopbSxw2HLsorvzv";
const WALLET_1_NON_BOUNCEABLE_TESTNET: &str = "0QCKhieGGl3ZbJ2zzggHsSLaXtRzk0znVopbSxw2HLsorqEq";

#[test]
fn test_ton_address_derive() {
    test_address_derive(
        CoinType::TON,
        "5849481021e305dfdf9f0eaf87e07f15efec3fde8d8ed639c9fcf0bc351d998b",
        "UQAACKJfEIfI5vkht_w3NYk8k-OU5Xl_jq9XNmmkcPaUO-tB",
    );
    test_address_derive(
        CoinType::TON,
        "4a22d994755145e4a4ce7263bdb3a8a70e449c1fccdd299f80df862bd4dcb930",
        "UQA3fRa_AHKBo1Lu8QF5xm_fiCLi197NfoPbeta0VJyvKa76",
    );
    test_address_derive(
        CoinType::TON,
        "287d4c0fcc445173fe211e4ade3518c75cff7a9dd79070f54395058cdd53e485",
        "UQCbJ0QdlDmC0ofyCtH15PHBzrj9sMLoLjNjkgTYaxR8gn9z",
    );
    test_address_derive(
        CoinType::TON,
        "136e464280c4222a99ac34d5077a2edf11e4468a076741e495d9f31ca7939a1f",
        "UQAliC8yJh-Ru2uwEWgEaEV9LHEs0-c1blYr_XZe7CpEwm9D",
    );
    test_address_derive(
        CoinType::TON,
        "f15bb09a2cf37f6e6b6515be4000cdf271338c56fb1ec81848f2f1407b3a4003",
        "UQCeAQaIFwwjmcJkYfqGiyHo2ag7qUMMfsUi28HLWmtpA7zF",
    );
    test_address_derive(
        CoinType::TON,
        "532005268411b3b4ac85b080c8a3bc4a52600be75f758013302745ac05ac18f0",
        "UQDD0YS5pQSe3fgHEKd-D7qTieRxmSknlQQW6fb7IFu7ky9T",
    );
    test_address_derive(
        CoinType::TON,
        "15e5a13ec259bb4515105ba0a84ee93eaa9f56f6fdf73bf6179d1ed80b6a399c",
        "UQAx8JmUT4p14RUAu9gpXqmTzkQz4e3GZz8VQjqWXFDxG6-S",
    );
    test_address_derive(
        CoinType::TON,
        "9b503ff85debe95093acf0f9b057607a0a5be91cb47e2e6ec342d7825c7fafbc",
        "UQD91HEk-TJVublA57dgkSwgrRORj48ubEIfjEPZIjQ08oZl",
    );
    test_address_derive(
        CoinType::TON,
        "97075969876382280ff7598738b3fd2c1748f9a549dd6f5d6aa5694c21deddce",
        "UQBrL2lNG3ThmYbf9gaA_-tsPfdrcGy27LP0M-qg-1TpG_wR",
    );
    test_address_derive(
        CoinType::TON,
        "57d86027989f8ec649cce3be862d68564d471395c5694918b0348e17c7ef6ffe",
        "UQDUJLg8MYZPT2C-2n-ulyNSkBsdDaUzqEq71dBx3l6fhnuL",
    );
    test_address_derive(
        CoinType::TON,
        "b1ad2bff14fc018493c32c37cded62892ec507471e34a25da0b5b5f05e131751",
        "UQD079e5CETiOrR_iS0atJukl7ixS6EYbtaWRSGykFAtwRhL",
    );
    test_address_derive(
        CoinType::TON,
        "16ad201c59ecd7ace74e1677160106923a3d2ee11c495be5c3b88ba6f7ef3d17",
        "UQDuU7NOk_eGzP5CLs_Zeg0LBpySAGy02qqGv0cO_zX5WN1n",
    );
    test_address_derive(
        CoinType::TON,
        "3d935b7a8c24e7dc55ef7c0c890806cee3af1174a62165d4d2fb64ccf2e2260b",
        "UQALRogl66QrJIb3KbStOd-ZadA6Ye2g23ME6JAMU1HOXRG2",
    );
    test_address_derive(
        CoinType::TON,
        "68f3a87d12514854774300b8a4c449616e208336f1b609c96fcd8b1a87d4e064",
        "UQALrF1c2UeoCybOsSeAdUmip4yhcCtrUAhKZ-9bxv6_okVx",
    );
    test_address_derive(
        CoinType::TON,
        "fbfbc640c4cd4649161a935562217f1caecf6e7f3a2818921f9ee336741a48cb",
        "UQAzCS7JoSiOi1BdH4nFkuvUwbBjxUzPx1AhQKwiwXAv27Xs",
    );
}

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
