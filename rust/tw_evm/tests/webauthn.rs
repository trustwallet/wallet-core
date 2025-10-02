// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_encoding::hex::ToHex;
use tw_evm::modules::biz::get_encoded_hash;
use tw_evm::modules::webauthn::{NormalizedS, WebAuthnAuth, WebAuthnMessage};
use tw_keypair::ecdsa::nist256p1;
use tw_keypair::traits::SigningKeyTrait;
use tw_number::U256;

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
    let user_op_hash = "0xeec8a79003e04300ad8430d72adb8d933a485c6d9abfc98c8a27ce0a16366f9b";

    // WebAuthn Specific data.
    let authenticator_data =
        "49960de5880e8c687434170f6476605b8fe4aeb9a28632c7995cf3ba831d97630500000000";
    let client_data_json = r#"{"type":"webauthn.get","challenge":"GFQOrZ9hRIfwzt9nUt8zxrl6vVQmMnhLK9klXhDFP6c","origin":"https://sign.coinbase.com","crossOrigin":false}"#;

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
        "0x18540ead9f614487f0cedf6752df33c6b97abd542632784b2bd9255e10c53fa7"
    );

    // Step 2. Generate WebAuthn message hash.
    let webauthn = WebAuthnMessage::new(authenticator_data, client_data_json.to_string()).unwrap();
    assert_eq!(
        webauthn.message_hash.to_hex_prefixed(),
        "0x03301a2660d1cea1c268c237ccd63ecfc8ca32573d7a6cf702cbbbaab9d7ac2a"
    );

    // Step 3. Sign WebAuthn message hash.
    let private_key = nist256p1::KeyPair::try_from(passkey_private).unwrap();
    let message_signature = private_key.sign(webauthn.message_hash).unwrap();
    assert_eq!(
        message_signature.r().to_hex_prefixed(),
        "0x3d0ece361a0a304ed87dbcccbcd0ae5924ed497bf0500cc850e57b76d4f7d6c1"
    );
    assert_eq!(
        message_signature.s().to_hex_prefixed(),
        "0xa87345592418c98dcff8e159f5e0d9fefe8636d95c784a1a740e514ab2c7fb87"
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
        "0x49960de5880e8c687434170f6476605b8fe4aeb9a28632c7995cf3ba831d97630500000000"
    );
    assert_eq!(
        webauthn.client_data_json,
        r#"{"type":"webauthn.get","challenge":"GFQOrZ9hRIfwzt9nUt8zxrl6vVQmMnhLK9klXhDFP6c","origin":"https://sign.coinbase.com","crossOrigin":false}"#
    );
    assert_eq!(webauthn.challenge_index, U256::from(23_u32));
    assert_eq!(webauthn.type_index, U256::from(1_u32));
    assert_eq!(
        webauthn.r.to_big_endian().to_hex_prefixed(),
        "0x3d0ece361a0a304ed87dbcccbcd0ae5924ed497bf0500cc850e57b76d4f7d6c1"
    );
    assert_eq!(
        webauthn.s.as_ref().to_big_endian().to_hex_prefixed(),
        "0x578cbaa5dbe7367330071ea60a1f2600be60c3d44a9f546a7fab7978499b29ca"
    );

    let encoded_webauthn = webauthn.abi_encode();
    let expected = "0x000000000000000000000000000000000000000000000000000000000000002000000000000000000000000000000000000000000000000000000000000000c00000000000000000000000000000000000000000000000000000000000000120000000000000000000000000000000000000000000000000000000000000001700000000000000000000000000000000000000000000000000000000000000013d0ece361a0a304ed87dbcccbcd0ae5924ed497bf0500cc850e57b76d4f7d6c1578cbaa5dbe7367330071ea60a1f2600be60c3d44a9f546a7fab7978499b29ca000000000000000000000000000000000000000000000000000000000000002549960de5880e8c687434170f6476605b8fe4aeb9a28632c7995cf3ba831d97630500000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000008a7b2274797065223a22776562617574686e2e676574222c226368616c6c656e6765223a224746514f725a3968524966777a74396e5574387a78726c367656516d4d6e684c4b396b6c58684446503663222c226f726967696e223a2268747470733a2f2f7369676e2e636f696e626173652e636f6d222c2263726f73734f726967696e223a66616c73657d00000000000000000000000000000000000000000000";
    assert_eq!(encoded_webauthn.to_hex_prefixed(), expected);
}
