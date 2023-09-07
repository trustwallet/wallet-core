mod common;

use common::{hex, ONE_BTC};
use tw_bitcoin::aliases::*;
use tw_bitcoin::BitcoinEntry;
use tw_coin_entry::coin_entry::CoinEntry;
use tw_coin_entry::modules::plan_builder::PlanBuilder;
use tw_coin_entry::test_utils::empty_context::EmptyCoinContext;
use tw_proto::BitcoinV2::Proto;
use tw_proto::Utxo::Proto as UtxoProto;

#[test]
fn transaction_plan_compose_brc20() {
    let _coin = EmptyCoinContext;

    let alice_private_key = hex("e253373989199da27c48680e3a3fc0f648d50f9a727ef17a7fe6a4dc3b159129");
    let alice_pubkey = hex("030f209b6ada5edb42c77fd2bc64ad650ae38314c8f451f3e36d80bc8e26f132cb");

    let txid1: Vec<u8> = hex("181c84965c9ea86a5fac32fdbd5f73a21a7a9e749fb6ab97e273af2329f6b911")
        .into_iter()
        .rev()
        .collect();

    let tx1 = Proto::Input {
        txid: txid1.as_slice().into(),
        vout: 0,
        value: ONE_BTC,
        sighash_type: UtxoProto::SighashType::All,
        to_recipient: ProtoInputRecipient::builder(Proto::mod_Input::InputBuilder {
            variant: ProtoInputBuilder::p2wpkh(alice_pubkey.as_slice().into()),
        }),
        ..Default::default()
    };

    let txid2: Vec<u8> = hex("858e450a1da44397bde05ca2f8a78510d74c623cc2f69736a8b3fbfadc161f6e")
        .into_iter()
        .rev()
        .collect();

    let tx2 = Proto::Input {
        txid: txid2.as_slice().into(),
        vout: 0,
        value: ONE_BTC * 2,
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

    let brc20_inscription = Proto::mod_Input::InputBrc20Inscription {
        one_prevout: false,
        inscribe_to: alice_pubkey.as_slice().into(),
        ticker: "oadf".into(),
        transfer_amount: 20,
    };

    let compose = Proto::ComposePlan {
        compose: Proto::mod_ComposePlan::OneOfcompose::brc20(
            Proto::mod_ComposePlan::ComposeBrc20Plan {
                inputs: vec![tx1, tx2],
                input_selector: UtxoProto::InputSelector::SelectAscending,
                tagged_output: Some(tagged_output),
                inscription: Some(brc20_inscription),
                fee_per_vb: 25,
                change_output: Some(change_output.clone()),
                disable_change_output: false,
            },
        ),
    };

    let builder = BitcoinEntry.plan_builder().unwrap();
    let built = builder.plan(&_coin, compose);

    if let Proto::mod_TransactionPlan::OneOfplan::brc20(plan) = built.plan {
        let commit = plan.commit.unwrap();

        // One input covers all outputs.
        assert_eq!(commit.inputs.len(), 1);
        // BRC20 inscription output + change.
        assert_eq!(commit.outputs.len(), 2);
        assert_eq!(commit.input_selector, UtxoProto::InputSelector::UseAll);
        assert_eq!(commit.fee_per_vb, 25);
        assert_eq!(commit.change_output, Default::default());
        assert!(commit.disable_change_output);

        let reveal = plan.reveal.unwrap();

        // One inputs covers all outputs.
        assert_eq!(reveal.inputs.len(), 1);
        assert_eq!(reveal.outputs.len(), 1);
        assert_eq!(reveal.input_selector, UtxoProto::InputSelector::UseAll);
        assert_eq!(reveal.fee_per_vb, 25);
        assert_eq!(reveal.change_output, Default::default());
        assert!(reveal.disable_change_output);
    } else {
        panic!()
    }
}
