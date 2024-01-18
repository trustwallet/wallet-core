mod common;

use common::{hex, MINER_FEE, ONE_BTC};
use tw_bitcoin::aliases::*;
use tw_bitcoin::entry::BitcoinEntry;
use tw_coin_entry::coin_entry::CoinEntry;
use tw_coin_entry::test_utils::test_context::TestCoinContext;
use tw_proto::BitcoinV2::Proto;
use tw_proto::Utxo::Proto as UtxoProto;

const SAT_VBYTE: u64 = 50;

#[test]
fn input_selection_default_values() {
    let coin = TestCoinContext::default();

    let alice_private_key = hex("57a64865bce5d4855e99b1cce13327c46171434f2d72eeaf9da53ee075e7f90a");
    let alice_pubkey = hex("028d7dce6d72fb8f7af9566616c6436349c67ad379f2404dd66fe7085fe0fba28f");
    let bob_pubkey = hex("025a0af1510f0f24d40dd00d7c0e51605ca504bbc177c3e19b065f373a1efdd22f");

    // Create transaction with P2WPKH as output.
    let txid: Vec<u8> = vec![1; 32];
    let tx1 = Proto::Input {
        txid: txid.as_slice().into(),
        vout: 0,
        value: ONE_BTC,
        sighash_type: UtxoProto::SighashType::All,
        to_recipient: ProtoInputRecipient::builder(Proto::mod_Input::InputBuilder {
            variant: ProtoInputBuilder::p2pkh(alice_pubkey.as_slice().into()),
        }),
        ..Default::default()
    };

    let out1 = Proto::Output {
        value: 50_000_000, // 0.5 BTC
        to_recipient: ProtoOutputRecipient::builder(Proto::mod_Output::OutputBuilder {
            variant: ProtoOutputBuilder::p2wpkh(Proto::ToPublicKeyOrHash {
                to_address: ProtoPubkeyOrHash::pubkey(bob_pubkey.as_slice().into()),
            }),
        }),
    };

    let signing = Proto::SigningInput {
        private_key: alice_private_key.as_slice().into(),
        inputs: vec![tx1],
        outputs: vec![out1],
        ..Default::default()
    };

    let signed = BitcoinEntry.sign(&coin, signing);

    // By default, we mandate that a change output is set.
    assert_eq!(signed.error, Proto::Error::Error_invalid_change_output);
    assert_eq!(signed.error_message, "Error_invalid_change_output");
    assert_eq!(signed.transaction, None);
    assert!(signed.encoded.is_empty());
    assert!(signed.txid.is_empty());
    assert_eq!(signed.weight, 0);
    assert_eq!(signed.fee, 0);
}

