use tw_coin_entry::coin_entry::CoinEntry;
use tw_proto::BitcoinV2::Proto;
use tw_proto::Utxo::Proto as UtxoProto;

use crate::entry::{BitcoinEntry, PlaceHolder};

fn hex(string: &str) -> Vec<u8> {
    tw_encoding::hex::decode(string).unwrap()
}

#[test]
fn coin_entry_sign_input_p2pkh_and_p2wpkh_output_p2wpkh() {
    let coin = PlaceHolder;

    let alice_private_key = hex("57a64865bce5d4855e99b1cce13327c46171434f2d72eeaf9da53ee075e7f90a");
    let alice_pubkey = hex("028d7dce6d72fb8f7af9566616c6436349c67ad379f2404dd66fe7085fe0fba28f");
    let bob_pubkey = hex("025a0af1510f0f24d40dd00d7c0e51605ca504bbc177c3e19b065f373a1efdd22f");
    let txid: Vec<u8> = hex("181c84965c9ea86a5fac32fdbd5f73a21a7a9e749fb6ab97e273af2329f6b911")
        .into_iter()
        .rev()
        .collect();

    let mut signing = Proto::SigningInput {
        version: 2,
        private_key: alice_private_key.as_slice().into(),
        lock_time: Proto::mod_SigningInput::OneOflock_time::blocks(0),
        input_selector: Proto::SelectorType::UseAll,
        inputs: vec![],
        outputs: vec![],
        sat_vb: 0,
        create_change: false,
    };

    signing.inputs.push(Proto::Input {
        txid: txid.as_slice().into(),
        vout: 0,
        amount: 100_000_000 * 50,
        sighash_type: UtxoProto::SighashType::All,
        one_prevout: false,
        variant: Proto::mod_Input::OneOfvariant::builder(Proto::mod_Input::InputVariant {
            variant: Proto::mod_Input::mod_InputVariant::OneOfvariant::p2pkh(
                alice_pubkey.as_slice().into(),
            ),
        }),
    });

    signing.outputs.push(Proto::Output {
        amount: 100_000_000 * 50 - 1_000_000,
        to_recipient: Proto::mod_Output::OneOfto_recipient::builder(Proto::Builder {
            type_pb: Proto::mod_Builder::OneOftype_pb::p2wpkh(Proto::ToPublicKeyOrHash {
                to_address: Proto::mod_ToPublicKeyOrHash::OneOfto_address::pubkey(
                    bob_pubkey.as_slice().into(),
                ),
            }),
        }),
    });

    let output = BitcoinEntry.sign(&coin, signing);
    let encoded = tw_encoding::hex::encode(output.encoded, false);

    assert_eq!(&encoded, "020000000111b9f62923af73e297abb69f749e7a1aa2735fbdfd32ac5f6aa89e5c96841c18000000006b483045022100df9ed0b662b759e68b89a42e7144cddf787782a7129d4df05642dd825930e6e6022051a08f577f11cc7390684bbad2951a6374072253ffcf2468d14035ed0d8cd6490121028d7dce6d72fb8f7af9566616c6436349c67ad379f2404dd66fe7085fe0fba28fffffffff01c0aff629010000001600140d0e1cec6c2babe8badde5e9b3dea667da90036d00000000");
}

