mod common;

use crate::common::{btc_info, dust_threshold, input, output, DUST};
use common::{MINER_FEE, ONE_BTC};
use tw_bitcoin::entry::BitcoinEntry;
use tw_coin_entry::coin_entry::CoinEntry;
use tw_coin_entry::test_utils::test_context::TestCoinContext;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_proto::BitcoinV3::Proto;
use tw_proto::Common::Proto::SigningError;
use tw_utxo::sighash::SighashBase;

#[test]
fn coin_entry_sign_input_p2pkh_output_p2pkh() {
    let coin = TestCoinContext::default();

    let alice_private_key = "56429688a1a6b00b90ccd22a0de0a376b6569d8684022ae92229a28478bfb657"
        .decode_hex()
        .unwrap();
    let alice_pubkey = "036666dd712e05a487916384bfcd5973eb53e8038eccbbf97f7eed775b87389536"
        .decode_hex()
        .unwrap();
    // let _bob_private_key = "b7da1ec42b19085fe09fec54b9d9eacd998ae4e6d2ad472be38d8393391b9ead".decode_hex().unwrap();
    let bob_pubkey = "037ed9a436e11ec4947ac4b7823787e24ba73180f1edd2857bff19c9f4d62b65bf"
        .decode_hex()
        .unwrap();

    // Create transaction with P2PKH as input and output.
    let txid = "1e1cdc48aa990d7e154a161d5b5f1cad737742e97d2712ab188027bb42e6e47b";
    let tx1 = Proto::Input {
        out_point: Some(input::out_point(txid, 0)),
        value: ONE_BTC * 50,
        sighash_type: SighashBase::All as u32,
        claiming_script: input::p2pkh(alice_pubkey.clone()),
        ..Default::default()
    };

    let out1 = Proto::Output {
        value: ONE_BTC * 50 - MINER_FEE,
        to_recipient: output::p2pkh(bob_pubkey.clone()),
    };

    let signing = Proto::SigningInput {
        private_keys: vec![alice_private_key.into()],
        inputs: vec![tx1],
        outputs: vec![out1],
        input_selector: Proto::InputSelector::UseAll,
        chain_info: Some(btc_info()),
        dust_policy: dust_threshold(DUST),
        ..Default::default()
    };

    let signed = BitcoinEntry.sign(&coin, signing);
    let encoded = signed.encoded.to_hex();

    assert_eq!(signed.error, SigningError::OK, "{}", signed.error_message);
    assert_eq!(&encoded, "02000000017be4e642bb278018ab12277de9427773ad1c5f5b1d164a157e0d99aa48dc1c1e000000006a473044022078eda020d4b86fcb3af78ef919912e6d79b81164dbbb0b0b96da6ac58a2de4b102201a5fd8d48734d5a02371c4b5ee551a69dca3842edbf577d863cf8ae9fdbbd4590121036666dd712e05a487916384bfcd5973eb53e8038eccbbf97f7eed775b87389536ffffffff01c0aff629010000001976a9145eaaa4f458f9158f86afcba08dd7448d27045e3d88ac00000000");
}
