mod common;
use common::{hex, ONE_BTC};

use tw_bitcoin::aliases::*;
use tw_bitcoin::entry::BitcoinEntry;
use tw_coin_entry::coin_entry::CoinEntry;
use tw_coin_entry::test_utils::test_context::TestCoinContext;
use tw_proto::BitcoinV2::Proto;
use tw_proto::Utxo::Proto as UtxoProto;

const SAT_VBYTE: u64 = 50;

const ALICE_PRIVATE_KEY: &str = "57a64865bce5d4855e99b1cce13327c46171434f2d72eeaf9da53ee075e7f90a";
const ALICE_PUBKEY: &str = "028d7dce6d72fb8f7af9566616c6436349c67ad379f2404dd66fe7085fe0fba28f";
const BOB_PUBKEY: &str = "025a0af1510f0f24d40dd00d7c0e51605ca504bbc177c3e19b065f373a1efdd22f";

#[test]
fn input_selection_no_change_output() {
    let coin = TestCoinContext::default();

    let alice_private_key = hex(ALICE_PRIVATE_KEY);
    let alice_pubkey = hex(ALICE_PUBKEY);
    let bob_pubkey = hex(BOB_PUBKEY);

    let txid: Vec<u8> = vec![1; 32];
    let tx1 = Proto::Input {
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
        value: 50_000_000, // 0.5 BTC
        to_recipient: ProtoOutputRecipient::builder(Proto::mod_Output::OutputBuilder {
            variant: ProtoOutputBuilder::p2wpkh(Proto::ToPublicKeyOrHash {
                to_address: ProtoPubkeyOrHash::pubkey(bob_pubkey.as_slice().into()),
            }),
        }),
    };

    // TODO: Mandate that fee_per_byte is non-zero?
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
fn input_selection_no_utxo_inputs() {
    let coin = TestCoinContext::default();

    let alice_private_key = hex(ALICE_PRIVATE_KEY);
    let alice_pubkey = hex(ALICE_PUBKEY);
    let bob_pubkey = hex(BOB_PUBKEY);

    let out1 = Proto::Output {
        value: 50_000_000, // 0.5 BTC
        to_recipient: ProtoOutputRecipient::builder(Proto::mod_Output::OutputBuilder {
            variant: ProtoOutputBuilder::p2wpkh(Proto::ToPublicKeyOrHash {
                to_address: ProtoPubkeyOrHash::pubkey(bob_pubkey.as_slice().into()),
            }),
        }),
    };

    let change_output = Proto::Output {
        // Will be set for us.
        value: 0,
        to_recipient: ProtoOutputRecipient::builder(Proto::mod_Output::OutputBuilder {
            variant: ProtoOutputBuilder::p2wpkh(Proto::ToPublicKeyOrHash {
                to_address: ProtoPubkeyOrHash::pubkey(alice_pubkey.as_slice().into()),
            }),
        }),
    };

    let signing = Proto::SigningInput {
        private_key: alice_private_key.as_slice().into(),
        // No inputs.
        inputs: vec![],
        outputs: vec![out1],
        // We set the change output accordingly.
        change_output: Some(change_output),
        ..Default::default()
    };

    let signed = BitcoinEntry.sign(&coin, signing);

    // Error
    assert_eq!(signed.error, Proto::Error::Error_utxo_insufficient_inputs);
    assert_eq!(signed.error_message, "Error_utxo_insufficient_inputs");
    assert_eq!(signed.transaction, None);
    assert!(signed.encoded.is_empty());
    assert!(signed.txid.is_empty());
    assert_eq!(signed.weight, 0);
    assert_eq!(signed.fee, 0);
}

#[test]
fn input_selection_no_utxo_outputs() {
    let coin = TestCoinContext::default();

    let alice_private_key = hex(ALICE_PRIVATE_KEY);
    let alice_pubkey = hex(ALICE_PUBKEY);

    let txid: Vec<u8> = vec![1; 32];
    let tx1 = Proto::Input {
        txid: txid.as_slice().into(),
        vout: 0,
        value: ONE_BTC,
        sighash_type: UtxoProto::SighashType::All,
        to_recipient: ProtoInputRecipient::builder(Proto::mod_Input::InputBuilder {
            variant: ProtoInputBuilder::p2wpkh(alice_pubkey.as_slice().into()),
        }),
        ..Default::default()
    };

    let signing = Proto::SigningInput {
        private_key: alice_private_key.as_slice().into(),
        // No inputs.
        inputs: vec![tx1],
        outputs: vec![],
        disable_change_output: true,
        ..Default::default()
    };

    let signed = BitcoinEntry.sign(&coin, signing);

    // Error
    assert_eq!(signed.error, Proto::Error::Error_utxo_no_outputs_specified);
    assert_eq!(signed.error_message, "Error_utxo_no_outputs_specified");
    assert_eq!(signed.transaction, None);
    assert!(signed.encoded.is_empty());
    assert!(signed.txid.is_empty());
    assert_eq!(signed.weight, 0);
    assert_eq!(signed.fee, 0);
}

#[test]
fn input_selection_insufficient_inputs() {
    let coin = TestCoinContext::default();

    let alice_private_key = hex(ALICE_PRIVATE_KEY);
    let alice_pubkey = hex(ALICE_PUBKEY);
    let bob_pubkey = hex(BOB_PUBKEY);

    let txid: Vec<u8> = vec![1; 32];
    let tx1 = Proto::Input {
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
        value: ONE_BTC * 2,
        to_recipient: ProtoOutputRecipient::builder(Proto::mod_Output::OutputBuilder {
            variant: ProtoOutputBuilder::p2wpkh(Proto::ToPublicKeyOrHash {
                to_address: ProtoPubkeyOrHash::pubkey(bob_pubkey.as_slice().into()),
            }),
        }),
    };

    let change_output = Proto::Output {
        // Will be set for us.
        value: 0,
        to_recipient: ProtoOutputRecipient::builder(Proto::mod_Output::OutputBuilder {
            variant: ProtoOutputBuilder::p2wpkh(Proto::ToPublicKeyOrHash {
                to_address: ProtoPubkeyOrHash::pubkey(alice_pubkey.as_slice().into()),
            }),
        }),
    };

    let signing = Proto::SigningInput {
        private_key: alice_private_key.as_slice().into(),
        // No inputs.
        inputs: vec![tx1],
        outputs: vec![out1],
        // We set the change output accordingly.
        change_output: Some(change_output),
        ..Default::default()
    };

    let signed = BitcoinEntry.sign(&coin, signing);

    // Input does not cover output.
    assert_eq!(signed.error, Proto::Error::Error_utxo_insufficient_inputs);
    assert_eq!(signed.error_message, "Error_utxo_insufficient_inputs");
    assert_eq!(signed.transaction, None);
    assert!(signed.encoded.is_empty());
    assert!(signed.txid.is_empty());
    assert_eq!(signed.weight, 0);
    assert_eq!(signed.fee, 0);
}

#[test]
fn input_selection_no_utxo_outputs_with_change_output() {
    let coin = TestCoinContext::default();

    let alice_private_key = hex(ALICE_PRIVATE_KEY);
    let alice_pubkey = hex(ALICE_PUBKEY);

    let txid: Vec<u8> = vec![1; 32];
    let tx1 = Proto::Input {
        txid: txid.as_slice().into(),
        vout: 0,
        value: ONE_BTC,
        sighash_type: UtxoProto::SighashType::All,
        to_recipient: ProtoInputRecipient::builder(Proto::mod_Input::InputBuilder {
            variant: ProtoInputBuilder::p2wpkh(alice_pubkey.as_slice().into()),
        }),
        ..Default::default()
    };

    let change_output = Proto::Output {
        // Will be set for us.
        value: 0,
        to_recipient: ProtoOutputRecipient::builder(Proto::mod_Output::OutputBuilder {
            variant: ProtoOutputBuilder::p2wpkh(Proto::ToPublicKeyOrHash {
                to_address: ProtoPubkeyOrHash::pubkey(alice_pubkey.as_slice().into()),
            }),
        }),
    };

    let signing = Proto::SigningInput {
        private_key: alice_private_key.as_slice().into(),
        // No inputs.
        inputs: vec![tx1],
        outputs: vec![],
        // We set the change output accordingly.
        change_output: Some(change_output),
        ..Default::default()
    };

    let signed = BitcoinEntry.sign(&coin, signing);

    // Even though there is a change output, we mandate "normal" outputs.
    assert_eq!(signed.error, Proto::Error::Error_utxo_no_outputs_specified);
    assert_eq!(signed.error_message, "Error_utxo_no_outputs_specified");
    assert_eq!(signed.transaction, None);
    assert!(signed.encoded.is_empty());
    assert!(signed.txid.is_empty());
    assert_eq!(signed.weight, 0);
    assert_eq!(signed.fee, 0);
}

#[test]
fn input_selection_select_in_order() {
    let coin = TestCoinContext::default();

    let alice_private_key = hex(ALICE_PRIVATE_KEY);
    let alice_pubkey = hex(ALICE_PUBKEY);
    let bob_pubkey = hex(BOB_PUBKEY);

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
        value: 0,
        to_recipient: ProtoOutputRecipient::builder(Proto::mod_Output::OutputBuilder {
            variant: ProtoOutputBuilder::p2wpkh(Proto::ToPublicKeyOrHash {
                to_address: ProtoPubkeyOrHash::pubkey(alice_pubkey.as_slice().into()),
            }),
        }),
    };

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

    let alice_private_key = hex(ALICE_PRIVATE_KEY);
    let alice_pubkey = hex(ALICE_PUBKEY);
    let bob_pubkey = hex(BOB_PUBKEY);

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

    // Inputs (ascending; third < second < ...).
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

#[test]
fn input_selection_select_descending() {
    let coin = TestCoinContext::default();

    let alice_private_key = hex(ALICE_PRIVATE_KEY);
    let alice_pubkey = hex(ALICE_PUBKEY);
    let bob_pubkey = hex(BOB_PUBKEY);

    let txid: Vec<u8> = vec![1; 32];
    let tx1 = Proto::Input {
        txid: txid.as_slice().into(),
        vout: 0,
        value: ONE_BTC,
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
        value: ONE_BTC * 3,
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
        value: ONE_BTC * 2,
        sighash_type: UtxoProto::SighashType::All,
        to_recipient: ProtoInputRecipient::builder(Proto::mod_Input::InputBuilder {
            variant: ProtoInputBuilder::p2wpkh(alice_pubkey.as_slice().into()),
        }),
        ..Default::default()
    };

    let out1 = Proto::Output {
        value: ONE_BTC * 4,
        to_recipient: ProtoOutputRecipient::builder(Proto::mod_Output::OutputBuilder {
            variant: ProtoOutputBuilder::p2wpkh(Proto::ToPublicKeyOrHash {
                to_address: ProtoPubkeyOrHash::pubkey(bob_pubkey.as_slice().into()),
            }),
        }),
    };

    let change_output = Proto::Output {
        // Will be set for us.
        value: 0,
        to_recipient: ProtoOutputRecipient::builder(Proto::mod_Output::OutputBuilder {
            variant: ProtoOutputBuilder::p2wpkh(Proto::ToPublicKeyOrHash {
                to_address: ProtoPubkeyOrHash::pubkey(alice_pubkey.as_slice().into()),
            }),
        }),
    };

    let signing = Proto::SigningInput {
        private_key: alice_private_key.as_slice().into(),
        // Select descending.
        input_selector: UtxoProto::InputSelector::SelectDescending,
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

    // Inputs (descending; second > third).
    assert_eq!(tx.inputs.len(), 2);

    assert_eq!(tx.inputs[0].txid, tx2.txid);
    assert_eq!(tx.inputs[0].txid, vec![2; 32]);
    assert_eq!(tx.inputs[0].vout, 0);
    assert_eq!(tx.inputs[0].sequence, u32::MAX);
    assert!(tx.inputs[0].script_sig.is_empty());
    assert!(!tx.inputs[0].witness_items.is_empty());

    assert_eq!(tx.inputs[1].txid, tx3.txid);
    assert_eq!(tx.inputs[1].txid, vec![3; 32]);
    assert_eq!(tx.inputs[1].vout, 0);
    assert_eq!(tx.inputs[1].sequence, u32::MAX);
    assert!(tx.inputs[1].script_sig.is_empty());
    assert!(!tx.inputs[1].witness_items.is_empty());

    // Outputs.
    assert_eq!(tx.outputs.len(), 2);

    // Output for recipient.
    assert!(!tx.outputs[0].script_pubkey.is_empty());
    assert_eq!(tx.outputs[0].value, out1.value);
    assert_eq!(tx.outputs[0].value, ONE_BTC * 4);
    assert!(tx.outputs[0].taproot_payload.is_empty());
    assert!(tx.outputs[0].control_block.is_empty());

    // Change output.
    assert!(!tx.outputs[1].script_pubkey.is_empty());
    assert_eq!(
        tx.outputs[1].value,
        tx2.value + tx3.value - out1.value - signed.fee
    );
    assert_eq!(
        tx.outputs[1].value,
        (ONE_BTC * 3) + (ONE_BTC * 2) - (ONE_BTC * 4) - 10_400,
    );
    assert!(tx.outputs[1].taproot_payload.is_empty());
    assert!(tx.outputs[1].control_block.is_empty());
}

#[test]
fn input_selection_use_all() {
    let coin = TestCoinContext::default();

    let alice_private_key = hex(ALICE_PRIVATE_KEY);
    let alice_pubkey = hex(ALICE_PUBKEY);
    let bob_pubkey = hex(BOB_PUBKEY);

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
        value: ONE_BTC,
        to_recipient: ProtoOutputRecipient::builder(Proto::mod_Output::OutputBuilder {
            variant: ProtoOutputBuilder::p2wpkh(Proto::ToPublicKeyOrHash {
                to_address: ProtoPubkeyOrHash::pubkey(bob_pubkey.as_slice().into()),
            }),
        }),
    };

    let change_output = Proto::Output {
        // Will be set for us.
        value: 0,
        to_recipient: ProtoOutputRecipient::builder(Proto::mod_Output::OutputBuilder {
            variant: ProtoOutputBuilder::p2wpkh(Proto::ToPublicKeyOrHash {
                to_address: ProtoPubkeyOrHash::pubkey(alice_pubkey.as_slice().into()),
            }),
        }),
    };

    let signing = Proto::SigningInput {
        private_key: alice_private_key.as_slice().into(),
        input_selector: UtxoProto::InputSelector::UseAll,
        inputs: vec![tx1.clone(), tx2.clone(), tx3.clone()],
        outputs: vec![out1.clone()],
        // We set the change output accordingly.
        change_output: Some(change_output),
        fee_per_vb: SAT_VBYTE,
        ..Default::default()
    };

    let signed = BitcoinEntry.sign(&coin, signing);

    assert_eq!(signed.error, Proto::Error::OK);
    assert!(signed.error_message.is_empty());
    assert_eq!(signed.weight, 1_104);
    assert_eq!(signed.fee, (signed.weight + 3) / 4 * SAT_VBYTE);
    assert_eq!(signed.fee, 13_800);

    let tx = signed.transaction.unwrap();
    assert_eq!(tx.version, 2);

    // All inputs used
    assert_eq!(tx.inputs.len(), 3);

    assert_eq!(tx.inputs[0].txid, tx1.txid);
    assert_eq!(tx.inputs[0].txid, vec![1; 32]);
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

    assert_eq!(tx.inputs[2].txid, tx3.txid);
    assert_eq!(tx.inputs[2].txid, vec![3; 32]);
    assert_eq!(tx.inputs[2].vout, 0);
    assert_eq!(tx.inputs[2].sequence, u32::MAX);
    assert!(tx.inputs[2].script_sig.is_empty());
    assert!(!tx.inputs[2].witness_items.is_empty());

    // Outputs.
    assert_eq!(tx.outputs.len(), 2);

    // Output for recipient.
    assert!(!tx.outputs[0].script_pubkey.is_empty());
    assert_eq!(tx.outputs[0].value, out1.value);
    assert_eq!(tx.outputs[0].value, ONE_BTC);
    assert!(tx.outputs[0].taproot_payload.is_empty());
    assert!(tx.outputs[0].control_block.is_empty());

    // Change output.
    assert!(!tx.outputs[1].script_pubkey.is_empty());
    assert_eq!(
        tx.outputs[1].value,
        tx1.value + tx2.value + tx3.value - out1.value - signed.fee
    );
    assert_eq!(
        tx.outputs[1].value,
        ONE_BTC * 3 + ONE_BTC * 2 + ONE_BTC - ONE_BTC - 13_800
    );
    assert!(tx.outputs[1].taproot_payload.is_empty());
    assert!(tx.outputs[1].control_block.is_empty());
}

#[test]
fn input_selection_use_all_without_change_output() {
    let coin = TestCoinContext::default();

    let alice_private_key = hex(ALICE_PRIVATE_KEY);
    let alice_pubkey = hex(ALICE_PUBKEY);
    let bob_pubkey = hex(BOB_PUBKEY);

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
        value: ONE_BTC,
        to_recipient: ProtoOutputRecipient::builder(Proto::mod_Output::OutputBuilder {
            variant: ProtoOutputBuilder::p2wpkh(Proto::ToPublicKeyOrHash {
                to_address: ProtoPubkeyOrHash::pubkey(bob_pubkey.as_slice().into()),
            }),
        }),
    };

    let signing = Proto::SigningInput {
        private_key: alice_private_key.as_slice().into(),
        input_selector: UtxoProto::InputSelector::UseAll,
        inputs: vec![tx1.clone(), tx2.clone(), tx3.clone()],
        outputs: vec![out1.clone()],
        // Disabled
        disable_change_output: true,
        fee_per_vb: SAT_VBYTE,
        ..Default::default()
    };

    let signed = BitcoinEntry.sign(&coin, signing);

    assert_eq!(signed.error, Proto::Error::OK);
    assert!(signed.error_message.is_empty());
    assert_eq!(signed.weight, 980);
    // All the remainder goes to fees.
    assert_eq!(signed.fee, tx1.value + tx2.value + tx3.value - out1.value);
    assert_eq!(signed.fee, ONE_BTC * 5);

    let tx = signed.transaction.unwrap();
    assert_eq!(tx.version, 2);

    // All inputs used
    assert_eq!(tx.inputs.len(), 3);

    assert_eq!(tx.inputs[0].txid, tx1.txid);
    assert_eq!(tx.inputs[0].txid, vec![1; 32]);
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

    assert_eq!(tx.inputs[2].txid, tx3.txid);
    assert_eq!(tx.inputs[2].txid, vec![3; 32]);
    assert_eq!(tx.inputs[2].vout, 0);
    assert_eq!(tx.inputs[2].sequence, u32::MAX);
    assert!(tx.inputs[2].script_sig.is_empty());
    assert!(!tx.inputs[2].witness_items.is_empty());

    // Only output, the rest goes to fees.
    assert_eq!(tx.outputs.len(), 1);

    // Output for recipient.
    assert!(!tx.outputs[0].script_pubkey.is_empty());
    assert_eq!(tx.outputs[0].value, out1.value);
    assert_eq!(tx.outputs[0].value, ONE_BTC);
    assert!(tx.outputs[0].taproot_payload.is_empty());
    assert!(tx.outputs[0].control_block.is_empty());
}
