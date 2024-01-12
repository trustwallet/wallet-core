// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::greenfield::{make_amount, PRIVATE_KEY_15560, PRIVATE_KEY_1686};
use tw_any_coin::test_utils::sign_utils::AnySignerHelper;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_proto::Common::Proto::SigningError;
use tw_proto::Greenfield::Proto;
use Proto::mod_Message::OneOfmessage_oneof as MessageEnum;

/// **Testnet**
/// Successfully broadcasted: https://greenfieldscan.com/tx/0x9f895cf2dd64fb1f428cefcf2a6585a813c3540fc9fe1ef42db1da2cb1df55ab
#[test]
fn test_greenfield_sign_send_order_9f895c() {
    let send_order = Proto::mod_Message::Send {
        from_address: "0x9d1d97aDFcd324Bbd603D3872BD78e04098510b1".into(),
        to_address: "0x280b27f3676db1C4475EE10F75D510Eb527fd155".into(),
        amounts: vec![make_amount("BNB", "1000000000000000")],
        ..Proto::mod_Message::Send::default()
    };

    let input = Proto::SigningInput {
        signing_mode: Proto::SigningMode::Eip712,
        account_number: 15560,
        eth_chain_id: "5600".into(),
        cosmos_chain_id: "greenfield_5600-1".into(),
        fee: Some(Proto::Fee {
            amounts: vec![make_amount("BNB", "2000000000000000")],
            gas: 200000,
        }),
        sequence: 2,
        messages: vec![Proto::Message {
            message_oneof: MessageEnum::send_coins_message(send_order),
        }],
        private_key: PRIVATE_KEY_15560.decode_hex().unwrap().into(),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Greenfield, input);

    let expected = r#"{"mode":"BROADCAST_MODE_SYNC","tx_bytes":"CpQBCpEBChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEnEKKjB4OWQxZDk3YURGY2QzMjRCYmQ2MDNEMzg3MkJENzhlMDQwOTg1MTBiMRIqMHgyODBiMjdmMzY3NmRiMUM0NDc1RUUxMEY3NUQ1MTBFYjUyN2ZkMTU1GhcKA0JOQhIQMTAwMDAwMDAwMDAwMDAwMBJ5ClgKTQomL2Nvc21vcy5jcnlwdG8uZXRoLmV0aHNlY3AyNTZrMS5QdWJLZXkSIwohAnnvNAZNoQ2wRjxwSAYWugIHA+w6RQJt73vr0ggvXW/IEgUKAwjIBRgCEh0KFwoDQk5CEhAyMDAwMDAwMDAwMDAwMDAwEMCaDBpByzpGhKmRAUo4egSoW1kifrt5VnwgJa3cspa0yoVun4ENO1JvKg0PrWrRsSazuVFvizvgIKfMqcA8489H9BmbbRs="}"#;
    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.serialized, expected);
    assert_eq!(output.signature.to_hex(), "cb3a4684a991014a387a04a85b59227ebb79567c2025addcb296b4ca856e9f810d3b526f2a0d0fad6ad1b126b3b9516f8b3be020a7cca9c03ce3cf47f4199b6d1b");
}

