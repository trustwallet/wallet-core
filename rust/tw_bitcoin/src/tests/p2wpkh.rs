use super::hex;
use crate::entry::{
    BitcoinEntry, PlaceHolder, ProtoInputBuilder, ProtoInputRecipient, ProtoOutputBuilder,
    ProtoOutputRecipient,
};
use tw_coin_entry::coin_entry::CoinEntry;
use tw_proto::BitcoinV2::Proto;
use tw_proto::Utxo::Proto as UtxoProto;

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

    let tx1 = Proto::Input {
        txid: txid.as_slice().into(),
        vout: 0,
        amount: 100_000_000 * 50,
        sequence: u32::MAX,
        sequence_enable_zero: false,
        sighash_type: UtxoProto::SighashType::All,
        to_recipient: ProtoInputRecipient::builder(Proto::mod_Input::Builder {
            variant: ProtoInputBuilder::p2pkh(alice_pubkey.as_slice().into()),
        }),
    };

    let out1 = Proto::Output {
        amount: 100_000_000 * 50 - 1_000_000,
        to_recipient: ProtoOutputRecipient::builder(Proto::mod_Output::Builder {
            variant: ProtoOutputBuilder::p2wpkh(Proto::ToPublicKeyOrHash {
                to_address: Proto::mod_ToPublicKeyOrHash::OneOfto_address::pubkey(
                    bob_pubkey.as_slice().into(),
                ),
            }),
        }),
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
    let encoded = tw_encoding::hex::encode(output.encoded, false);

    assert_eq!(&encoded, "020000000111b9f62923af73e297abb69f749e7a1aa2735fbdfd32ac5f6aa89e5c96841c18000000006b483045022100df9ed0b662b759e68b89a42e7144cddf787782a7129d4df05642dd825930e6e6022051a08f577f11cc7390684bbad2951a6374072253ffcf2468d14035ed0d8cd6490121028d7dce6d72fb8f7af9566616c6436349c67ad379f2404dd66fe7085fe0fba28fffffffff01c0aff629010000001600140d0e1cec6c2babe8badde5e9b3dea667da90036d00000000");
}
