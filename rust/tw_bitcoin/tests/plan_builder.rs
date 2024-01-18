mod common;

use common::{hex, ONE_BTC};
use tw_bitcoin::aliases::*;
use tw_bitcoin::BitcoinEntry;
use tw_coin_entry::coin_entry::CoinEntry;
use tw_coin_entry::modules::plan_builder::PlanBuilder;
use tw_coin_entry::test_utils::test_context::TestCoinContext;
use tw_proto::BitcoinV2::Proto;
use tw_proto::Utxo::Proto as UtxoProto;

const SAT_VBYTE: u64 = 25;

#[test]
fn transaction_plan_compose_brc20() {
    let _coin = TestCoinContext::default();

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
                private_key: alice_private_key.clone().into(),
                inputs: vec![tx1.clone(), tx2.clone()],
                input_selector: UtxoProto::InputSelector::SelectAscending,
                tagged_output: Some(tagged_output.clone()),
                inscription: Some(brc20_inscription),
                fee_per_vb: SAT_VBYTE,
                change_output: Some(change_output.clone()),
                disable_change_output: false,
            },
        ),
    };

    // Compute plan
    let builder = BitcoinEntry.plan_builder().unwrap();
    let built = builder.plan(&_coin, compose);
    assert_eq!(built.error, Proto::Error::OK);

    // Build the plan, returning the signed COMMIT and REVEAL transactions.
    let Proto::mod_TransactionPlan::OneOfplan::brc20(plan) = built.plan else { panic!() };

    let commit = plan.commit.unwrap();
    let reveal = plan.reveal.unwrap();

    let rev_commit_txid: Vec<u8> = {
        assert_eq!(commit.error, Proto::Error::OK);
        assert!(commit.error_message.is_empty());
        assert_eq!(commit.weight, 608);
        assert_eq!(commit.fee, (commit.weight + 3) / 4 * SAT_VBYTE);
        assert_eq!(commit.fee, 3_800);

        let tx = commit.transaction.unwrap();
        assert_eq!(tx.version, 2);

        // Inputs, only one input was selected (ONE_BTC * 2).
        assert_eq!(tx.inputs.len(), 1);
        assert_eq!(tx.inputs[0].txid, tx1.txid);
        assert_eq!(tx.inputs[0].vout, 0);
        assert_eq!(tx.inputs[0].sequence, u32::MAX);
        assert!(tx.inputs[0].script_sig.is_empty());
        assert!(!tx.inputs[0].witness_items.is_empty());

        // Outputs (BRC20 inscription + change output).
        assert_eq!(tx.outputs.len(), 2);

        // Output for recipient.
        assert!(!tx.outputs[0].script_pubkey.is_empty());
        // Since this output is used as a single input in the REVEAL
        // transaction, the amount of stats here covers just enough for the
        // REVEAL fee plus the tagged satoshis.
        assert_eq!(tx.outputs[0].value, reveal.fee + tagged_output.value);
        assert_eq!(tx.outputs[0].value, 3846);
        assert!(!tx.outputs[0].taproot_payload.is_empty());
        assert!(!tx.outputs[0].control_block.is_empty());

        // Change output.
        assert!(!tx.outputs[1].script_pubkey.is_empty());
        assert_eq!(
            tx.outputs[1].value,
            tx1.value - tx.outputs[0].value - commit.fee
        );
        assert_eq!(tx.outputs[1].value, ONE_BTC - 3846 - 3_800);
        assert!(tx.outputs[1].taproot_payload.is_empty());
        assert!(tx.outputs[1].control_block.is_empty());

        commit.txid.iter().copied().rev().collect()
    };

    {
        assert_eq!(reveal.error, Proto::Error::OK);
        assert!(reveal.error_message.is_empty());
        assert_eq!(reveal.weight, 527);
        assert_eq!(reveal.fee, (reveal.weight + 3) / 4 * SAT_VBYTE);
        assert_eq!(reveal.fee, 3_300);

        let tx = reveal.transaction.unwrap();
        assert_eq!(tx.version, 2);

        // Inputs, only one input was selected (ONE_BTC * 2).
        assert_eq!(tx.inputs.len(), 1);
        // Reference the COMMIT transaction(!).
        assert_eq!(tx.inputs[0].txid, rev_commit_txid);
        assert_eq!(tx.inputs[0].vout, 0);
        assert_eq!(tx.inputs[0].sequence, u32::MAX);
        assert!(tx.inputs[0].script_sig.is_empty());
        assert!(!tx.inputs[0].witness_items.is_empty());

        // Outputs (only tagged output, no change output).
        assert_eq!(tx.outputs.len(), 1);

        // Output for recipient.
        assert!(!tx.outputs[0].script_pubkey.is_empty());
        assert_eq!(tx.outputs[0].value, tagged_output.value);
        assert_eq!(tx.outputs[0].value, 546);
        assert!(tx.outputs[0].taproot_payload.is_empty());
        assert!(tx.outputs[0].control_block.is_empty());
    }
}
