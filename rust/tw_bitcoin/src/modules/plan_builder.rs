use tw_coin_entry::modules::plan_builder::PlanBuilder;
use tw_proto::BitcoinV2::Proto;

pub struct BitcoinPlanBuilder;

impl PlanBuilder for BitcoinPlanBuilder {
	type SigningInput<'a> = Proto::PlanInput;
	type Plan = Proto::PlanOutput;

	#[inline]
	fn plan(&self, _coin: &dyn tw_coin_entry::coin_context::CoinContext, _input: Self::SigningInput<'_>) -> Self::Plan {
		todo!()
	}
}