#[test]
fn input_selection_select_in_order() {
    let coin = TestCoinContext::default();

    let alice_private_key = hex("57a64865bce5d4855e99b1cce13327c46171434f2d72eeaf9da53ee075e7f90a");
    let alice_pubkey = hex("028d7dce6d72fb8f7af9566616c6436349c67ad379f2404dd66fe7085fe0fba28f");
    let bob_pubkey = hex("025a0af1510f0f24d40dd00d7c0e51605ca504bbc177c3e19b065f373a1efdd22f");

    // Create transaction with P2WPKH as output.
    let txid: Vec<u8> = vec![1; 32];
    let tx1 = Proto::Input {
        txid: txid.as_slice().into(),
        vout: 0,
        value: ONE_BTC * 3,
        sighash_type: UtxoProto::SighashType::All,
        to_recipient: ProtoInputRecipient::builder(Proto::mod_Input::InputBuilder {
            variant: ProtoInputBuilder::p2wpkh(alice_pubkey.as_slice().into()),
        }),
        ..Default::default()
    };

    let txid: Vec<u8> = vec![2; 32];
    let tx2 = Proto::Input {
        txid: txid.as_slice().into(),
        vout: 0,
        value: ONE_BTC * 2,
        sighash_type: UtxoProto::SighashType::All,
        to_recipient: ProtoInputRecipient::builder(Proto::mod_Input::InputBuilder {
            variant: ProtoInputBuilder::p2wpkh(alice_pubkey.as_slice().into()),
        }),
        ..Default::default()
    };

    let txid: Vec<u8> = vec![3; 32];
    let tx3 = Proto::Input {
        txid: txid.as_slice().into(),
        vout: 0,
        value: ONE_BTC,
        sighash_type: UtxoProto::SighashType::All,
        to_recipient: ProtoInputRecipient::builder(Proto::mod_Input::InputBuilder {
            variant: ProtoInputBuilder::p2wpkh(alice_pubkey.as_slice().into()),
        }),
        ..Default::default()
    };

    let out1 = Proto::Output {
        value: ONE_BTC / 2, // 0.5 BTC
        to_recipient: ProtoOutputRecipient::builder(Proto::mod_Output::OutputBuilder {
            variant: ProtoOutputBuilder::p2wpkh(Proto::ToPublicKeyOrHash {
                to_address: ProtoPubkeyOrHash::pubkey(bob_pubkey.as_slice().into()),
            }),
        }),
    };

    let change_output = Proto::Output {
        // Will be set for us.
        // TODO: Enforce that this is set to zero?
        value: 0,
        to_recipient: ProtoOutputRecipient::builder(Proto::mod_Output::OutputBuilder {
            variant: ProtoOutputBuilder::p2wpkh(Proto::ToPublicKeyOrHash {
                to_address: ProtoPubkeyOrHash::pubkey(alice_pubkey.as_slice().into()),
            }),
        }),
    };

    // TODO:
    // * Return an error if fee_per_vb is zero?
    // * disable_change_output = false on default?

    let signing = Proto::SigningInput {
        private_key: alice_private_key.as_slice().into(),
        input_selector: UtxoProto::InputSelector::SelectInOrder,
        inputs: vec![tx1.clone(), tx2, tx3],
        outputs: vec![out1.clone()],
        // We set the change output accordingly.
        change_output: Some(change_output),
        fee_per_vb: SAT_VBYTE,
        ..Default::default()
    };

    let signed = BitcoinEntry.sign(&coin, signing);

    assert_eq!(signed.error, Proto::Error::OK);
    assert!(signed.error_message.is_empty());
    assert_eq!(signed.weight, 560);
    assert_eq!(signed.fee, (signed.weight + 3) / 4 * SAT_VBYTE);
    assert_eq!(signed.fee, 7_000);

    let tx = signed.transaction.unwrap();
    assert_eq!(tx.version, 2);

    // Inputs, only one input was selected (ONE_BTC * 3).
    assert_eq!(tx.inputs.len(), 1);
    assert_eq!(tx.inputs[0].txid, tx1.txid);
    assert_eq!(tx.inputs[0].txid, vec![1; 32]);
    assert_eq!(tx.inputs[0].vout, 0);
    assert_eq!(tx.inputs[0].sequence, u32::MAX);
    assert!(tx.inputs[0].script_sig.is_empty());
    assert!(!tx.inputs[0].witness_items.is_empty());

    // Outputs.
    assert_eq!(tx.outputs.len(), 2);

    // Output for recipient.
    assert!(!tx.outputs[0].script_pubkey.is_empty());
    assert_eq!(tx.outputs[0].value, out1.value);
    assert_eq!(tx.outputs[0].value, 50_000_000);
    assert!(tx.outputs[0].taproot_payload.is_empty());
    assert!(tx.outputs[0].control_block.is_empty());

    // Change output.
    assert!(!tx.outputs[1].script_pubkey.is_empty());
    assert_eq!(tx.outputs[1].value, tx1.value - out1.value - signed.fee);
    assert_eq!(tx.outputs[1].value, ONE_BTC * 3 - 50_000_000 - 7_000);
    assert!(tx.outputs[1].taproot_payload.is_empty());
    assert!(tx.outputs[1].control_block.is_empty());
}

