mod common;

use common::{hex, ONE_BTC};
use tw_bitcoin::aliases::*;
use tw_bitcoin::entry::BitcoinEntry;
use tw_coin_entry::coin_entry::CoinEntry;
use tw_coin_entry::test_utils::test_context::TestCoinContext;
use tw_proto::BitcoinV2::Proto;
use tw_proto::Utxo::Proto as UtxoProto;

const SAT_VB: u64 = 20;

#[test]
fn p2pkh_fee_estimate() {
    let coin = TestCoinContext::default();

    let alice_private_key = hex("57a64865bce5d4855e99b1cce13327c46171434f2d72eeaf9da53ee075e7f90a");
    let alice_pubkey = hex("028d7dce6d72fb8f7af9566616c6436349c67ad379f2404dd66fe7085fe0fba28f");

    let bob_pubkey = hex("025a0af1510f0f24d40dd00d7c0e51605ca504bbc177c3e19b065f373a1efdd22f");
    let txid: Vec<u8> = hex("181c84965c9ea86a5fac32fdbd5f73a21a7a9e749fb6ab97e273af2329f6b911")
        .into_iter()
        .rev()
        .collect();

    let mut signing = Proto::SigningInput {
        private_key: alice_private_key.as_slice().into(),
        inputs: vec![],
        outputs: vec![],
        input_selector: UtxoProto::InputSelector::UseAll,
        fee_per_vb: SAT_VB,
        disable_change_output: true,
        ..Default::default()
    };

    signing.inputs.push(Proto::Input {
        txid: txid.as_slice().into(),
        vout: 0,
        value: 2 * ONE_BTC,
        sighash_type: UtxoProto::SighashType::All,
        to_recipient: ProtoInputRecipient::builder(Proto::mod_Input::InputBuilder {
            variant: ProtoInputBuilder::p2pkh(alice_pubkey.as_slice().into()),
        }),
        ..Default::default()
    });

    signing.outputs.push(Proto::Output {
        value: ONE_BTC,
        to_recipient: ProtoOutputRecipient::builder(Proto::mod_Output::OutputBuilder {
            variant: ProtoOutputBuilder::p2pkh(Proto::ToPublicKeyOrHash {
                to_address: ProtoPubkeyOrHash::pubkey(bob_pubkey.as_slice().into()),
            }),
        }),
    });

    let prehashes = BitcoinEntry.preimage_hashes(&coin, signing.clone());
    assert_eq!(prehashes.error, Proto::Error::OK);
    assert_eq!(prehashes.weight_estimate, 768);
    assert_eq!(prehashes.fee_estimate, (768 + 3) / 4 * SAT_VB);

    let signed = BitcoinEntry.sign(&coin, signing);
    assert_eq!(signed.error, Proto::Error::OK);
    assert_eq!(signed.weight, 768);
    assert_eq!(signed.fee, (768 + 3) / 4 * SAT_VB);
}

#[test]
fn p2wpkh_fee_estimate() {
    let coin = TestCoinContext::default();

    let alice_private_key = hex("57a64865bce5d4855e99b1cce13327c46171434f2d72eeaf9da53ee075e7f90a");
    let alice_pubkey = hex("028d7dce6d72fb8f7af9566616c6436349c67ad379f2404dd66fe7085fe0fba28f");

    let bob_pubkey = hex("025a0af1510f0f24d40dd00d7c0e51605ca504bbc177c3e19b065f373a1efdd22f");
    let txid: Vec<u8> = hex("181c84965c9ea86a5fac32fdbd5f73a21a7a9e749fb6ab97e273af2329f6b911")
        .into_iter()
        .rev()
        .collect();

    let mut signing = Proto::SigningInput {
        private_key: alice_private_key.as_slice().into(),
        inputs: vec![],
        outputs: vec![],
        input_selector: UtxoProto::InputSelector::UseAll,
        fee_per_vb: SAT_VB,
        disable_change_output: true,
        ..Default::default()
    };

    signing.inputs.push(Proto::Input {
        txid: txid.as_slice().into(),
        vout: 0,
        value: 2 * ONE_BTC,
        sequence: u32::MAX,
        sighash_type: UtxoProto::SighashType::All,
        to_recipient: ProtoInputRecipient::builder(Proto::mod_Input::InputBuilder {
            variant: ProtoInputBuilder::p2wpkh(alice_pubkey.as_slice().into()),
        }),
        ..Default::default()
    });

    signing.outputs.push(Proto::Output {
        value: ONE_BTC,
        to_recipient: ProtoOutputRecipient::builder(Proto::mod_Output::OutputBuilder {
            variant: ProtoOutputBuilder::p2wpkh(Proto::ToPublicKeyOrHash {
                to_address: ProtoPubkeyOrHash::pubkey(bob_pubkey.as_slice().into()),
            }),
        }),
    });

    let prehashes = BitcoinEntry.preimage_hashes(&coin, signing.clone());
    assert_eq!(prehashes.error, Proto::Error::OK);
    // TODO: The estimated weight/fee is slightly off from the finalized
    // weight/fee. This is probably good enough, but we can probably improve
    // this.
    assert_eq!(prehashes.weight_estimate, 436);
    assert_eq!(prehashes.fee_estimate, (436 + 3) / 4 * SAT_VB);

    let signed = BitcoinEntry.sign(&coin, signing);
    assert_eq!(signed.error, Proto::Error::OK);
    assert_eq!(signed.weight, 438);
    assert_eq!(signed.fee, (438 + 3) / 4 * SAT_VB);
}

