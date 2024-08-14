// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_memory::test_utils::tw_string_helper::TWStringHelper;
use wallet_core_rs::ffi::ton::address_converter::{
    tw_ton_address_converter_from_boc, tw_ton_address_converter_to_boc,
    tw_ton_address_converter_to_user_friendly,
};

struct GetJettonAddressInput {
    main_address: &'static str,
    expected_main_address_encoded: &'static str,
    jetton_address_boc_encoded: &'static str,
    expected_jetton_address: &'static str,
}

fn test_address_converter_get_jetton_address(input: GetJettonAddressInput) {
    let main_address = TWStringHelper::create(input.main_address);

    let main_address_boc_encoded_ptr =
        unsafe { tw_ton_address_converter_to_boc(main_address.ptr()) };
    let main_address_boc_encoded = TWStringHelper::wrap(main_address_boc_encoded_ptr)
        .to_string()
        .unwrap();
    assert_eq!(
        main_address_boc_encoded,
        input.expected_main_address_encoded
    );

    // curl --location 'https://toncenter.com/api/v2/runGetMethod' --header 'Content-Type: application/json' --data \
    // '{"address":"JETTON_CONTRACT_ADDRESS","method":"get_wallet_address","method":"get_wallet_address","stack":[["tvm.Slice","YOUR_MAIN_ADDRESS"]]}'

    // `get_wallet_address` response:
    let jetton_address_boc_encoded = TWStringHelper::create(input.jetton_address_boc_encoded);
    let jetton_address_ptr =
        unsafe { tw_ton_address_converter_from_boc(jetton_address_boc_encoded.ptr()) };
    let jetton_address_str = TWStringHelper::wrap(jetton_address_ptr)
        .to_string()
        .unwrap();
    assert_eq!(jetton_address_str, input.expected_jetton_address);
}

#[test]
fn test_address_converter_get_jetton_notcoin_address() {
    test_address_converter_get_jetton_address(GetJettonAddressInput {
        main_address: "UQBjKqthWBE6GEcqb_epTRFrQ1niS6Z1Z1MHMwR-mnAYRoYr",
        expected_main_address_encoded:
            "te6cckEBAQEAJAAAQ4AMZVVsKwInQwjlTf71KaItaGs8SXTOrOpg5mCP004DCNAptHQU",
        // curl --location 'https://toncenter.com/api/v2/runGetMethod' --header 'Content-Type: application/json' --data \
        // '{"address":"EQAvlWFDxGF2lXm67y4yzC17wYKD9A0guwPkMs1gOsM__NOT","method":"get_wallet_address","method":"get_wallet_address","stack":[["tvm.Slice","te6cckEBAQEAJAAAQ4AMZVVsKwInQwjlTf71KaItaGs8SXTOrOpg5mCP004DCNAptHQU"]]}'
        jetton_address_boc_encoded:
            "te6cckEBAQEAJAAAQ4AFvT5rqwxcbKfITqnkwL+go4Zi9bulRHAtLt4cjjFdK7B8L+Cq",
        expected_jetton_address: "UQAt6fNdWGLjZT5CdU8mBf0FHDMXrd0qI4FpdvDkcYrpXV5H",
    });
}

#[test]
fn test_address_converter_get_jetton_usdt_address() {
    test_address_converter_get_jetton_address(GetJettonAddressInput {
        main_address: "UQBjKqthWBE6GEcqb_epTRFrQ1niS6Z1Z1MHMwR-mnAYRoYr",
        expected_main_address_encoded:
            "te6cckEBAQEAJAAAQ4AMZVVsKwInQwjlTf71KaItaGs8SXTOrOpg5mCP004DCNAptHQU",
        // curl --location 'https://toncenter.com/api/v2/runGetMethod' --header 'Content-Type: application/json' --data \
        // '{"address":"EQCxE6mUtQJKFnGfaROTKOt1lZbDiiX1kCixRv7Nw2Id_sDs","method":"get_wallet_address","method":"get_wallet_address","stack":[["tvm.Slice","te6cckEBAQEAJAAAQ4AMZVVsKwInQwjlTf71KaItaGs8SXTOrOpg5mCP004DCNAptHQU"]]}'
        jetton_address_boc_encoded:
            "te6cckEBAQEAJAAAQ4Aed71FEI46jdFXghsGUIG2GIR8wpbQaLzrKNj7BtHOEHBSO5Mf",
        expected_jetton_address: "UQDzveoohHHUboq8ENgyhA2wxCPmFLaDRedZRsfYNo5wg4TL",
    });
}

