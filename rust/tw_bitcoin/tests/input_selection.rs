mod common;

use common::{hex, MINER_FEE, ONE_BTC};
use tw_bitcoin::aliases::*;
use tw_bitcoin::entry::BitcoinEntry;
use tw_coin_entry::coin_entry::CoinEntry;
use tw_coin_entry::test_utils::test_context::TestCoinContext;
use tw_proto::BitcoinV2::Proto;
use tw_proto::Utxo::Proto as UtxoProto;

#[test]
fn coin_entry_select_inputs() {
    let coin = TestCoinContext::default();

    let alice_private_key = hex("57a64865bce5d4855e99b1cce13327c46171434f2d72eeaf9da53ee075e7f90a");
    let alice_pubkey = hex("028d7dce6d72fb8f7af9566616c6436349c67ad379f2404dd66fe7085fe0fba28f");
    let bob_private_key = hex("05dead4689ec7d55de654771120866be83bf1b8e25c9a1b77fc58a336e1cd1a3");
    let bob_pubkey = hex("025a0af1510f0f24d40dd00d7c0e51605ca504bbc177c3e19b065f373a1efdd22f");

    // Create transaction with P2WPKH as output.
    let txid: Vec<u8> = vec![0; 32];
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

    let tx2 = Proto::Input {
        txid: txid.as_slice().into(),
        vout: 0,
        value: ONE_BTC / 2,
        sighash_type: UtxoProto::SighashType::All,
        to_recipient: ProtoInputRecipient::builder(Proto::mod_Input::InputBuilder {
            variant: ProtoInputBuilder::p2pkh(alice_pubkey.as_slice().into()),
        }),
        ..Default::default()
    };

    let tx3 = Proto::Input {
        txid: txid.as_slice().into(),
        vout: 0,
        value: ONE_BTC / 4,
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
        inputs: vec![tx1, tx2, tx3],
        outputs: vec![out1],
        input_selector: UtxoProto::InputSelector::SelectAscending,
        disable_change_output: true,
        ..Default::default()
    };

    let signed = BitcoinEntry.sign(&coin, signing);
	dbg!(&signed.transaction);
    //let encoded = tw_encoding::hex::encode(signed.encoded, false);

    assert_eq!(signed.error, Proto::Error::OK);
}