/// Successfully broadcasted: https://greenfieldscan.com/tx/DA50E269A13D2C42770CD6A4C35EA7EBEE46C677465ED92985215E67C340E28D
#[test]
fn test_greenfield_sign_send_order_b798ab() {
    let send_order = Proto::mod_Message::Send {
        from_address: "0xF1DB7D5256d721fE3C144F5c1ed4b3A3A94Dc444".into(),
        to_address: "0x9d1d97aDFcd324Bbd603D3872BD78e04098510b1".into(),
        amounts: vec![make_amount("BNB", "100000000000000")],
        ..Proto::mod_Message::Send::default()
    };

    let input = Proto::SigningInput {
        signing_mode: Proto::SigningMode::Eip712,
        account_number: 1686,
        eth_chain_id: "1017".into(),
        cosmos_chain_id: "greenfield_1017-1".into(),
        fee: Some(Proto::Fee {
            amounts: vec![make_amount("BNB", "6000000000000")],
            gas: 1200,
        }),
        sequence: 0,
        messages: vec![Proto::Message {
            message_oneof: MessageEnum::send_coins_message(send_order),
        }],
        private_key: PRIVATE_KEY_1686.decode_hex().unwrap().into(),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Greenfield, input);

    let expected = r#"{"mode":"BROADCAST_MODE_SYNC","tx_bytes":"CpMBCpABChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEnAKKjB4RjFEQjdENTI1NmQ3MjFmRTNDMTQ0RjVjMWVkNGIzQTNBOTREYzQ0NBIqMHg5ZDFkOTdhREZjZDMyNEJiZDYwM0QzODcyQkQ3OGUwNDA5ODUxMGIxGhYKA0JOQhIPMTAwMDAwMDAwMDAwMDAwEnMKVgpNCiYvY29zbW9zLmNyeXB0by5ldGguZXRoc2VjcDI1NmsxLlB1YktleRIjCiEDCbufQiEfB0Z6DbH3BQTjvz/DjTWbiziPi702Ws2BbJYSBQoDCMgFEhkKFAoDQk5CEg02MDAwMDAwMDAwMDAwELAJGkGvQ8FyO09ECauebgRGWal94uZtp0K1vkqnl0xHCzzEylWLS7PJcBepUCkIkG4xnswnN4n7EqHZoZ+qVTH8Ue7fGw=="}"#;
    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.serialized, expected);
    assert_eq!(output.signature.to_hex(), "af43c1723b4f4409ab9e6e044659a97de2e66da742b5be4aa7974c470b3cc4ca558b4bb3c97017a9502908906e319ecc273789fb12a1d9a19faa5531fc51eedf1b");

    let expected_signature_json = r#"[{"pub_key":{"type":"/cosmos.crypto.eth.ethsecp256k1.PubKey","value":"Awm7n0IhHwdGeg2x9wUE478/w401m4s4j4u9NlrNgWyW"},"signature":"r0PBcjtPRAmrnm4ERlmpfeLmbadCtb5Kp5dMRws8xMpVi0uzyXAXqVApCJBuMZ7MJzeJ+xKh2aGfqlUx/FHu3xs="}]"#;
    assert_eq!(output.signature_json, expected_signature_json);
}

/// **Testnet**
/// Successfully broadcasted Greenfield: https://greenfieldscan.com/tx/38C29C530A74946CFD22EE07DA642F5EF9399BC9AEA59EC56A9B5BE16DE16CE7
/// BSC (parent transaction): https://testnet.bscscan.com/tx/0x7f73c8a362e14e58cb5e0ec17616afc50eff7aa398db472383a6d017c8a5861a
#[test]
fn test_greenfield_sign_transfer_out() {
    let transfer_out = Proto::mod_Message::BridgeTransferOut {
        from_address: "0x9d1d97aDFcd324Bbd603D3872BD78e04098510b1".into(),
        to_address: "0x9d1d97aDFcd324Bbd603D3872BD78e04098510b1".into(),
        amount: Some(make_amount("BNB", "5670000000000000")),
        ..Proto::mod_Message::BridgeTransferOut::default()
    };

    let input = Proto::SigningInput {
        signing_mode: Proto::SigningMode::Eip712,
        account_number: 15560,
        eth_chain_id: "5600".into(),
        cosmos_chain_id: "greenfield_5600-1".into(),
        fee: Some(Proto::Fee {
            amounts: vec![make_amount("BNB", "6000000000000")],
            gas: 1200,
        }),
        sequence: 7,
        messages: vec![Proto::Message {
            message_oneof: MessageEnum::bridge_transfer_out(transfer_out),
        }],
        private_key: PRIVATE_KEY_15560.decode_hex().unwrap().into(),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Greenfield, input);

    let expected = r#"{"mode":"BROADCAST_MODE_SYNC","tx_bytes":"CpkBCpYBCiEvZ3JlZW5maWVsZC5icmlkZ2UuTXNnVHJhbnNmZXJPdXQScQoqMHg5ZDFkOTdhREZjZDMyNEJiZDYwM0QzODcyQkQ3OGUwNDA5ODUxMGIxEioweDlkMWQ5N2FERmNkMzI0QmJkNjAzRDM4NzJCRDc4ZTA0MDk4NTEwYjEaFwoDQk5CEhA1NjcwMDAwMDAwMDAwMDAwEnUKWApNCiYvY29zbW9zLmNyeXB0by5ldGguZXRoc2VjcDI1NmsxLlB1YktleRIjCiECee80Bk2hDbBGPHBIBha6AgcD7DpFAm3ve+vSCC9db8gSBQoDCMgFGAcSGQoUCgNCTkISDTYwMDAwMDAwMDAwMDAQsAkaQc4DDByhu80Uy/M3sQePvAmhmbFWZeGq359rugtskEiXKfCzSB86XmBi+l+Q5ETDS2folMxbufHSE8gM4WBCHzgc"}"#;
    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.serialized, expected);
    assert_eq!(output.signature.to_hex(), "ce030c1ca1bbcd14cbf337b1078fbc09a199b15665e1aadf9f6bba0b6c90489729f0b3481f3a5e6062fa5f90e444c34b67e894cc5bb9f1d213c80ce160421f381c");
}

