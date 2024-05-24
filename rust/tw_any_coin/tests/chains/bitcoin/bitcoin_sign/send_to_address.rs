use crate::chains::common::bitcoin::input::out_point;
use crate::chains::common::bitcoin::{dust_threshold, input, output, sign, DUST, SIGHASH_ALL};
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::DecodeHex;
use tw_proto::BitcoinV3::Proto;

// #[test]
// fn send_to_p2sh_address() {
//     let alice_private_key = hex("57a64865bce5d4855e99b1cce13327c46171434f2d72eeaf9da53ee075e7f90a");
//     let alice_pubkey = hex("028d7dce6d72fb8f7af9566616c6436349c67ad379f2404dd66fe7085fe0fba28f");
//     let bob_pubkey = hex("025a0af1510f0f24d40dd00d7c0e51605ca504bbc177c3e19b065f373a1efdd22f");
//
//     let txid: Vec<u8> = hex("181c84965c9ea86a5fac32fdbd5f73a21a7a9e749fb6ab97e273af2329f6b911")
//         .into_iter()
//         .rev()
//         .collect();
//
//     let tx1 = Proto::Input {
//         txid: txid.as_slice().into(),
//         vout: 0,
//         value: 10_000,
//         sighash_type: UtxoProto::SighashType::All,
//         to_recipient: ProtoInputRecipient::builder(Proto::mod_Input::InputBuilder {
//             variant: ProtoInputBuilder::p2pkh(alice_pubkey.as_slice().into()),
//         }),
//         ..Default::default()
//     };
//
//     // Create the P2SH address.
//     let recipient = PublicKey::from_slice(&bob_pubkey).unwrap();
//     // We use a simple P2PKH as the redeem script.
//     let redeem_script = ScriptBuf::new_p2pkh(&recipient.pubkey_hash());
//     let address = Address::p2sh(&redeem_script, bitcoin::Network::Bitcoin).unwrap();
//
//     // The output variant is derived from the specified address.
//     let out1 = Proto::Output {
//         value: 1_000,
//         to_recipient: ProtoOutputRecipient::from_address(address.to_string().into()),
//     };
//
//     let signing = Proto::SigningInput {
//         private_key: alice_private_key.as_slice().into(),
//         inputs: vec![tx1],
//         outputs: vec![out1],
//         input_selector: UtxoProto::InputSelector::UseAll,
//         disable_change_output: true,
//         ..Default::default()
//     };
//
//     let signed = BitcoinEntry.sign(&coin, signing);
//     assert_eq!(signed.error, Proto::Error::OK);
//
//     let tx = signed.transaction.as_ref().unwrap();
//     assert_eq!(tx.inputs.len(), 1);
//     assert_eq!(tx.outputs.len(), 1);
//
//     // The expected P2SH scriptPubkey
//     let expected = ScriptBuf::new_p2sh(&redeem_script.script_hash());
//
//     assert_eq!(tx.outputs[0].value, 1_000);
//     assert_eq!(tx.outputs[0].script_pubkey, expected.as_bytes());
//     assert!(tx.outputs[0].taproot_payload.is_empty());
//     assert!(tx.outputs[0].control_block.is_empty());
// }

