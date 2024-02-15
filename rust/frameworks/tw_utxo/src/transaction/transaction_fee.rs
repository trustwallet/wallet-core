use crate::{sighash_computer::{TxSigningArgs, UtxoToSign}, transaction::{transaction_interface::TxInputInterface, transaction_parts::Amount}};

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
    fn base_size(&self) -> usize;
    fn size(&self) -> usize;
    fn weight(&self) -> usize;
    fn vsize(&self) -> usize;
    fn fee(&self, fee_rate: Amount) -> Amount;

    fn select_inputs(
        &mut self,
        mut args: TxSigningArgs,
        fee_rate: Amount,
        selector: InputSelector,
    ) -> Result<(TxSigningArgs, Amount), ()> {
        let total_out = self
            .outputs()
            .iter()
            .map(|output| output.value())
            .sum::<Amount>();

        debug_assert_eq!(args.utxos_to_sign.len(), self.inputs().len());

        // We clone transaction so that the orginal remains unchanged in case we
        // return an error early.
        let mut tx = self.clone();

        // Prepare the available UTXOs.
        let mut utxos: Vec<(Self::Input, UtxoToSign)> = tx
            .inputs()
            .into_iter()
            .cloned()
            .zip(args.utxos_to_sign.into_iter())
            .collect::<Vec<_>>();

        // Sort the UTXOs.
        match selector {
            InputSelector::InOrder => {
                // Nothing to do.
            },
            InputSelector::Ascending => {
                utxos.sort_by(|(_, a), (_, b)| a.amount.partial_cmp(&b.amount).unwrap());
            },
            InputSelector::Descending => {
                utxos.sort_by(|(_, a), (_, b)| b.amount.partial_cmp(&a.amount).unwrap());
            },
        }

        // Select the UTXOs to cover all the outputs and the fee.
        let mut total_in = Amount::from(0);
        let mut selected_utxo = Vec::with_capacity(utxos.len());
        let mut selected_args = Vec::with_capacity(utxos.len());

        let mut total_covered = false;
        for (input, arg) in utxos {
            debug_assert!(!input.has_witness() && !input.has_script_sig());

            total_in += arg.amount;

            selected_utxo.push(input);
            selected_args.push(arg);

            tx.replace_inputs(selected_utxo.clone());

            if total_in >= total_out + tx.fee(fee_rate) {
                // Update self with selected in UTXOs.
                self.replace_inputs(selected_utxo);

                total_covered = true;
                break;
            }
        }

        // Explicit discarding, continuing with `self`.
        std::mem::drop(tx);

        if !total_covered {
            // Insufficient funds.
            return Err(());
        }

        // Update the args with the selected UTXOs (args).
        args.utxos_to_sign = selected_args;

        // Calculate the change.
        let change = total_in - total_out - self.fee(fee_rate);

        Ok((args, change))
    }
}
