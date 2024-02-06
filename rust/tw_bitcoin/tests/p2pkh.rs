mod common;

use common::{hex, MINER_FEE, ONE_BTC};
use tw_bitcoin::aliases::*;
use tw_bitcoin::entry::BitcoinEntry;
use tw_coin_entry::coin_entry::CoinEntry;
use tw_coin_entry::test_utils::test_context::TestCoinContext;
use tw_proto::BitcoinV2::Proto;
use tw_proto::Utxo::Proto as UtxoProto;

#[test]
fn coin_entry_sign_input_p2pkh_output_p2pkh() {
    let coin = TestCoinContext::default();

    let alice_private_key = hex("56429688a1a6b00b90ccd22a0de0a376b6569d8684022ae92229a28478bfb657");
    let alice_pubkey = hex("036666dd712e05a487916384bfcd5973eb53e8038eccbbf97f7eed775b87389536");
    let _bob_private_key = hex("b7da1ec42b19085fe09fec54b9d9eacd998ae4e6d2ad472be38d8393391b9ead");
    let bob_pubkey = hex("037ed9a436e11ec4947ac4b7823787e24ba73180f1edd2857bff19c9f4d62b65bf");

    // Create transaction with P2PKH as input and output.
    let txid: Vec<u8> = hex("1e1cdc48aa990d7e154a161d5b5f1cad737742e97d2712ab188027bb42e6e47b")
        .into_iter()
        .rev()
        .collect();

    let tx1 = Proto::Input {
        txid: txid.as_slice().into(),
        vout: 0,
        value: ONE_BTC * 50,
        sighash_type: UtxoProto::SighashType::All,
        to_recipient: ProtoInputRecipient::builder(Proto::mod_Input::InputBuilder {
            variant: ProtoInputBuilder::p2pkh(alice_pubkey.as_slice().into()),
        }),
        ..Default::default()
    };

    let out1 = Proto::Output {
        value: ONE_BTC * 50 - MINER_FEE,
        to_recipient: ProtoOutputRecipient::builder(Proto::mod_Output::OutputBuilder {
            variant: ProtoOutputBuilder::p2pkh(Proto::ToPublicKeyOrHash {
                to_address: ProtoPubkeyOrHash::pubkey(bob_pubkey.as_slice().into()),
            }),
        }),
    };

    let signing = Proto::SigningInput {
        private_key: alice_private_key.as_slice().into(),
        inputs: vec![tx1],
        outputs: vec![out1],
        input_selector: UtxoProto::InputSelector::UseAll,
        disable_change_output: true,
        ..Default::default()
    };

    let signed = BitcoinEntry.sign(&coin, signing);
    let encoded = tw_encoding::hex::encode(signed.encoded, false);

    assert_eq!(signed.error, Proto::Error::OK);
    assert_eq!(&encoded, "02000000017be4e642bb278018ab12277de9427773ad1c5f5b1d164a157e0d99aa48dc1c1e000000006a473044022078eda020d4b86fcb3af78ef919912e6d79b81164dbbb0b0b96da6ac58a2de4b102201a5fd8d48734d5a02371c4b5ee551a69dca3842edbf577d863cf8ae9fdbbd4590121036666dd712e05a487916384bfcd5973eb53e8038eccbbf97f7eed775b87389536ffffffff01c0aff629010000001976a9145eaaa4f458f9158f86afcba08dd7448d27045e3d88ac00000000");
}
