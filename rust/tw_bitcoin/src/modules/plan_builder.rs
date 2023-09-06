use tw_coin_entry::modules::plan_builder::PlanBuilder;
use tw_proto::BitcoinV2::Proto;

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
        _input: Proto::mod_ComposePlan::ComposeBrc20Plan<'_>,
    ) -> Proto::mod_TransactionPlan::Brc20Plan {
        todo!()
    }
}