#[test]
fn send_to_p2pkh_address() {
    let alice_private_key = "57a64865bce5d4855e99b1cce13327c46171434f2d72eeaf9da53ee075e7f90a"
        .decode_hex()
        .unwrap();
    let alice_pubkey = "028d7dce6d72fb8f7af9566616c6436349c67ad379f2404dd66fe7085fe0fba28f"
        .decode_hex()
        .unwrap();
    let bob_address = "12C2h5hXPxyrdvnYUBFaBGFnNNYjpWXhPX";

    let txid = "181c84965c9ea86a5fac32fdbd5f73a21a7a9e749fb6ab97e273af2329f6b911";
    let tx1 = Proto::Input {
        out_point: out_point(txid, 0),
        value: 10_000,
        sighash_type: SIGHASH_ALL,
        claiming_script: input::p2pkh(alice_pubkey.clone()),
        ..Default::default()
    };

    // The output variant is derived from the specified address.
    let out1 = Proto::Output {
        value: 1_000,
        to_recipient: output::to_address(bob_address),
    };

    let signing = Proto::SigningInput {
        private_keys: vec![alice_private_key.into()],
        inputs: vec![tx1],
        outputs: vec![out1],
        input_selector: Proto::InputSelector::UseAll,
        dust_policy: dust_threshold(DUST),
        ..Default::default()
    };

    sign::BitcoinSignHelper::new(&signing)
        .coin(CoinType::Bitcoin)
        .sign(sign::Expected {
            encoded: "020000000111b9f62923af73e297abb69f749e7a1aa2735fbdfd32ac5f6aa89e5c96841c18000000006b483045022100a42afe29055e70d912c7bc3b113a6a58216ad0eafc7d73ae447df9044e347d900220530c5b4721c0c832d06be4921369792da9187df6fcf17bcf6c77a27d5ed85a200121028d7dce6d72fb8f7af9566616c6436349c67ad379f2404dd66fe7085fe0fba28fffffffff01e8030000000000001976a9140d0e1cec6c2babe8badde5e9b3dea667da90036d88ac00000000",
            txid: "ff0901ef4796e9a0a82c4f8a6edebe9112cc54c4eefac77f6d4c4dd267b0da86",
            inputs: vec![10_000],
            outputs: vec![1_000],
            vsize: 192,
            fee: 9_000,
        });
}
//
// #[test]
// fn send_to_p2wsh_address() {
//     let alice_private_key = hex("57a64865bce5d4855e99b1cce13327c46171434f2d72eeaf9da53ee075e7f90a");
//     let alice_pubkey = hex("028d7dce6d72fb8f7af9566616c6436349c67ad379f2404dd66fe7085fe0fba28f");
//     let bob_pubkey = hex("025a0af1510f0f24d40dd00d7c0e51605ca504bbc177c3e19b065f373a1efdd22f");
//
//     let txid: Vec<u8> = hex("181c84965c9ea86a5fac32fdbd5f73a21a7a9e749fb6ab97e273af2329f6b911")
//         .into_iter()
//         .rev()
//         .collect();
//
//     let tx1 = Proto::Input {
//         txid: txid.as_slice().into(),
//         vout: 0,
//         value: 10_000,
//         sighash_type: UtxoProto::SighashType::All,
//         to_recipient: ProtoInputRecipient::builder(Proto::mod_Input::InputBuilder {
//             variant: ProtoInputBuilder::p2pkh(alice_pubkey.as_slice().into()),
//         }),
//         ..Default::default()
//     };
//
//     // Create the P2WSH address.
//     let recipient = PublicKey::from_slice(&bob_pubkey).unwrap();
//     // We use a simple P2PKH as the redeem script.
//     let redeem_script = ScriptBuf::new_p2pkh(&recipient.pubkey_hash());
//     let address = Address::p2wsh(&redeem_script, bitcoin::Network::Bitcoin);
//
//     // The output variant is derived from the specified address.
//     let out1 = Proto::Output {
//         value: 1_000,
//         to_recipient: ProtoOutputRecipient::from_address(address.to_string().into()),
//     };
//
//     let signing = Proto::SigningInput {
//         private_key: alice_private_key.as_slice().into(),
//         inputs: vec![tx1],
//         outputs: vec![out1],
//         input_selector: UtxoProto::InputSelector::UseAll,
//         disable_change_output: true,
//         ..Default::default()
//     };
//
//     let signed = BitcoinEntry.sign(&coin, signing);
//     assert_eq!(signed.error, Proto::Error::OK);
//
//     let tx = signed.transaction.as_ref().unwrap();
//     assert_eq!(tx.inputs.len(), 1);
//     assert_eq!(tx.outputs.len(), 1);
//
//     // The expected Pw2SH scriptPubkey
//     let expected = ScriptBuf::new_v0_p2wsh(&redeem_script.wscript_hash());
//
//     assert_eq!(tx.outputs[0].value, 1_000);
//     assert_eq!(tx.outputs[0].script_pubkey, expected.as_bytes());
//     assert!(tx.outputs[0].taproot_payload.is_empty());
//     assert!(tx.outputs[0].control_block.is_empty());
// }

