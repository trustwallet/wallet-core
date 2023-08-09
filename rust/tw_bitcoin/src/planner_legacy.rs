use crate::entry::{PlaceHolderProto, BitcoinEntry};
use bitcoin::VarInt;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::{PublicKeyBytes, SignatureBytes};
use tw_proto::Bitcoin::Proto::{self as BitcoinProto, UnspentTransaction};
use std::borrow::Cow;

// Legacy implementations used for `Bitcoin.proto` backwards-compatibility.
impl BitcoinEntry {
    #[inline]
    fn compile_legacy(
        &self,
        _coin: &dyn CoinContext,
        _input: PlaceHolderProto<'static>,
        _signatures: Vec<SignatureBytes>,
        _public_keys: Vec<PublicKeyBytes>,
    ) -> PlaceHolderProto<'static> {
        todo!()
    }

    #[inline]
    fn plan_builder_legacy(&self) -> Option<LegacyPlanBuilder> {
        None
    }
}

pub struct LegacyPlanBuilder;

impl LegacyPlanBuilder {
    fn plan<'a>(&self, _coin: &dyn CoinContext, proto: BitcoinProto::SigningInput<'a>) -> BitcoinProto::TransactionPlan<'a> {
        let mut inputs = proto.utxo.to_vec();
        inputs.sort_by(|a, b| a.amount.partial_cmp(&b.amount).unwrap());

        let mut total_selected_amount = 0;
        let mut remaining = proto.amount as u64;
        let selected: Vec<UnspentTransaction<'_>> = inputs.into_iter().take_while(|input| {
            remaining = remaining.saturating_sub(input.amount as u64);
            total_selected_amount += input.amount as u64;

            remaining != 0
        }).collect();

        if remaining != 0 {
            // Return error
            todo!()
        }

        let mut weight = 0;
        let mut witness_weight = 0;

        let scale_factor = 1;
        for input in &selected {
            weight += scale_factor*(
                // Outpoint
                32 + 4
                // Sequence
                + 4
                + VarInt(input.script.as_ref().len() as u64).len()
                + input.script.len()
            );

            if !input.spendingScript.is_empty() {
                witness_weight += 1;
                weight += input.spendingScript.len();
            }
        }

        let change = total_selected_amount - proto.amount as u64;

        BitcoinProto::TransactionPlan {
            amount: proto.amount as i64,
            available_amount: total_selected_amount as i64,
            fee: 0,
            change: change as i64,
            utxos: selected,
            // Used for Zcash
            branch_id: Cow::default(),
            error: tw_proto::Common::Proto::SigningError::OK,
            // Used for other chain(s).
            output_op_return: Cow::default(),
            // Used for other chain(s).
            preblockhash: Cow::default(),
            // Used for other chain(s).
            preblockheight: 0,
        }
    }
}
