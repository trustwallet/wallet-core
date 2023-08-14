use tw_coin_entry::coin_entry::CoinEntry;
use tw_proto::BitcoinV2::Proto;
use tw_proto::Utxo::Proto as UtxoProto;

use crate::entry::{BitcoinEntry, PlaceHolder};

#[test]
fn coin_entry_sign_input_p2pkh_and_p2wpkh_output_p2wpkh() {
    let coin = PlaceHolder;
    let alice_private_key = tw_encoding::hex::decode(
        "57a64865bce5d4855e99b1cce13327c46171434f2d72eeaf9da53ee075e7f90a",
    )
    .unwrap();

    let alice_pubkey = tw_encoding::hex::decode(
        "028d7dce6d72fb8f7af9566616c6436349c67ad379f2404dd66fe7085fe0fba28f",
    )
    .unwrap();

    let bob_pubkey = tw_encoding::hex::decode(
        "025a0af1510f0f24d40dd00d7c0e51605ca504bbc177c3e19b065f373a1efdd22f",
    )
    .unwrap();

    let txid: Vec<u8> = tw_encoding::hex::decode(
        "181c84965c9ea86a5fac32fdbd5f73a21a7a9e749fb6ab97e273af2329f6b911",
    )
    .unwrap()
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
        sighash: 0,
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