#[test]
fn send_to_p2wpkh_address() {
    let alice_private_key = "57a64865bce5d4855e99b1cce13327c46171434f2d72eeaf9da53ee075e7f90a"
        .decode_hex()
        .unwrap();
    let alice_pubkey = "028d7dce6d72fb8f7af9566616c6436349c67ad379f2404dd66fe7085fe0fba28f"
        .decode_hex()
        .unwrap();
    let bob_address = "bc1qp58pemrv9w473wkauh5m8h4xvldfqqmdk7s5ju";

    let txid = "181c84965c9ea86a5fac32fdbd5f73a21a7a9e749fb6ab97e273af2329f6b911";
    let tx1 = Proto::Input {
        out_point: out_point(txid, 0),
        value: 10_000,
        sighash_type: SIGHASH_ALL,
        claiming_script: input::p2pkh(alice_pubkey),
        ..Default::default()
    };

    // The output variant is derived from the specified address.
    let out1 = Proto::Output {
        value: 1_000,
        to_recipient: output::to_address(bob_address),
    };

    let signing = Proto::SigningInput {
        private_keys: vec![alice_private_key.into()],
        inputs: vec![tx1],
        outputs: vec![out1],
        input_selector: Proto::InputSelector::UseAll,
        dust_policy: dust_threshold(DUST),
        ..Default::default()
    };

    sign::BitcoinSignHelper::new(&signing)
        .coin(CoinType::Bitcoin)
        .sign(sign::Expected {
            encoded: "020000000111b9f62923af73e297abb69f749e7a1aa2735fbdfd32ac5f6aa89e5c96841c18000000006b483045022100cdee6087a59c27f670d04d2782d139b202abdb936643bff8c0a46ec726bbc7ec022072241248659a582ef3460bdd232936fcea77b25b0e024f3841f8ba5e89efd31a0121028d7dce6d72fb8f7af9566616c6436349c67ad379f2404dd66fe7085fe0fba28fffffffff01e8030000000000001600140d0e1cec6c2babe8badde5e9b3dea667da90036d00000000",
            txid: "711566e32e03c01106f019bf1911958979441f5aec5f58b7a368549d4cac569c",
            inputs: vec![10_000],
            outputs: vec![1_000],
            vsize: 189,
            fee: 9_000,
        });
}

#[test]
fn send_to_p2tr_key_path_address() {
    let alice_private_key = "57a64865bce5d4855e99b1cce13327c46171434f2d72eeaf9da53ee075e7f90a"
        .decode_hex()
        .unwrap();
    let alice_pubkey = "028d7dce6d72fb8f7af9566616c6436349c67ad379f2404dd66fe7085fe0fba28f"
        .decode_hex()
        .unwrap();
    let bob_address = "bc1pps3sf8cl8xuy42s9gnyacez9wlwjxpyf32lrxlac7fw4wnuf6e9s0v066l";

    let txid = "181c84965c9ea86a5fac32fdbd5f73a21a7a9e749fb6ab97e273af2329f6b911";
    let tx1 = Proto::Input {
        out_point: out_point(txid, 0),
        value: 10_000,
        sighash_type: SIGHASH_ALL,
        claiming_script: input::p2pkh(alice_pubkey),
        ..Default::default()
    };

    // The output variant is derived from the specified address.
    let out1 = Proto::Output {
        value: 1_000,
        to_recipient: output::to_address(bob_address),
    };

    let signing = Proto::SigningInput {
        private_keys: vec![alice_private_key.into()],
        inputs: vec![tx1],
        outputs: vec![out1],
        input_selector: Proto::InputSelector::UseAll,
        dust_policy: dust_threshold(DUST),
        ..Default::default()
    };

    sign::BitcoinSignHelper::new(&signing)
        .coin(CoinType::Bitcoin)
        .sign(sign::Expected {
            encoded: "020000000111b9f62923af73e297abb69f749e7a1aa2735fbdfd32ac5f6aa89e5c96841c18000000006b483045022100c85b0030e0f18fdb6bf4b6085945732ed6354aa45beb4da3cc8fb76378ab424a02204aef0ccfd9270549c302ca8fca2aed9e6d459b07e7a1af8477f2137e5ca070f30121028d7dce6d72fb8f7af9566616c6436349c67ad379f2404dd66fe7085fe0fba28fffffffff01e8030000000000002251200c23049f1f39b84aaa0544c9dc644577dd2304898abe337fb8f25d574f89d64b00000000",
            txid: "b37fdb67c208aeb9a42b329eeafe52d9ff870341c8bdc2e09b2a8de64ea13d2a",
            inputs: vec![10_000],
            outputs: vec![1_000],
            vsize: 201,
            fee: 9_000,
        });
}
