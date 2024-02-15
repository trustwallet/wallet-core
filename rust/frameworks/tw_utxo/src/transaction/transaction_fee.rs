use crate::{sighash_computer::UtxoToSign, transaction::transaction_parts::Amount};

use super::transaction_interface::{TransactionInterface, TxOutputInterface};

#[non_exhaustive]
pub enum InputSelector {
    Ascending,
    Descending,
    InOrder,
}

pub trait TransactionFee: TransactionInterface
where
    Self::Output: TxOutputInterface,
{
    fn size(&self) -> usize;
    fn vsize(&self) -> usize;
    fn weight(&self) -> usize;
    fn fee(&self, fee_rate: Amount) -> Amount;

    fn select_inputs(
        &mut self,
        args: &[UtxoToSign],
        fee_rate: Amount,
        selector: InputSelector,
    ) -> Result<(), ()> {
        let total_out = self
            .outputs()
            .iter()
            .map(|output| output.value())
            .sum::<Amount>();

        debug_assert_eq!(args.len(), self.inputs().len());

        // We clone transaction so that the orginal remains unchanged in case we
        // return an error early.
        let mut tx = self.clone();

        let mut utxos: Vec<(Self::Input, Amount)> = tx
            .inputs()
            .into_iter()
            .cloned()
            .zip(args.into_iter().map(|a| a.amount))
            .collect::<Vec<_>>();

        match selector {
            InputSelector::Ascending => {
                utxos.sort_by(|(_, a), (_, b)| a.partial_cmp(b).unwrap());
            },
            InputSelector::Descending => {
                utxos.sort_by(|(_, a), (_, b)| b.partial_cmp(a).unwrap());
            },
            InputSelector::InOrder => {
                // Nothing to do.
            },
        }

        let mut total_in = Amount::from(0);

        let mut selected = Vec::with_capacity(utxos.len());
        for (input, value) in utxos {
            total_in += value;
            selected.push(input);

            tx.replace_inputs(selected.clone());

            if total_in >= total_out + tx.fee(fee_rate) {
                // Update inputs
                self.replace_inputs(selected);

                return Ok(());
            }
        }

        // Insufficient funds.
        Err(())
    }
}

pub trait TxIndividualFee {
    fn size(&self) -> usize;
    fn vsize(&self) -> usize;
    fn weight(&self) -> usize;
}