#[test]
fn test_greenfield_sign_no_messages() {
    let input = Proto::SigningInput {
        signing_mode: Proto::SigningMode::Eip712,
        account_number: 15560,
        eth_chain_id: "5600".into(),
        cosmos_chain_id: "greenfield_5600-1".into(),
        fee: Some(Proto::Fee {
            amounts: vec![make_amount("BNB", "6000000000000")],
            gas: 1200,
        }),
        sequence: 3,
        messages: Vec::default(),
        private_key: PRIVATE_KEY_15560.decode_hex().unwrap().into(),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Greenfield, input);

    assert_eq!(output.error, SigningError::Error_invalid_params);
}

/// Successfully broadcasted: https://greenfieldscan.com/tx/82E967B702AFFA9227C3E987CD35B85DD862AE1C49F3027A639E354480138C40
#[test]
fn test_greenfield_sign_multiple_messages_82e967() {
    let send_order1 = Proto::mod_Message::Send {
        from_address: "0xF1DB7D5256d721fE3C144F5c1ed4b3A3A94Dc444".into(),
        to_address: "0x9d1d97aDFcd324Bbd603D3872BD78e04098510b1".into(),
        amounts: vec![make_amount("BNB", "100000000000000")],
        ..Proto::mod_Message::Send::default()
    };
    let send_order2 = Proto::mod_Message::Send {
        from_address: "0xF1DB7D5256d721fE3C144F5c1ed4b3A3A94Dc444".into(),
        to_address: "0x280b27f3676db1C4475EE10F75D510Eb527fd155".into(),
        amounts: vec![make_amount("BNB", "90000000000000")],
        ..Proto::mod_Message::Send::default()
    };

    let input = Proto::SigningInput {
        signing_mode: Proto::SigningMode::Eip712,
        account_number: 1686,
        eth_chain_id: "1017".into(),
        cosmos_chain_id: "greenfield_1017-1".into(),
        fee: Some(Proto::Fee {
            amounts: vec![make_amount("BNB", "15000000000000")],
            gas: 3000,
        }),
        sequence: 1,
        messages: vec![
            Proto::Message {
                message_oneof: MessageEnum::send_coins_message(send_order1),
            },
            Proto::Message {
                message_oneof: MessageEnum::send_coins_message(send_order2),
            },
        ],
        private_key: PRIVATE_KEY_1686.decode_hex().unwrap().into(),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Greenfield, input);

    let expected = r#"{"mode":"BROADCAST_MODE_SYNC","tx_bytes":"CqUCCpABChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEnAKKjB4RjFEQjdENTI1NmQ3MjFmRTNDMTQ0RjVjMWVkNGIzQTNBOTREYzQ0NBIqMHg5ZDFkOTdhREZjZDMyNEJiZDYwM0QzODcyQkQ3OGUwNDA5ODUxMGIxGhYKA0JOQhIPMTAwMDAwMDAwMDAwMDAwCo8BChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEm8KKjB4RjFEQjdENTI1NmQ3MjFmRTNDMTQ0RjVjMWVkNGIzQTNBOTREYzQ0NBIqMHgyODBiMjdmMzY3NmRiMUM0NDc1RUUxMEY3NUQ1MTBFYjUyN2ZkMTU1GhUKA0JOQhIOOTAwMDAwMDAwMDAwMDASdgpYCk0KJi9jb3Ntb3MuY3J5cHRvLmV0aC5ldGhzZWNwMjU2azEuUHViS2V5EiMKIQMJu59CIR8HRnoNsfcFBOO/P8ONNZuLOI+LvTZazYFslhIFCgMIyAUYARIaChUKA0JOQhIOMTUwMDAwMDAwMDAwMDAQuBcaQS23kxqz7R/QZTegc1Ic1U0g+YUm9r8Y8Zx0+sHzKREYWg04yxy/EKvVY7LIqATQ2zi4rb9TZSIyXuMcyGk2JQYb"}"#;
    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.serialized, expected);
    assert_eq!(output.signature.to_hex(), "2db7931ab3ed1fd06537a073521cd54d20f98526f6bf18f19c74fac1f32911185a0d38cb1cbf10abd563b2c8a804d0db38b8adbf536522325ee31cc8693625061b");

    let expected_signature_json = r#"[{"pub_key":{"type":"/cosmos.crypto.eth.ethsecp256k1.PubKey","value":"Awm7n0IhHwdGeg2x9wUE478/w401m4s4j4u9NlrNgWyW"},"signature":"LbeTGrPtH9BlN6BzUhzVTSD5hSb2vxjxnHT6wfMpERhaDTjLHL8Qq9VjssioBNDbOLitv1NlIjJe4xzIaTYlBhs="}]"#;
    assert_eq!(output.signature_json, expected_signature_json);
}

