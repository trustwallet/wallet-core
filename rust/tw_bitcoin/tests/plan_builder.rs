mod common;

use common::{hex, ONE_BTC};
use tw_bitcoin::aliases::*;
use tw_bitcoin::BitcoinEntry;
use tw_coin_entry::coin_entry::CoinEntry;
use tw_coin_entry::test_utils::empty_context::EmptyCoinContext;
use tw_proto::BitcoinV2::Proto;
use tw_proto::Utxo::Proto as UtxoProto;

#[test]
fn coin_entry_sign_brc20_commit_reveal_transfer() {
    let coin = EmptyCoinContext;

    let alice_private_key = hex("e253373989199da27c48680e3a3fc0f648d50f9a727ef17a7fe6a4dc3b159129");
    let alice_pubkey = hex("030f209b6ada5edb42c77fd2bc64ad650ae38314c8f451f3e36d80bc8e26f132cb");

    let txid: Vec<u8> = hex("797d17d47ae66e598341f9dfdea020b04d4017dcf9cc33f0e51f7a6082171fb1")
        .into_iter()
        .rev()
        .collect();

    let tx1 = Proto::Input {
        txid: txid.as_slice().into(),
        vout: 0,
        value: ONE_BTC,
        sighash_type: UtxoProto::SighashType::All,
        to_recipient: ProtoInputRecipient::builder(Proto::mod_Input::InputBuilder {
            variant: ProtoInputBuilder::p2wpkh(alice_pubkey.as_slice().into()),
        }),
        ..Default::default()
    };

    let tx2 = Proto::Input {
        txid: txid.as_slice().into(),
        vout: 0,
        value: ONE_BTC,
        sighash_type: UtxoProto::SighashType::All,
        to_recipient: ProtoInputRecipient::builder(Proto::mod_Input::InputBuilder {
            variant: ProtoInputBuilder::p2wpkh(alice_pubkey.as_slice().into()),
        }),
        ..Default::default()
    };

    let tagged_output = Proto::Output {
        value: 546,
        to_recipient: ProtoOutputRecipient::builder(Proto::mod_Output::OutputBuilder {
            variant: ProtoOutputBuilder::p2wpkh(Proto::ToPublicKeyOrHash {
                to_address: ProtoPubkeyOrHash::pubkey(alice_pubkey.as_slice().into()),
            }),
        }),
    };

    let change_output = Proto::Output {
        // Will be set by the library.
        value: 0,
        to_recipient: ProtoOutputRecipient::builder(Proto::mod_Output::OutputBuilder {
            variant: ProtoOutputBuilder::p2wpkh(Proto::ToPublicKeyOrHash {
                to_address: ProtoPubkeyOrHash::pubkey(alice_pubkey.as_slice().into()),
            }),
        }),
    };

    let compose = Proto::ComposePlan {
        compose: Proto::mod_ComposePlan::OneOfcompose::brc20(
            Proto::mod_ComposePlan::ComposeBrc20Plan {
                inputs: vec![tx1, tx2],
                input_selector: UtxoProto::InputSelector::SelectAscending,
                tagged_output: Some(tagged_output),
                //inscription: Option<TW::BitcoinV2::Proto::mod_Input::InputBrc20Inscription<'a>>,
                inscription: Default::default(),
                fee_per_vb: 25,
                change_output: Some(change_output),
                disable_change_output: false,
            },
        ),
    };
}
