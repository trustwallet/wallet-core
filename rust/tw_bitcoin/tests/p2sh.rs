mod common;

use bitcoin::script::PushBytesBuf;
use bitcoin::{PublicKey, ScriptBuf};
use common::{hex, MINER_FEE, ONE_BTC};
use tw_bitcoin::aliases::*;
use tw_bitcoin::entry::BitcoinEntry;
use tw_bitcoin::modules::signer::Signer;
use tw_coin_entry::coin_entry::CoinEntry;
use tw_coin_entry::test_utils::test_context::TestCoinContext;
use tw_proto::BitcoinV2::Proto;
use tw_proto::Utxo::Proto as UtxoProto;

#[test]
fn coin_entry_sign_input_p2pkh_output_p2sh() {
    let coin = TestCoinContext::default();

    let alice_private_key = hex("56429688a1a6b00b90ccd22a0de0a376b6569d8684022ae92229a28478bfb657");
    let alice_pubkey = hex("036666dd712e05a487916384bfcd5973eb53e8038eccbbf97f7eed775b87389536");
    let bob_private_key = hex("b7da1ec42b19085fe09fec54b9d9eacd998ae4e6d2ad472be38d8393391b9ead");
    let bob_pubkey = hex("037ed9a436e11ec4947ac4b7823787e24ba73180f1edd2857bff19c9f4d62b65bf");

    // Create transaction with P2SH as output (spend).

    let txid: Vec<u8> = hex("e7503721268d0547b3b009dab56e5ebd8bcadbfc7dfae3468a56b5cb0c07a2f7")
        .into_iter()
        .rev()
        .collect();

    // We use a simple P2PKH as the redeem script (ie. P2PKH embedded inside P2SH).
    let bob_native_pubkey = PublicKey::from_slice(&bob_pubkey).unwrap();
    let redeem_script = ScriptBuf::new_p2pkh(&bob_native_pubkey.pubkey_hash());

    let tx1 = Proto::Input {
        txid: txid.as_slice().into(),
        vout: 0,
        value: 50 * ONE_BTC,
        sighash_type: UtxoProto::SighashType::All,
        to_recipient: ProtoInputRecipient::builder(Proto::mod_Input::InputBuilder {
            variant: ProtoInputBuilder::p2pkh(alice_pubkey.as_slice().into()),
        }),
        ..Default::default()
    };

    let out1 = Proto::Output {
        value: 50 * ONE_BTC - MINER_FEE,
        to_recipient: ProtoOutputRecipient::builder(Proto::mod_Output::OutputBuilder {
            variant: ProtoOutputBuilder::p2sh(Proto::mod_Output::OutputRedeemScriptOrHash {
                variant: ProtoOutputRedeemScriptOrHashBuilder::redeem_script(
                    redeem_script.as_bytes().into(),
                ),
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
    assert_eq!(&encoded, "0200000001f7a2070ccbb5568a46e3fa7dfcdbca8bbd5e6eb5da09b0b347058d26213750e7000000006a473044022007c88caf624c0a130fc79d2835ed5b6db49f2dea0d5e685f06138aaa4a904d690220243fe7744c8b48759e74a87075de3f548988252a770871fc1444652bb32ec46e0121036666dd712e05a487916384bfcd5973eb53e8038eccbbf97f7eed775b87389536ffffffff01c0aff6290100000017a914a519b524d55ae8972e8e0e6b9d645ab20eb2635e8700000000");

    // Create transaction with P2SH as input (claim).

    let txid: Vec<u8> = hex("5d99b77a411a879fb6fa5b442f0d121965346d8e5ab61e0d189967fd5f49bd82")
        .into_iter()
        .rev()
        .collect();

    let tx1 = Proto::Input {
        txid: txid.as_slice().into(),
        vout: 0,
        value: 50 * ONE_BTC - MINER_FEE,
        sighash_type: UtxoProto::SighashType::All,
        to_recipient: ProtoInputRecipient::builder(Proto::mod_Input::InputBuilder {
            // The way P2SH is signed in Bitcoin, we first place the redeem script directly here.
            variant: ProtoInputBuilder::p2sh(redeem_script.as_bytes().into()),
        }),
        ..Default::default()
    };

    let out1 = Proto::Output {
        value: 50 * ONE_BTC - MINER_FEE - MINER_FEE,
        to_recipient: ProtoOutputRecipient::builder(Proto::mod_Output::OutputBuilder {
            variant: ProtoOutputBuilder::p2pkh(Proto::ToPublicKeyOrHash {
                to_address: ProtoPubkeyOrHash::pubkey(alice_pubkey.as_slice().into()),
            }),
        }),
    };

    let mut signing = Proto::SigningInput {
        inputs: vec![tx1],
        outputs: vec![out1],
        input_selector: UtxoProto::InputSelector::UseAll,
        disable_change_output: true,
        ..Default::default()
    };

    // Generate the sighashes.
    let sighashes = BitcoinEntry.preimage_hashes(&coin, signing.clone());
    assert_eq!(sighashes.error, Proto::Error::OK);

    // Sign the sighashes.
    let signatures = Signer::signatures_from_proto(
        &sighashes,
        bob_private_key.to_vec(),
        Default::default(),
        false,
    )
    .unwrap();

    let sig = &signatures[0];

    // Construc the final redeem scrip with the necessary stack items (signature + pubkey).
    let mut sig_buf = PushBytesBuf::new();
    sig_buf.extend_from_slice(sig).unwrap();

    let mut redeem_buf = PushBytesBuf::new();
    redeem_buf
        .extend_from_slice(redeem_script.as_bytes())
        .unwrap();

    let finalized = ScriptBuf::builder()
        .push_slice(sig_buf)
        .push_key(&bob_native_pubkey)
        .push_slice(redeem_buf.as_push_bytes())
        .into_script();

    // Now that we've signed the input, we update the input with the complete,
    // finalized redeem script.
    signing.inputs[0].to_recipient = ProtoInputRecipient::builder(Proto::mod_Input::InputBuilder {
        variant: ProtoInputBuilder::p2sh(finalized.as_bytes().into()),
    });

    // Compile the final transaction.
    let signed = BitcoinEntry.compile(&coin, signing, signatures, vec![]);
    let encoded = tw_encoding::hex::encode(signed.encoded, false);
    assert_eq!(signed.error, Proto::Error::OK);
    assert_eq!(&encoded, "020000000182bd495ffd6799180d1eb65a8e6d346519120d2f445bfab69f871a417ab7995d000000008447304402207aad4b72c6d78c81a1e795325bd5ddb449f0a1363205903f5e37950e6b89054102202aaf4dd919700d21fe2431352df99c434378bd0d46b778b445079579300effdf0121037ed9a436e11ec4947ac4b7823787e24ba73180f1edd2857bff19c9f4d62b65bf1976a9145eaaa4f458f9158f86afcba08dd7448d27045e3d88acffffffff01806de729010000001976a914e4c1ea86373d554b8f4efff2cfb0001ea19124d288ac00000000");
}
