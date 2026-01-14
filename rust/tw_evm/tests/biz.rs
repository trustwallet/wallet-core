// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_encoding::hex;
use tw_encoding::hex::ToHex;
use tw_evm::modules::biz::{get_encoded_hash, sign_user_op_hash};
use tw_evm::modules::webauthn::{NormalizedS, WebAuthnAuth, WebAuthnMessage};
use tw_keypair::ecdsa::nist256p1;
use tw_keypair::traits::SigningKeyTrait;
use tw_number::U256;

#[test]
fn test_get_encoded_hash() {
    let chain_id = U256::from(31337u64).to_big_endian();
    let code_address = "0x2e234DAe75C793f67A35089C9d99245E1C58470b";
    let code_name = "Biz";
    let code_version = "v1.0.0";
    let type_hash = "0x4f51e7a567f083a31264743067875fc6a7ae45c32c5bd71f6a998c4625b13867";
    let domain_separator_hash =
        "0xd87cd6ef79d4e2b95e15ce8abf732db51ec771f1ca2edccf22a46c729ac56472";
    let sender = "0x174a240e5147D02dE4d7724D5D3E1c1bF11cE029";
    let user_op_hash = "0xf177858c1c500e51f38ffe937bed7e4d3a8678725900be4682d3ce04d97071eb";

    let encoded_hash = get_encoded_hash(
        &chain_id[..],
        code_address,
        code_name,
        code_version,
        type_hash,
        domain_separator_hash,
        sender,
        user_op_hash,
    )
    .unwrap();
    assert_eq!(
        hex::encode(encoded_hash, true),
        "0xc63891abc38f7a991f89ad7cb6d7e53543627b0536c3f5e545b736756c971635"
    );
}

#[test]
fn test_sign_user_op_hash() {
    let hash = "0xc63891abc38f7a991f89ad7cb6d7e53543627b0536c3f5e545b736756c971635";
    let private_key = "0x947dd69af402e7f48da1b845dfc1df6be593d01a0d8274bd03ec56712e7164e8";
    let signed_hash = sign_user_op_hash(hash, private_key).unwrap();
    assert_eq!(hex::encode(signed_hash, true), "0xa29e460720e4b539f593d1a407827d9608cccc2c18b7af7b3689094dca8a016755bca072ffe39bc62285b65aff8f271f20798a421acf18bb2a7be8dbe0eb05f81c");
}

#[test]
fn test_sign_user_op_with_passkey_session() {
    let chain_id = U256::from(31_337u64).to_big_endian();
    let code_address = "0xa0Cb889707d426A7A386870A03bc70d1b0697598";
    let code_name = "PasskeySession";
    let code_version = "v1.0.0";
    // keccak("PasskeySession(bytes32 userOpHash)")
    let type_hash = "0x3463fe66e4d03af5b942aebde2b191eff52d291c0a2c8cc302d786854f34bfc9";
    // keccak("EIP712Domain(string name,string version,uint256 chainId,address verifyingContract,bytes32 salt)")
    let domain_separator_hash =
        "0xd87cd6ef79d4e2b95e15ce8abf732db51ec771f1ca2edccf22a46c729ac56472";
    let sender = "0x336Cd992a83242D91f556C1F7e855AcA366193e0";
    let user_op_hash = "0x7762e85586107f2bca787a9163b71f0584eabd84258a93cca0e896589a193571";

    // WebAuthn Specific data.
    let authenticator_data =
        "0x49960de5880e8c687434170f6476605b8fe4aeb9a28632c7995cf3ba831d97630500000000";
    let client_data_json = r#"{"type":"webauthn.get","challenge":"fRMDMfFrs9K8PXLbAoedB0XURSWS5Wcj3osnzx7gBsc","origin":"https://sign.coinbase.com","crossOrigin":false}"#;

    // secp256p1 (nist256p1) private key.
    let passkey_private = "0x03d99692017473e2d631945a812607b23269d85721e0f370b8d3e7d29a874fd2";

    // Step 1. Encode UserOperation hash.
    let encoded_user_op_hash = get_encoded_hash(
        &chain_id[..],
        code_address,
        code_name,
        code_version,
        type_hash,
        domain_separator_hash,
        sender,
        user_op_hash,
    )
    .unwrap();
    assert_eq!(
        encoded_user_op_hash.to_hex_prefixed(),
        "0x7d130331f16bb3d2bc3d72db02879d0745d4452592e56723de8b27cf1ee006c7"
    );

    // Step 2. Generate WebAuthn message hash.
    let webauthn = WebAuthnMessage::new(authenticator_data, client_data_json.to_string()).unwrap();
    assert_eq!(
        webauthn.message_hash.to_hex_prefixed(),
        "0x0b8772b952d27e2a8d6a51b0177b18b8ed1c3ebede2d0d7992646841b25322ac"
    );

    // Step 3. Sign WebAuthn message hash.
    let private_key = nist256p1::KeyPair::try_from(passkey_private).unwrap();
    let message_signature = private_key.sign(webauthn.message_hash).unwrap();
    assert_eq!(
        message_signature.r().to_hex_prefixed(),
        "0x73f8762dd6fb0eb39aea829525658fca612d1c433db6381c9d63a52ee15a26be"
    );
    assert_eq!(
        message_signature.s().to_hex_prefixed(),
        "0xe091f452b74519a2894a96d142bdd1888ac6513f5dff76e48c0312144ef9b382"
    );

    // Step 4. Encode WebAuthn Auth struct.
    let normalized_z = NormalizedS::new(U256::from_big_endian(message_signature.s()));
    let webauthn = WebAuthnAuth::new(
        webauthn,
        U256::from_big_endian(message_signature.r()),
        normalized_z,
    )
    .unwrap();
    assert_eq!(
        webauthn.authenticator_data.to_hex_prefixed(),
        authenticator_data
    );
    assert_eq!(webauthn.client_data_json, client_data_json);
    assert_eq!(webauthn.challenge_index, U256::from(23_u32));
    assert_eq!(webauthn.type_index, U256::from(1_u32));
    assert_eq!(
        webauthn.r.to_big_endian().to_hex_prefixed(),
        "0x73f8762dd6fb0eb39aea829525658fca612d1c433db6381c9d63a52ee15a26be"
    );
    assert_eq!(
        webauthn.s.as_ref().to_big_endian().to_hex_prefixed(),
        "0x1f6e0bac48bae65e76b5692ebd422e773220a96e491827a067b6b8aead6971cf"
    );

    let encoded_webauthn = webauthn.abi_encode();
    let expected = "0x000000000000000000000000000000000000000000000000000000000000002000000000000000000000000000000000000000000000000000000000000000c000000000000000000000000000000000000000000000000000000000000001200000000000000000000000000000000000000000000000000000000000000017000000000000000000000000000000000000000000000000000000000000000173f8762dd6fb0eb39aea829525658fca612d1c433db6381c9d63a52ee15a26be1f6e0bac48bae65e76b5692ebd422e773220a96e491827a067b6b8aead6971cf000000000000000000000000000000000000000000000000000000000000002549960de5880e8c687434170f6476605b8fe4aeb9a28632c7995cf3ba831d97630500000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000008a7b2274797065223a22776562617574686e2e676574222c226368616c6c656e6765223a2266524d444d66467273394b3850584c62416f656442305855525357533557636a336f736e7a783767427363222c226f726967696e223a2268747470733a2f2f7369676e2e636f696e626173652e636f6d222c2263726f73734f726967696e223a66616c73657d00000000000000000000000000000000000000000000";
    assert_eq!(encoded_webauthn.to_hex_prefixed(), expected);
}
