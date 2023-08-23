use super::hex;
use crate::entry::{
    BitcoinEntry, PlaceHolder, ProtoInputBuilder, ProtoInputRecipient, ProtoOutputBuilder,
    ProtoOutputRecipient,
};
use tw_coin_entry::coin_entry::CoinEntry;
use tw_proto::BitcoinV2::Proto;
use tw_proto::Utxo::Proto as UtxoProto;

#[test]
fn coin_entry_sign_ordinal_nft_commit_reveal_transfer() {
    let coin = PlaceHolder;

    let alice_private_key = hex("e253373989199da27c48680e3a3fc0f648d50f9a727ef17a7fe6a4dc3b159129");
    let alice_pubkey = hex("030f209b6ada5edb42c77fd2bc64ad650ae38314c8f451f3e36d80bc8e26f132cb");

    let txid: Vec<u8> = hex("579590c3227253ad423b1e7e3c5b073b8a280d307c68aecd779df2600daa2f99")
        .into_iter()
        .rev()
        .collect();

    let tx1 = Proto::Input {
        txid: txid.as_slice().into(),
        vout: 0,
        amount: 32_400,
        sequence: u32::MAX,
        sequence_enable_zero: false,
        sighash_type: UtxoProto::SighashType::All,
        to_recipient: ProtoInputRecipient::builder(Proto::mod_Input::Builder {
            variant: ProtoInputBuilder::p2wpkh(alice_pubkey.as_slice().into()),
        }),
    };

    let out1 = Proto::Output {
        amount: 31_100,
        to_recipient: ProtoOutputRecipient::builder(Proto::mod_Output::Builder {
            variant: ProtoOutputBuilder::ordinal_inscribe(Proto::mod_Output::OrdinalInscription {
                inscribe_to: alice_pubkey.as_slice().into(),
                mime_type: "image/png".into(),
                payload: hex(super::data::NFT_INSCRIPTION_IMAGE_DATA).into(),
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
    let transaction = output.transaction.unwrap();

    assert_eq!(transaction.inputs.len(), 1);
    assert_eq!(transaction.outputs.len(), 1);
    assert_eq!(&encoded, "02000000000101992faa0d60f29d77cdae687c300d288a3b075b3c7e1e3b42ad537222c39095570000000000ffffffff017c790000000000002251202ac69a7e9dba801e9fcba826055917b84ca6fba4d51a29e47d478de603eedab602473044022054212984443ed4c66fc103d825bfd2da7baf2ab65d286e3c629b36b98cd7debd022050214cfe5d3b12a17aaaf1a196bfeb2f0ad15ffb320c4717eb7614162453e4fe0121030f209b6ada5edb42c77fd2bc64ad650ae38314c8f451f3e36d80bc8e26f132cb00000000");

    let txid: Vec<u8> = hex("f1e708e5c5847339e16accf8716c14b33717c14d6fe68f9db36627cecbde7117")
        .into_iter()
        .rev()
        .collect();

    let tx1 = Proto::Input {
        txid: txid.as_slice().into(),
        vout: 0,
        amount: 31_100,
        sequence: u32::MAX,
        sequence_enable_zero: false,
        sighash_type: UtxoProto::SighashType::UseDefault,
        to_recipient: ProtoInputRecipient::builder(Proto::mod_Input::Builder {
            variant: ProtoInputBuilder::ordinal_inscribe(Proto::mod_Input::OrdinalInscription {
                one_prevout: false,
                inscribe_to: alice_pubkey.as_slice().into(),
                mime_type: "image/png".into(),
                payload: hex(super::data::NFT_INSCRIPTION_IMAGE_DATA).into(),
            }),
        }),
    };

    let out1 = Proto::Output {
        amount: 546,
        to_recipient: ProtoOutputRecipient::builder(Proto::mod_Output::Builder {
            variant: ProtoOutputBuilder::p2wpkh(Proto::ToPublicKeyOrHash {
                to_address: Proto::mod_ToPublicKeyOrHash::OneOfto_address::pubkey(
                    alice_pubkey.as_slice().into(),
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
    let transaction = output.transaction.unwrap();

    const REVEAL_RAW: &str = super::data::NFT_INSCRIPTION_RAW_HEX;

    assert_eq!(transaction.inputs.len(), 1);
    assert_eq!(transaction.outputs.len(), 1);

    assert_eq!(encoded[..164], REVEAL_RAW[..164]);
    assert_ne!(encoded[164..292], REVEAL_RAW[164..292]);
    assert_eq!(encoded[292..], REVEAL_RAW[292..]);
}
