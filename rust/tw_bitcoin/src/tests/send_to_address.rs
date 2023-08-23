use super::hex;
use crate::entry::{
    BitcoinEntry, PlaceHolder, ProtoInputBuilder, ProtoInputRecipient, ProtoOutputBuilder,
    ProtoOutputRecipient,
};
use bitcoin::{Address, PublicKey, ScriptBuf};
use tw_coin_entry::coin_entry::CoinEntry;
use tw_proto::BitcoinV2::Proto;
use tw_proto::Utxo::Proto as UtxoProto;

#[test]
fn send_to_p2pkh_address() {
    let coin = PlaceHolder;

    let alice_private_key = hex("57a64865bce5d4855e99b1cce13327c46171434f2d72eeaf9da53ee075e7f90a");
    let alice_pubkey = hex("028d7dce6d72fb8f7af9566616c6436349c67ad379f2404dd66fe7085fe0fba28f");
    let bob_pubkey = hex("025a0af1510f0f24d40dd00d7c0e51605ca504bbc177c3e19b065f373a1efdd22f");

    let txid: Vec<u8> = hex("181c84965c9ea86a5fac32fdbd5f73a21a7a9e749fb6ab97e273af2329f6b911")
        .into_iter()
        .rev()
        .collect();

    let tx1 = Proto::Input {
        txid: txid.as_slice().into(),
        vout: 0,
        amount: 10_000,
        sequence: u32::MAX,
        sequence_enable_zero: false,
        sighash_type: UtxoProto::SighashType::All,
        to_recipient: ProtoInputRecipient::builder(Proto::mod_Input::Builder {
            variant: ProtoInputBuilder::p2pkh(alice_pubkey.as_slice().into()),
        }),
    };

    // Create the P2PKH address.
    let recipient = PublicKey::from_slice(&bob_pubkey).unwrap();
    let address = Address::p2pkh(&recipient, bitcoin::Network::Bitcoin);
    let address_string = address.to_string();

    let out1 = Proto::Output {
        amount: 1_000,
        to_recipient: ProtoOutputRecipient::from_address(address_string.as_bytes().into()),
    };

    let signing = Proto::SigningInput {
        version: 2,
        private_key: alice_private_key.as_slice().into(),
        lock_time: Default::default(),
        inputs: vec![tx1],
        outputs: vec![out1],
        input_selector: UtxoProto::InputSelector::UseAll,
        sat_vb: 0,
        change_output: Default::default(),
        disable_change_output: true,
    };

    let output = BitcoinEntry.sign(&coin, signing);
    assert_eq!(output.error, Proto::Error::OK);

    let tx = output.transaction.as_ref().unwrap();

    assert_eq!(tx.inputs.len(), 1);
    assert_eq!(tx.outputs.len(), 1);

    // The expected P2PKH scriptPubkey
    let expected = ScriptBuf::new_p2pkh(&recipient.pubkey_hash());

    assert_eq!(tx.outputs[0].amount, 1_000);
    assert_eq!(tx.outputs[0].script_pubkey, expected.as_bytes());
    assert!(tx.outputs[0].taproot_payload.is_empty());
    assert!(tx.outputs[0].control_block.is_empty());
}

#[test]
fn send_to_p2wpkh_address() {
    let coin = PlaceHolder;

    let alice_private_key = hex("57a64865bce5d4855e99b1cce13327c46171434f2d72eeaf9da53ee075e7f90a");
    let alice_pubkey = hex("028d7dce6d72fb8f7af9566616c6436349c67ad379f2404dd66fe7085fe0fba28f");
    let bob_pubkey = hex("025a0af1510f0f24d40dd00d7c0e51605ca504bbc177c3e19b065f373a1efdd22f");

    let txid: Vec<u8> = hex("181c84965c9ea86a5fac32fdbd5f73a21a7a9e749fb6ab97e273af2329f6b911")
        .into_iter()
        .rev()
        .collect();

    let tx1 = Proto::Input {
        txid: txid.as_slice().into(),
        vout: 0,
        amount: 10_000,
        sequence: u32::MAX,
        sequence_enable_zero: false,
        sighash_type: UtxoProto::SighashType::All,
        to_recipient: ProtoInputRecipient::builder(Proto::mod_Input::Builder {
            variant: ProtoInputBuilder::p2pkh(alice_pubkey.as_slice().into()),
        }),
    };

    // Create the P2WPKH address.
    let recipient = PublicKey::from_slice(&bob_pubkey).unwrap();
    let address = Address::p2wpkh(&recipient, bitcoin::Network::Bitcoin).unwrap();
    let address_string = address.to_string();

    let out1 = Proto::Output {
        amount: 1_000,
        to_recipient: ProtoOutputRecipient::from_address(address_string.as_bytes().into()),
    };

    let signing = Proto::SigningInput {
        version: 2,
        private_key: alice_private_key.as_slice().into(),
        lock_time: Default::default(),
        inputs: vec![tx1],
        outputs: vec![out1],
        input_selector: UtxoProto::InputSelector::UseAll,
        sat_vb: 0,
        change_output: Default::default(),
        disable_change_output: true,
    };

    let output = BitcoinEntry.sign(&coin, signing);
    assert_eq!(output.error, Proto::Error::OK);

    let tx = output.transaction.as_ref().unwrap();

    assert_eq!(tx.inputs.len(), 1);
    assert_eq!(tx.outputs.len(), 1);

    // The expected P2WPKH scriptPubkey
    let expected = ScriptBuf::new_v0_p2wpkh(&recipient.wpubkey_hash().unwrap());

    assert_eq!(tx.outputs[0].amount, 1_000);
    assert_eq!(tx.outputs[0].script_pubkey, expected.as_bytes());
    assert!(tx.outputs[0].taproot_payload.is_empty());
    assert!(tx.outputs[0].control_block.is_empty());
}
