mod common;

use bitcoin::consensus::Encodable;
use bitcoin::{PublicKey, ScriptBuf, Witness};
use common::{hex, MINER_FEE, ONE_BTC};
use tw_bitcoin::aliases::*;
use tw_bitcoin::entry::BitcoinEntry;
use tw_bitcoin::modules::signer::Signer;
use tw_coin_entry::coin_entry::CoinEntry;
use tw_coin_entry::test_utils::test_context::TestCoinContext;
use tw_proto::BitcoinV2::Proto;
use tw_proto::Utxo::Proto as UtxoProto;

#[test]
fn coin_entry_sign_input_p2pkh_output_p2wsh() {
    let coin = TestCoinContext::default();

    let alice_private_key = hex("56429688a1a6b00b90ccd22a0de0a376b6569d8684022ae92229a28478bfb657");
    let alice_pubkey = hex("036666dd712e05a487916384bfcd5973eb53e8038eccbbf97f7eed775b87389536");
    let bob_private_key = hex("b7da1ec42b19085fe09fec54b9d9eacd998ae4e6d2ad472be38d8393391b9ead");
    let bob_pubkey = hex("037ed9a436e11ec4947ac4b7823787e24ba73180f1edd2857bff19c9f4d62b65bf");

    // Create transaction with P2WSH as output (spend).

    let txid: Vec<u8> = hex("c01007bb55bde4e70278e1154c34db72f34a833687d3f37443bd5c49137ee5fe")
        .into_iter()
        .rev()
        .collect();

    // We use a simple P2PKH as the redeem script (ie. P2PKH embedded inside P2WSH).
    let bob_native_pubkey = PublicKey::from_slice(&bob_pubkey).unwrap();
    let redeem_script = ScriptBuf::new_p2pkh(&bob_native_pubkey.pubkey_hash());

    let tx1 = Proto::Input {
        txid: txid.as_slice().into(),
        vout: 0,
        value: 50 * ONE_BTC - 2 * MINER_FEE,
        sighash_type: UtxoProto::SighashType::All,
        to_recipient: ProtoInputRecipient::builder(Proto::mod_Input::InputBuilder {
            variant: ProtoInputBuilder::p2pkh(alice_pubkey.as_slice().into()),
        }),
        ..Default::default()
    };

    let out1 = Proto::Output {
        value: 50 * ONE_BTC - 3 * MINER_FEE,
        to_recipient: ProtoOutputRecipient::builder(Proto::mod_Output::OutputBuilder {
            variant: ProtoOutputBuilder::p2wsh(Proto::mod_Output::OutputRedeemScriptOrHash {
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
        change_output: Default::default(),
        disable_change_output: true,
        ..Default::default()
    };

    let signed = BitcoinEntry.sign(&coin, signing);
    let encoded = tw_encoding::hex::encode(signed.encoded, false);
    assert_eq!(signed.error, Proto::Error::OK);
    assert_eq!(&encoded, "0200000001fee57e13495cbd4374f3d38736834af372db344c15e17802e7e4bd55bb0710c0000000006b483045022100edd19b379131b9f7a05ff2a79313ccec181f2e3fd06901e27ffff4fa500dbf95022060ae976cd70b8af956fdf4582bb1289bdf233a5bace82e69ea9b7cfb55c583370121036666dd712e05a487916384bfcd5973eb53e8038eccbbf97f7eed775b87389536ffffffff01402bd82901000000220020883a539555e537e0498732376a3d4d282e304bce7bfda6876a2b63b08a04f54400000000");

    // Create transaction with P2WSH as input (claim).

    let txid: Vec<u8> = hex("dd9d4ca23532f5c89d016e1aacef1210ab5b9d00527c633969841daca7dd17c7")
        .into_iter()
        .rev()
        .collect();

    let tx1 = Proto::Input {
        txid: txid.as_slice().into(),
        vout: 0,
        value: 50 * ONE_BTC - 3 * MINER_FEE,
        sighash_type: UtxoProto::SighashType::All,
        to_recipient: ProtoInputRecipient::builder(Proto::mod_Input::InputBuilder {
            // The way P2WSH is signed in Bitcoin, we first place the redeem script directly here.
            variant: ProtoInputBuilder::p2wsh(redeem_script.to_bytes().into()),
        }),
        ..Default::default()
    };

    let out1 = Proto::Output {
        value: 50 * ONE_BTC - 4 * MINER_FEE,
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

    // Sign the sighashes.
    let signatures = Signer::signatures_from_proto(
        &sighashes,
        bob_private_key.to_vec(),
        Default::default(),
        false,
    )
    .unwrap();

    let sig = &signatures[0];

    // Construc the final redeem witness stack with the necessary witness stack items (signature + pubkey).
    let mut finalized = Witness::new();
    finalized.push(sig);
    finalized.push(bob_native_pubkey.to_bytes());
    finalized.push(redeem_script);

    // The witness stack must be encoded correctly.
    let mut encoded = vec![];
    let _ = finalized.consensus_encode(&mut encoded).unwrap();

    // Now that we've signed the input, we update the input with the complete,
    // finalized redeem witness stack..
    signing.inputs[0].to_recipient = ProtoInputRecipient::builder(Proto::mod_Input::InputBuilder {
        variant: ProtoInputBuilder::p2wsh(encoded.into()),
    });

    // Compile the final transaction.
    let signed = BitcoinEntry.compile(&coin, signing, signatures, vec![]);
    let encoded = tw_encoding::hex::encode(signed.encoded, false);
    assert_eq!(signed.error, Proto::Error::OK);
    assert_eq!(&encoded, "02000000000101c717dda7ac1d846939637c52009d5bab1012efac1a6e019dc8f53235a24c9ddd0000000000ffffffff0100e9c829010000001976a914e4c1ea86373d554b8f4efff2cfb0001ea19124d288ac0347304402201d22810b5580a49a2e73d7c4ea90754b5d70d36adb9a8f0c9cb7393da1d1d28f02207683b2e3d31a5c7e74126681f1f2a7249b7a3a918d5890ef69b94bd3bb4fb9300121037ed9a436e11ec4947ac4b7823787e24ba73180f1edd2857bff19c9f4d62b65bf1976a9145eaaa4f458f9158f86afcba08dd7448d27045e3d88ac00000000");
}