#[test]
fn input_selection_select_ascending() {
    let coin = TestCoinContext::default();

    let alice_private_key = hex("57a64865bce5d4855e99b1cce13327c46171434f2d72eeaf9da53ee075e7f90a");
    let alice_pubkey = hex("028d7dce6d72fb8f7af9566616c6436349c67ad379f2404dd66fe7085fe0fba28f");
    let bob_pubkey = hex("025a0af1510f0f24d40dd00d7c0e51605ca504bbc177c3e19b065f373a1efdd22f");

    // Create transaction with P2WPKH as output.
    let txid: Vec<u8> = vec![1; 32];
    let tx1 = Proto::Input {
        txid: txid.as_slice().into(),
        vout: 0,
        value: ONE_BTC * 3,
        sighash_type: UtxoProto::SighashType::All,
        to_recipient: ProtoInputRecipient::builder(Proto::mod_Input::InputBuilder {
            variant: ProtoInputBuilder::p2wpkh(alice_pubkey.as_slice().into()),
        }),
        ..Default::default()
    };

    let txid: Vec<u8> = vec![2; 32];
    let tx2 = Proto::Input {
        txid: txid.as_slice().into(),
        vout: 0,
        value: ONE_BTC * 2,
        sighash_type: UtxoProto::SighashType::All,
        to_recipient: ProtoInputRecipient::builder(Proto::mod_Input::InputBuilder {
            variant: ProtoInputBuilder::p2wpkh(alice_pubkey.as_slice().into()),
        }),
        ..Default::default()
    };

    let txid: Vec<u8> = vec![3; 32];
    let tx3 = Proto::Input {
        txid: txid.as_slice().into(),
        vout: 0,
        value: ONE_BTC,
        sighash_type: UtxoProto::SighashType::All,
        to_recipient: ProtoInputRecipient::builder(Proto::mod_Input::InputBuilder {
            variant: ProtoInputBuilder::p2wpkh(alice_pubkey.as_slice().into()),
        }),
        ..Default::default()
    };

    let out1 = Proto::Output {
        value: ONE_BTC + ONE_BTC / 2, // 1.5 BTC
        to_recipient: ProtoOutputRecipient::builder(Proto::mod_Output::OutputBuilder {
            variant: ProtoOutputBuilder::p2wpkh(Proto::ToPublicKeyOrHash {
                to_address: ProtoPubkeyOrHash::pubkey(bob_pubkey.as_slice().into()),
            }),
        }),
    };

    let change_output = Proto::Output {
        // Will be set for us.
        // TODO: Enforce that this is set to zero?
        value: 0,
        to_recipient: ProtoOutputRecipient::builder(Proto::mod_Output::OutputBuilder {
            variant: ProtoOutputBuilder::p2wpkh(Proto::ToPublicKeyOrHash {
                to_address: ProtoPubkeyOrHash::pubkey(alice_pubkey.as_slice().into()),
            }),
        }),
    };

    let signing = Proto::SigningInput {
        private_key: alice_private_key.as_slice().into(),
        //input_selector: UtxoProto::InputSelector::SelectAscending, // default
        inputs: vec![tx1, tx2.clone(), tx3.clone()],
        outputs: vec![out1.clone()],
        // We set the change output accordingly.
        change_output: Some(change_output),
        fee_per_vb: SAT_VBYTE,
        ..Default::default()
    };

    let signed = BitcoinEntry.sign(&coin, signing);

    assert_eq!(signed.error, Proto::Error::OK);
    assert!(signed.error_message.is_empty());
    assert_eq!(signed.weight, 832);
    assert_eq!(signed.fee, (signed.weight + 3) / 4 * SAT_VBYTE);
    assert_eq!(signed.fee, 10_400);

    let tx = signed.transaction.unwrap();
    assert_eq!(tx.version, 2);

    // Inputs.
    assert_eq!(tx.inputs.len(), 2);

    assert_eq!(tx.inputs[0].txid, tx3.txid);
    assert_eq!(tx.inputs[0].txid, vec![3; 32]);
    assert_eq!(tx.inputs[0].vout, 0);
    assert_eq!(tx.inputs[0].sequence, u32::MAX);
    assert!(tx.inputs[0].script_sig.is_empty());
    assert!(!tx.inputs[0].witness_items.is_empty());

    assert_eq!(tx.inputs[1].txid, tx2.txid);
    assert_eq!(tx.inputs[1].txid, vec![2; 32]);
    assert_eq!(tx.inputs[1].vout, 0);
    assert_eq!(tx.inputs[1].sequence, u32::MAX);
    assert!(tx.inputs[1].script_sig.is_empty());
    assert!(!tx.inputs[1].witness_items.is_empty());

    // Outputs.
    assert_eq!(tx.outputs.len(), 2);

    // Output for recipient.
    assert!(!tx.outputs[0].script_pubkey.is_empty());
    assert_eq!(tx.outputs[0].value, out1.value);
    assert_eq!(tx.outputs[0].value, ONE_BTC + ONE_BTC / 2); // 1.5 BTC
    assert!(tx.outputs[0].taproot_payload.is_empty());
    assert!(tx.outputs[0].control_block.is_empty());

    // Change output.
    assert!(!tx.outputs[1].script_pubkey.is_empty());
    assert_eq!(
        tx.outputs[1].value,
        tx3.value + tx2.value - out1.value - signed.fee
    );
    assert_eq!(
        tx.outputs[1].value,
        (ONE_BTC + ONE_BTC * 2) - (ONE_BTC + ONE_BTC / 2) - 10_400
    );
    assert!(tx.outputs[1].taproot_payload.is_empty());
    assert!(tx.outputs[1].control_block.is_empty());
}