/// Successfully broadcasted: https://greenfieldscan.com/tx/E3539ED65D195BE8CE43C1A6567384C758E40432B8FDC7C25841E21F5F53375B
#[test]
fn test_greenfield_sign_multiple_messages_e3539e() {
    let send_order = Proto::mod_Message::Send {
        from_address: "0xF1DB7D5256d721fE3C144F5c1ed4b3A3A94Dc444".into(),
        to_address: "0x9d1d97aDFcd324Bbd603D3872BD78e04098510b1".into(),
        amounts: vec![make_amount("BNB", "100000000000000")],
        ..Proto::mod_Message::Send::default()
    };
    let transfer_out = Proto::mod_Message::BridgeTransferOut {
        from_address: "0xF1DB7D5256d721fE3C144F5c1ed4b3A3A94Dc444".into(),
        to_address: "0xF1DB7D5256d721fE3C144F5c1ed4b3A3A94Dc444".into(),
        amount: Some(make_amount("BNB", "200000000000000")),
        ..Proto::mod_Message::BridgeTransferOut::default()
    };

    let input = Proto::SigningInput {
        signing_mode: Proto::SigningMode::Eip712,
        account_number: 1686,
        eth_chain_id: "1017".into(),
        cosmos_chain_id: "greenfield_1017-1".into(),
        fee: Some(Proto::Fee {
            amounts: vec![make_amount("BNB", "15000000000000")],
            gas: 3000,
        }),
        sequence: 2,
        messages: vec![
            Proto::Message {
                message_oneof: MessageEnum::send_coins_message(send_order),
            },
            Proto::Message {
                message_oneof: MessageEnum::bridge_transfer_out(transfer_out),
            },
        ],
        private_key: PRIVATE_KEY_1686.decode_hex().unwrap().into(),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Greenfield, input);

    let expected = r#"{"mode":"BROADCAST_MODE_SYNC","tx_bytes":"CqsCCpABChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEnAKKjB4RjFEQjdENTI1NmQ3MjFmRTNDMTQ0RjVjMWVkNGIzQTNBOTREYzQ0NBIqMHg5ZDFkOTdhREZjZDMyNEJiZDYwM0QzODcyQkQ3OGUwNDA5ODUxMGIxGhYKA0JOQhIPMTAwMDAwMDAwMDAwMDAwCpUBCiEvZ3JlZW5maWVsZC5icmlkZ2UuTXNnVHJhbnNmZXJPdXQScAoqMHhGMURCN0Q1MjU2ZDcyMWZFM0MxNDRGNWMxZWQ0YjNBM0E5NERjNDQ0EioweEYxREI3RDUyNTZkNzIxZkUzQzE0NEY1YzFlZDRiM0EzQTk0RGM0NDQaFgoDQk5CEg8yMDAwMDAwMDAwMDAwMDASdgpYCk0KJi9jb3Ntb3MuY3J5cHRvLmV0aC5ldGhzZWNwMjU2azEuUHViS2V5EiMKIQMJu59CIR8HRnoNsfcFBOO/P8ONNZuLOI+LvTZazYFslhIFCgMIyAUYAhIaChUKA0JOQhIOMTUwMDAwMDAwMDAwMDAQuBcaQfkHNYUs2oMR0/ZauYXv8Nd/9bbrQ4w323P4GpyEO0j8IadMVE8TkmI6lIx4H/hPSbpG9wRrqhZZfzTsqSv5rDgb"}"#;
    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.serialized, expected);
    assert_eq!(output.signature.to_hex(), "f90735852cda8311d3f65ab985eff0d77ff5b6eb438c37db73f81a9c843b48fc21a74c544f1392623a948c781ff84f49ba46f7046baa16597f34eca92bf9ac381b");

    let expected_signature_json = r#"[{"pub_key":{"type":"/cosmos.crypto.eth.ethsecp256k1.PubKey","value":"Awm7n0IhHwdGeg2x9wUE478/w401m4s4j4u9NlrNgWyW"},"signature":"+Qc1hSzagxHT9lq5he/w13/1tutDjDfbc/ganIQ7SPwhp0xUTxOSYjqUjHgf+E9Jukb3BGuqFll/NOypK/msOBs="}]"#;
    assert_eq!(output.signature_json, expected_signature_json);
}