#[test]
fn test_address_converter_get_jetton_ston_address() {
    test_address_converter_get_jetton_address(GetJettonAddressInput {
        main_address: "EQATQPeCwtMzQ9u54nTjUNcK4n_0VRSxPOOROLf_IE0OU3XK",
        expected_main_address_encoded:
            "te6cckEBAQEAJAAAQ4ACaB7wWFpmaHt3PE6cahrhXE/+iqKWJ5xyJxb/5AmhynDu6Ygj",
        // curl --location 'https://toncenter.com/api/v2/runGetMethod' --header 'Content-Type: application/json' --data \
        // '{"address":"EQA2kCVNwVsil2EM2mB0SkXytxCqQjS4mttjDpnXmwG9T6bO","method":"get_wallet_address","method":"get_wallet_address","stack":[["tvm.Slice","te6cckEBAQEAJAAAQ4ACaB7wWFpmaHt3PE6cahrhXE/+iqKWJ5xyJxb/5AmhynDu6Ygj"]]}'
        jetton_address_boc_encoded:
            "te6cckEBAQEAJAAAQ4ALPu0dyA1gHd3r7J1rxlvhXSvT5y3rokMDMiCQ86TsUJDnt69H",
        expected_jetton_address: "UQBZ92juQGsA7u9fZOteMt8K6V6fOW9dEhgZkQSHnSdihHPH",
    });
}

#[test]
fn test_address_converter_from_boc_null_address() {
    // `get_wallet_address` response:
    let null_address_boc_encoded = TWStringHelper::create("te6cckEBAQEAAwAAASCUQYZV");
    let jetton_address_ptr =
        unsafe { tw_ton_address_converter_from_boc(null_address_boc_encoded.ptr()) };
    assert_eq!(
        TWStringHelper::wrap(jetton_address_ptr)
            .to_string()
            .unwrap(),
        "UQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAJKZ"
    );
}

#[test]
fn test_address_converter_from_boc_error() {
    #[track_caller]
    fn test_impl(addr_boc_encoded: &str) {
        let addr_boc_encoded_ptr = TWStringHelper::create(addr_boc_encoded);
        let jetton_address_ptr =
            unsafe { tw_ton_address_converter_from_boc(addr_boc_encoded_ptr.ptr()) };
        assert!(
            jetton_address_ptr.is_null(),
            "'{}' BoC encoded expected to be invalid",
            addr_boc_encoded
        );
    }

    // No type bit.
    test_impl("te6cckEBAQEAAwAAAcCO6ba2");
    // No res1 and workchain bits.
    test_impl("te6cckEBAQEAAwAAAaDsenDX");
    // Incomplete hash (31 bytes instead of 32).
    test_impl("te6cckEBAQEAIwAAQYAgQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAUGJnJWk=");
    // Expected 267 bits, found 268.
    test_impl("te6cckEBAQEAJAAAQ4AgQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEgGG0Gq");
}

#[test]
fn test_address_converter_to_user_friendly() {
    #[track_caller]
    fn test_impl(addr: &str, bounceable: bool, testnet: bool, expected: &str) {
        let addr_ptr = TWStringHelper::create(addr);
        let actual_ptr = unsafe {
            tw_ton_address_converter_to_user_friendly(addr_ptr.ptr(), bounceable, testnet)
        };
        let actual_str = TWStringHelper::wrap(actual_ptr).to_string().unwrap();
        assert_eq!(actual_str, expected);
    }

    let raw_address = "0:8a8627861a5dd96c9db3ce0807b122da5ed473934ce7568a5b4b1c361cbb28ae";
    let bounceable = "EQCKhieGGl3ZbJ2zzggHsSLaXtRzk0znVopbSxw2HLsorkdl";
    let non_bounceable = "UQCKhieGGl3ZbJ2zzggHsSLaXtRzk0znVopbSxw2HLsorhqg";
    let bounceable_testnet = "kQCKhieGGl3ZbJ2zzggHsSLaXtRzk0znVopbSxw2HLsorvzv";
    let non_bounceable_testnet = "0QCKhieGGl3ZbJ2zzggHsSLaXtRzk0znVopbSxw2HLsorqEq";

    // Raw to user friendly.
    test_impl(raw_address, true, false, bounceable);
    test_impl(raw_address, false, false, non_bounceable);
    test_impl(raw_address, true, true, bounceable_testnet);
    test_impl(raw_address, false, true, non_bounceable_testnet);

    // Bounceable to non-bounceable.
    test_impl(bounceable, false, false, non_bounceable);

    // Non-bounceable to bounceable.
    test_impl(non_bounceable, true, false, bounceable);

    // Non-bounceable to non-bounceable.
    test_impl(non_bounceable, false, false, non_bounceable);
}

#[test]
fn test_address_converter_to_user_friendly_error() {
    #[track_caller]
    fn test_impl(addr: &str, bounceable: bool, testnet: bool) {
        let addr_ptr = TWStringHelper::create(addr);
        let actual_ptr = unsafe {
            tw_ton_address_converter_to_user_friendly(addr_ptr.ptr(), bounceable, testnet)
        };
        assert!(actual_ptr.is_null());
    }

    // No "0:" prefix.
    test_impl(
        "8a8627861a5dd96c9db3ce0807b122da5ed473934ce7568a5b4b1c361cbb28ae",
        true,
        false,
    );

    // Too short.
    test_impl(
        "EQCKhieGGl3ZbJ2zzggHsSLaXtRzk0znVopbSxw2HLsor",
        false,
        false,
    );
}
