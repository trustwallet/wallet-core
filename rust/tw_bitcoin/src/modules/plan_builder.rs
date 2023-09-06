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
        let tagged_output = super::utils::hard_clone_proto_output(proto.tagged_output.unwrap());

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
            outputs: vec![tagged_output.clone()],
            input_selector: UtxoProto::InputSelector::UseAll,
            disable_change_output: true,
            ..Default::default()
        };

        let presigned = BitcoinEntry.preimage_hashes(_coin, reveal_signing.clone());
        let fee_estimate = presigned.fee_estimate;
        let commit_txid: Vec<u8> = presigned.txid.to_vec().iter().copied().rev().collect();

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
            inputs: proto
                .inputs
                .into_iter()
                .map(super::utils::hard_clone_proto_input)
                .collect(),
            outputs: vec![brc20_output],
            input_selector: proto.input_selector,
            disable_change_output: proto.disable_change_output,
            ..Default::default()
        };

        let brc20_input = Proto::Input {
            txid: commit_txid.into(),
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

        Proto::mod_TransactionPlan::Brc20Plan {
            commit: Some(commit_signing),
            reveal: Some(reveal_signing),
        }
    }
}
