mod common;

use common::hex;
use tw_bitcoin::aliases::*;
use tw_bitcoin::BitcoinEntry;
use tw_coin_entry::coin_entry::CoinEntry;
use tw_coin_entry::test_utils::test_context::TestCoinContext;
use tw_proto::BitcoinV2::Proto;
use tw_proto::Utxo::Proto as UtxoProto;

#[test]
fn coin_entry_sign_brc20_commit_reveal_transfer() {
    let coin = TestCoinContext::default();

    let alice_private_key = hex("e253373989199da27c48680e3a3fc0f648d50f9a727ef17a7fe6a4dc3b159129");
    let alice_pubkey = hex("030f209b6ada5edb42c77fd2bc64ad650ae38314c8f451f3e36d80bc8e26f132cb");

    let txid: Vec<u8> = hex("8ec895b4d30adb01e38471ca1019bfc8c3e5fbd1f28d9e7b5653260d89989008")
        .into_iter()
        .rev()
        .collect();

    let tx1 = Proto::Input {
        txid: txid.as_slice().into(),
        vout: 1,
        value: 26_400,
        sighash_type: UtxoProto::SighashType::All,
        to_recipient: ProtoInputRecipient::builder(Proto::mod_Input::InputBuilder {
            variant: ProtoInputBuilder::p2wpkh(alice_pubkey.as_slice().into()),
        }),
        ..Default::default()
    };

    let out1 = Proto::Output {
        value: 7_000,
        to_recipient: ProtoOutputRecipient::builder(Proto::mod_Output::OutputBuilder {
            variant: ProtoOutputBuilder::brc20_inscribe(
                Proto::mod_Output::OutputBrc20Inscription {
                    inscribe_to: alice_pubkey.as_slice().into(),
                    ticker: "oadf".into(),
                    transfer_amount: "20".into(),
                },
            ),
        }),
    };

    // Change/return transaction.
    let out2 = Proto::Output {
        value: 16_400,
        to_recipient: ProtoOutputRecipient::builder(Proto::mod_Output::OutputBuilder {
            variant: ProtoOutputBuilder::p2wpkh(Proto::ToPublicKeyOrHash {
                to_address: ProtoPubkeyOrHash::pubkey(alice_pubkey.as_slice().into()),
            }),
        }),
    };

    let signing = Proto::SigningInput {
        private_key: alice_private_key.as_slice().into(),
        inputs: vec![tx1],
        outputs: vec![out1, out2],
        input_selector: UtxoProto::InputSelector::UseAll,
        disable_change_output: true,
        ..Default::default()
    };

    let signed = BitcoinEntry.sign(&coin, signing);
    assert_eq!(signed.error, Proto::Error::OK);
    assert_eq!(
        signed.txid,
        hex("797d17d47ae66e598341f9dfdea020b04d4017dcf9cc33f0e51f7a6082171fb1")
    );

    let encoded = tw_encoding::hex::encode(signed.encoded, false);
    let transaction = signed.transaction.unwrap();

    assert_eq!(transaction.inputs.len(), 1);
    assert_eq!(transaction.outputs.len(), 2);
    assert_eq!(&encoded, "02000000000101089098890d2653567b9e8df2d1fbe5c3c8bf1910ca7184e301db0ad3b495c88e0100000000ffffffff02581b000000000000225120e8b706a97732e705e22ae7710703e7f589ed13c636324461afa443016134cc051040000000000000160014e311b8d6ddff856ce8e9a4e03bc6d4fe5050a83d02483045022100a44aa28446a9a886b378a4a65e32ad9a3108870bd725dc6105160bed4f317097022069e9de36422e4ce2e42b39884aa5f626f8f94194d1013007d5a1ea9220a06dce0121030f209b6ada5edb42c77fd2bc64ad650ae38314c8f451f3e36d80bc8e26f132cb00000000");

    // https://www.blockchain.com/explorer/transactions/btc/797d17d47ae66e598341f9dfdea020b04d4017dcf9cc33f0e51f7a6082171fb1
    let txid: Vec<u8> = hex("797d17d47ae66e598341f9dfdea020b04d4017dcf9cc33f0e51f7a6082171fb1")
        .into_iter()
        .rev()
        .collect();

    let tx1 = Proto::Input {
        txid: txid.as_slice().into(),
        vout: 0,
        value: 7_000,
        sighash_type: UtxoProto::SighashType::UseDefault,
        to_recipient: ProtoInputRecipient::builder(Proto::mod_Input::InputBuilder {
            variant: ProtoInputBuilder::brc20_inscribe(Proto::mod_Input::InputBrc20Inscription {
                one_prevout: false,
                inscribe_to: alice_pubkey.as_slice().into(),
                ticker: "oadf".into(),
                transfer_amount: "20".into(),
            }),
        }),
        ..Default::default()
    };

    let out1 = Proto::Output {
        value: 546,
        to_recipient: ProtoOutputRecipient::builder(Proto::mod_Output::OutputBuilder {
            variant: ProtoOutputBuilder::p2wpkh(Proto::ToPublicKeyOrHash {
                to_address: ProtoPubkeyOrHash::pubkey(alice_pubkey.as_slice().into()),
            }),
        }),
    };

    let signing = Proto::SigningInput {
        private_key: alice_private_key.as_slice().into(),
        inputs: vec![tx1],
        outputs: vec![out1],
        input_selector: UtxoProto::InputSelector::UseAll,
        disable_change_output: true,
        // We enable deterministic Schnorr signatures here
        dangerous_use_fixed_schnorr_rng: true,
        ..Default::default()
    };

    let signed = BitcoinEntry.sign(&coin, signing);
    assert_eq!(signed.error, Proto::Error::OK);

    // https://www.blockchain.com/explorer/transactions/btc/7046dc2689a27e143ea2ad1039710885147e9485ab6453fa7e87464aa7dd3eca
    assert_eq!(
        signed.txid,
        hex("7046dc2689a27e143ea2ad1039710885147e9485ab6453fa7e87464aa7dd3eca")
    );

    let encoded = tw_encoding::hex::encode(signed.encoded, false);
    let transaction = signed.transaction.unwrap();

    assert_eq!(encoded, "02000000000101b11f1782607a1fe5f033ccf9dc17404db020a0dedff94183596ee67ad4177d790000000000ffffffff012202000000000000160014e311b8d6ddff856ce8e9a4e03bc6d4fe5050a83d03406a35548b8fa4620028e021a944c1d3dc6e947243a7bfc901bf63fefae0d2460efa149a6440cab51966aa4f09faef2d1e5efcba23ab4ca6e669da598022dbcfe35b0063036f7264010118746578742f706c61696e3b636861727365743d7574662d3800377b2270223a226272632d3230222c226f70223a227472616e73666572222c227469636b223a226f616466222c22616d74223a223230227d6821c00f209b6ada5edb42c77fd2bc64ad650ae38314c8f451f3e36d80bc8e26f132cb00000000");
    assert_eq!(transaction.inputs.len(), 1);
    assert_eq!(transaction.outputs.len(), 1);
}
