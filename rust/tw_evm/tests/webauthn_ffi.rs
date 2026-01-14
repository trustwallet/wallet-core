// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_encoding::hex::{DecodeHex, ToHex};
use tw_evm::ffi::webauthn_solidity::tw_webauthn_solidity_get_message_hash;
use tw_hash::H256;
use tw_keypair::ecdsa::{der, nist256p1};
use tw_keypair::traits::SigningKeyTrait;
use tw_memory::test_utils::tw_data_helper::TWDataHelper;
use tw_memory::test_utils::tw_string_helper::TWStringHelper;

#[test]
fn test_get_webauthn_message_hash() {
    let authenticator_data = TWStringHelper::create(
        "0x49960de5880e8c687434170f6476605b8fe4aeb9a28632c7995cf3ba831d97630500000000",
    );
    let client_data_json = TWStringHelper::create(
        r#"{"type":"webauthn.get","challenge":"GFQOrZ9hRIfwzt9nUt8zxrl6vVQmMnhLK9klXhDFP6c","origin":"https://sign.coinbase.com","crossOrigin":false}"#,
    );

    let message_hash = TWDataHelper::wrap(unsafe {
        tw_webauthn_solidity_get_message_hash(authenticator_data.ptr(), client_data_json.ptr())
    });
    assert_eq!(
        message_hash.to_vec().unwrap().to_hex_prefixed(),
        "0x03301a2660d1cea1c268c237ccd63ecfc8ca32573d7a6cf702cbbbaab9d7ac2a"
    );
}

#[test]
fn test_get_webauthn_formatted_signature() {
    let authenticator_data = TWStringHelper::create(
        "0x49960de5880e8c687434170f6476605b8fe4aeb9a28632c7995cf3ba831d97630500000000",
    );
    let client_data_json = TWStringHelper::create(
        r#"{"type":"webauthn.get","challenge":"GFQOrZ9hRIfwzt9nUt8zxrl6vVQmMnhLK9klXhDFP6c","origin":"https://sign.coinbase.com","crossOrigin":false}"#,
    );
    let der_signature = TWDataHelper::create(
        "304502203d0ece361a0a304ed87dbcccbcd0ae5924ed497bf0500cc850e57b76d4f7d6c1022100a87345592418c98dcff8e159f5e0d9fefe8636d95c784a1a740e514ab2c7fb87"
            .decode_hex()
            .unwrap()
    );
    let pk = nist256p1::KeyPair::try_from(
        "0x03d99692017473e2d631945a812607b23269d85721e0f370b8d3e7d29a874fd2",
    )
    .unwrap();
    let sig = pk
        .sign(H256::from(
            "0x0b8772b952d27e2a8d6a51b0177b18b8ed1c3ebede2d0d7992646841b25322ac",
        ))
        .unwrap();
    let ss = der::Signature::new(sig.r(), sig.s()).unwrap();
    println!("{}", ss.to_hex_prefixed());

    let formatted_signature = TWDataHelper::wrap(unsafe {
        tw_evm::ffi::webauthn_solidity::tw_webauthn_solidity_get_formatted_signature(
            authenticator_data.ptr(),
            client_data_json.ptr(),
            der_signature.ptr(),
        )
    });
    assert_eq!(
        formatted_signature.to_vec().unwrap().to_hex_prefixed(),
        "0x000000000000000000000000000000000000000000000000000000000000002000000000000000000000000000000000000000000000000000000000000000c00000000000000000000000000000000000000000000000000000000000000120000000000000000000000000000000000000000000000000000000000000001700000000000000000000000000000000000000000000000000000000000000013d0ece361a0a304ed87dbcccbcd0ae5924ed497bf0500cc850e57b76d4f7d6c1578cbaa5dbe7367330071ea60a1f2600be60c3d44a9f546a7fab7978499b29ca000000000000000000000000000000000000000000000000000000000000002549960de5880e8c687434170f6476605b8fe4aeb9a28632c7995cf3ba831d97630500000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000008a7b2274797065223a22776562617574686e2e676574222c226368616c6c656e6765223a224746514f725a3968524966777a74396e5574387a78726c367656516d4d6e684c4b396b6c58684446503663222c226f726967696e223a2268747470733a2f2f7369676e2e636f696e626173652e636f6d222c2263726f73734f726967696e223a66616c73657d00000000000000000000000000000000000000000000"
    );
}