#[test]
fn coin_entry_sign_input_p2pkh_output_p2tr_key_path() {
    let coin = PlaceHolder;

    let alice_private_key = hex("12ce558df23528f1aa86f1f51ac7e13a197a06bda27610fa89e13b04c40ee999");
    let alice_pubkey = hex("0351e003fdc48e7f31c9bc94996c91f6c3273b7ef4208a1686021bedf7673bb058");
    let bob_private_key = hex("26c2566adcc030a1799213bfd546e615f6ab06f72085ec6806ff1761da48d227");
    let bob_pubkey = hex("02c0938cf377023dfde55e9c96b3cff4ca8894fb6b5d2009006bd43c0bff69cac9");
    let txid: Vec<u8> = hex("c50563913e5a838f937c94232f5a8fc74e58b629fae41dfdffcc9a70f833b53a")
        .into_iter()
        .rev()
        .collect();
    let txid2: Vec<u8> = hex("9a582032f6a50cedaff77d3d5604b33adf8bc31bdaef8de977c2187e395860ac")
        .into_iter()
        .rev()
        .collect();

    let mut signing = Proto::SigningInput {
        version: 2,
        private_key: alice_private_key.as_slice().into(),
        lock_time: Proto::mod_SigningInput::OneOflock_time::blocks(0),
        input_selector: Proto::SelectorType::UseAll,
        inputs: vec![],
        outputs: vec![],
        sat_vb: 0,
        create_change: false,
    };

    signing.inputs.push(Proto::Input {
        txid: txid.as_slice().into(),
        vout: 0,
        amount: 100_000_000 * 50,
        sighash_type: UtxoProto::SighashType::All,
        one_prevout: false,
        variant: Proto::mod_Input::OneOfvariant::builder(Proto::mod_Input::InputVariant {
            variant: Proto::mod_Input::mod_InputVariant::OneOfvariant::p2pkh(
                alice_pubkey.as_slice().into(),
            ),
        }),
    });

    signing.outputs.push(Proto::Output {
        amount: 100_000_000 * 50 - 1_000_000,
        to_recipient: Proto::mod_Output::OneOfto_recipient::builder(Proto::Builder {
            type_pb: Proto::mod_Builder::OneOftype_pb::p2tr_key_path(bob_pubkey.as_slice().into()),
        }),
    });

    let output = BitcoinEntry.sign(&coin, signing);
    let encoded = tw_encoding::hex::encode(output.encoded, false);

    assert_eq!(&encoded, "02000000013ab533f8709accfffd1de4fa29b6584ec78f5a2f23947c938f835a3e916305c5000000006b48304502210086ab2c2192e2738529d6cd9604d8ee75c5b09b0c2f4066a5c5fa3f87a26c0af602202afc7096aaa992235c43e712146057b5ed6a776d82b9129620bc5a21991c0a5301210351e003fdc48e7f31c9bc94996c91f6c3273b7ef4208a1686021bedf7673bb058ffffffff01c0aff62901000000225120e01cfdd05da8fa1d71f987373f3790d45dea9861acb0525c86656fe50f4397a600000000");

    let mut signing = Proto::SigningInput {
        version: 2,
        private_key: bob_private_key.as_slice().into(),
        lock_time: Proto::mod_SigningInput::OneOflock_time::blocks(0),
        input_selector: Proto::SelectorType::UseAll,
        inputs: vec![],
        outputs: vec![],
        sat_vb: 0,
        create_change: false,
    };

    signing.inputs.push(Proto::Input {
        txid: txid2.as_slice().into(),
        vout: 0,
        amount: 100_000_000 * 50 - 1_000_000,
        sighash_type: UtxoProto::SighashType::UseDefault,
        one_prevout: false,
        variant: Proto::mod_Input::OneOfvariant::builder(Proto::mod_Input::InputVariant {
            variant: Proto::mod_Input::mod_InputVariant::OneOfvariant::p2tr_key_path(
                bob_pubkey.as_slice().into(),
            ),
        }),
    });

    signing.outputs.push(Proto::Output {
        amount: 100_000_000 * 50 - 1_000_000 - 1_000_000,
        to_recipient: Proto::mod_Output::OneOfto_recipient::builder(Proto::Builder {
            type_pb: Proto::mod_Builder::OneOftype_pb::p2tr_key_path(
                alice_pubkey.as_slice().into(),
            ),
        }),
    });

    let output = BitcoinEntry.sign(&coin, signing);
    let encoded = tw_encoding::hex::encode(output.encoded, false);

    assert_eq!(&encoded, "02000000000101ac6058397e18c277e98defda1bc38bdf3ab304563d7df7afed0ca5f63220589a0000000000ffffffff01806de72901000000225120a5c027857e359d19f625e52a106b8ac6ca2d6a8728f6cf2107cd7958ee0787c20140ec2d3910d41506b60aaa20520bb72f15e2d2cbd97e3a8e26ee7bad5f4c56b0f2fb0ceaddac33cb2813a33ba017ba6b1d011bab74a0426f12a2bcf47b4ed5bc8600000000");
}
