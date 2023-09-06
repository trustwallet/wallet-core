use crate::{aliases::*, BitcoinEntry};
use tw_coin_entry::coin_entry::CoinEntry;
use tw_coin_entry::modules::plan_builder::PlanBuilder;
use tw_proto::BitcoinV2::Proto;
use tw_proto::Utxo::Proto as UtxoProto;

pub struct BitcoinPlanBuilder;

impl PlanBuilder for BitcoinPlanBuilder {
    type SigningInput<'a> = Proto::ComposePlan<'a>;
    type Plan = Proto::TransactionPlan<'static>;

    #[inline]
    fn plan(
        &self,
        _coin: &dyn tw_coin_entry::coin_context::CoinContext,
        _input: Self::SigningInput<'_>,
    ) -> Self::Plan {
        todo!()
    }
}

impl BitcoinPlanBuilder {
    fn plan_brc20(
        &self,
        _coin: &dyn tw_coin_entry::coin_context::CoinContext,
        proto: Proto::mod_ComposePlan::ComposeBrc20Plan<'static>,
    ) -> Proto::mod_TransactionPlan::Brc20Plan<'static> {
        let brc20_info = proto.inscription.unwrap();
        //let tagged_output = proto.tagged_output.unwrap();
        let tagged_output = Default::default();

        let txid = vec![0; 32];
        let brc20_input = Proto::Input {
            txid: txid.into(),
            sighash_type: UtxoProto::SighashType::UseDefault,
            to_recipient: ProtoInputRecipient::builder(Proto::mod_Input::InputBuilder {
                variant: ProtoInputBuilder::brc20_inscribe(brc20_info.clone()),
            }),
            ..Default::default()
        };

        let reveal_signing = Proto::SigningInput {
            inputs: vec![brc20_input],
            outputs: vec![tagged_output],
            input_selector: UtxoProto::InputSelector::UseAll,
            disable_change_output: true,
            ..Default::default()
        };

        let presigned = BitcoinEntry.preimage_hashes(_coin, reveal_signing.clone());
        let fee_estimate = presigned.fee_estimate;
        // TODO:
        let commit_txid = vec![0; 32];

        let dust_limit: u64 = if proto.dust_limit == 0 {
            546
        } else {
            proto.dust_limit
        };

        let brc20_output = Proto::Output {
            value: fee_estimate + dust_limit,
            to_recipient: ProtoOutputRecipient::builder(Proto::mod_Output::OutputBuilder {
                variant: ProtoOutputBuilder::brc20_inscribe(
                    Proto::mod_Output::OutputBrc20Inscription {
                        inscribe_to: brc20_info.inscribe_to.to_vec().into(),
                        ticker: brc20_info.ticker.to_string().into(),
                        transfer_amount: brc20_info.transfer_amount,
                    },
                ),
            }),
        };

        let commit_signing = Proto::SigningInput {
            inputs: proto.inputs.into_iter().map(clone_proto_input).collect(),
            outputs: vec![brc20_output],
            input_selector: proto.input_selector,
            disable_change_output: proto.disable_change_output,
            ..Default::default()
        };

        // TODO: Should be commit_presigned = 0
        let _txid = vec![0; 32];

        Proto::mod_TransactionPlan::Brc20Plan {
            commit: Some(commit_signing),
            reveal: Some(reveal_signing),
        }
    }
}

fn clone_proto_input(proto: Proto::Input<'_>) -> Proto::Input<'static> {
    fn new_builder(variant: ProtoInputBuilder<'static>) -> ProtoInputRecipient<'static> {
        ProtoInputRecipient::builder(Proto::mod_Input::InputBuilder { variant })
    }

    let to_recipient = match proto.to_recipient {
        ProtoInputRecipient::builder(builder) => match builder.variant {
            ProtoInputBuilder::p2sh(script) => {
                new_builder(ProtoInputBuilder::p2sh(script.to_vec().into()))
            },
            ProtoInputBuilder::p2pkh(script) => {
                new_builder(ProtoInputBuilder::p2pkh(script.to_vec().into()))
            },
            ProtoInputBuilder::p2wsh(script) => {
                new_builder(ProtoInputBuilder::p2wsh(script.to_vec().into()))
            },
            ProtoInputBuilder::p2wpkh(script) => {
                new_builder(ProtoInputBuilder::p2wpkh(script.to_vec().into()))
            },
            ProtoInputBuilder::p2tr_key_path(key_path) => new_builder(
                ProtoInputBuilder::p2tr_key_path(Proto::mod_Input::InputTaprootKeyPath {
                    one_prevout: key_path.one_prevout,
                    public_key: key_path.public_key.to_vec().into(),
                }),
            ),
            ProtoInputBuilder::p2tr_script_path(script) => new_builder(
                ProtoInputBuilder::p2tr_script_path(Proto::mod_Input::InputTaprootScriptPath {
                    one_prevout: script.one_prevout,
                    payload: script.payload.to_vec().into(),
                    control_block: script.control_block.to_vec().into(),
                }),
            ),
            ProtoInputBuilder::brc20_inscribe(brc20) => new_builder(
                ProtoInputBuilder::brc20_inscribe(Proto::mod_Input::InputBrc20Inscription {
                    one_prevout: brc20.one_prevout,
                    inscribe_to: brc20.inscribe_to.to_vec().into(),
                    ticker: brc20.ticker.to_string().into(),
                    transfer_amount: brc20.transfer_amount,
                }),
            ),
            ProtoInputBuilder::ordinal_inscribe(ord) => new_builder(
                ProtoInputBuilder::ordinal_inscribe(Proto::mod_Input::InputOrdinalInscription {
                    one_prevout: ord.one_prevout,
                    inscribe_to: ord.inscribe_to.to_vec().into(),
                    mime_type: ord.mime_type.to_string().into(),
                    payload: ord.payload.to_vec().into(),
                }),
            ),
            ProtoInputBuilder::None => todo!(),
        },
        ProtoInputRecipient::custom_script(custom) => {
            ProtoInputRecipient::custom_script(Proto::mod_Input::InputScriptWitness {
                script_pubkey: custom.script_pubkey.to_vec().into(),
                script_sig: custom.script_sig.to_vec().into(),
                witness_items: custom
                    .witness_items
                    .iter()
                    .map(|item| item.to_vec().into())
                    .collect(),
                signing_method: custom.signing_method,
            })
        },
        ProtoInputRecipient::None => todo!(),
    };

    Proto::Input {
        private_key: proto.private_key.to_vec().into(),
        txid: proto.txid.to_vec().into(),
        to_recipient,
        ..proto
    }
}