#[test]
fn p2tr_key_path_fee_estimate() {
    let coin = TestCoinContext::default();

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
        value: 2 * ONE_BTC,
        sighash_type: UtxoProto::SighashType::All,
        to_recipient: ProtoInputRecipient::builder(Proto::mod_Input::InputBuilder {
            variant: ProtoInputBuilder::p2tr_key_path(Proto::mod_Input::InputTaprootKeyPath {
                one_prevout: false,
                public_key: alice_pubkey.as_slice().into(),
            }),
        }),
        ..Default::default()
    };

    let out1 = Proto::Output {
        value: ONE_BTC,
        to_recipient: ProtoOutputRecipient::builder(Proto::mod_Output::OutputBuilder {
            variant: ProtoOutputBuilder::p2tr_key_path(bob_pubkey.as_slice().into()),
        }),
    };

    let signing = Proto::SigningInput {
        private_key: alice_private_key.as_slice().into(),
        inputs: vec![tx1],
        outputs: vec![out1],
        input_selector: UtxoProto::InputSelector::UseAll,
        fee_per_vb: SAT_VB,
        disable_change_output: true,
        ..Default::default()
    };

    let prehashes = BitcoinEntry.preimage_hashes(&coin, signing.clone());
    assert_eq!(prehashes.error, Proto::Error::OK);
    // TODO: The estimated weight/fee is slightly off from the finalized
    // weight/fee. This is probably good enough, but we can probably improve
    // this.
    assert_eq!(prehashes.weight_estimate, 450);
    assert_eq!(prehashes.fee_estimate, (450 + 3) / 4 * SAT_VB);

    let signed = BitcoinEntry.sign(&coin, signing);
    assert_eq!(signed.error, Proto::Error::OK);
    assert_eq!(signed.weight, 445);
    assert_eq!(signed.fee, (445 + 3) / 4 * SAT_VB);
}

#[test]
fn brc20_inscribe_fee_estimate() {
    let coin = TestCoinContext::default();

    let alice_private_key = hex("57a64865bce5d4855e99b1cce13327c46171434f2d72eeaf9da53ee075e7f90a");
    let alice_pubkey = hex("028d7dce6d72fb8f7af9566616c6436349c67ad379f2404dd66fe7085fe0fba28f");

    let txid: Vec<u8> = hex("181c84965c9ea86a5fac32fdbd5f73a21a7a9e749fb6ab97e273af2329f6b911")
        .into_iter()
        .rev()
        .collect();

    let tx1 = Proto::Input {
        txid: txid.as_slice().into(),
        vout: 0,
        value: 2 * ONE_BTC,
        sighash_type: UtxoProto::SighashType::All,
        to_recipient: ProtoInputRecipient::builder(Proto::mod_Input::InputBuilder {
            variant: ProtoInputBuilder::brc20_inscribe(Proto::mod_Input::InputBrc20Inscription {
                one_prevout: false,
                inscribe_to: alice_pubkey.as_slice().into(),
                ticker: "oadf".into(),
                transfer_amount: 20,
            }),
        }),
        ..Default::default()
    };

    let out1 = Proto::Output {
        value: ONE_BTC,
        to_recipient: ProtoOutputRecipient::builder(Proto::mod_Output::OutputBuilder {
            variant: ProtoOutputBuilder::brc20_inscribe(
                Proto::mod_Output::OutputBrc20Inscription {
                    inscribe_to: alice_pubkey.as_slice().into(),
                    ticker: "oadf".into(),
                    transfer_amount: 20,
                },
            ),
        }),
    };

    let signing = Proto::SigningInput {
        private_key: alice_private_key.as_slice().into(),
        inputs: vec![tx1],
        outputs: vec![out1],
        input_selector: UtxoProto::InputSelector::UseAll,
        fee_per_vb: SAT_VB,
        disable_change_output: true,
        ..Default::default()
    };

    let prehashes = BitcoinEntry.preimage_hashes(&coin, signing.clone());
    assert_eq!(prehashes.error, Proto::Error::OK);
    // TODO: The estimated weight/fee is slightly off from the finalized
    // weight/fee. This is probably good enough, but we can probably improve
    // this.
    assert_eq!(prehashes.weight_estimate, 575);
    assert_eq!(prehashes.fee_estimate, (575 + 3) / 4 * SAT_VB);

    let signed = BitcoinEntry.sign(&coin, signing);
    assert_eq!(signed.error, Proto::Error::OK);
    assert_eq!(signed.weight, 571);
    assert_eq!(signed.fee, (571 + 3) / 4 * SAT_VB);
}
