use bitcoin::witness;

use crate::{
    sighash_computer::{TxSigningArgs, UtxoToSign},
    transaction::{transaction_interface::TxInputInterface, transaction_parts::Amount},
};

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

    // TODO: This trait should only have those methods.
    fn fee(&self, fee_rate: Amount) -> Amount;
    // Computes the fee for a given scriptSig size.
    fn fee_for_script_sig(bytes: usize, fee_rate: Amount) -> Amount;
    // Computes the fee for a given witness size.
    fn fee_for_witness(bytes: usize, fee_rate: Amount) -> Amount;

    fn select_inputs(
        &mut self,
        mut args: TxSigningArgs,
        fee_rate: Amount,
        selector: InputSelector,
        // TODO: Adjust return type.
    ) -> Result<(TxSigningArgs, Amount, Amount), ()> {
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
        let mut selected_utxo = Vec::with_capacity(utxos.len());
        let mut selected_args = Vec::with_capacity(utxos.len());

        let mut total_in = Amount::from(0);
        let mut script_fee = Amount::from(0);

        let mut total_covered = false;
        for (input, arg) in utxos {
            dbg!("new input");
            // There is no scriptSig or witness in the input.
            debug_assert!(!input.has_script_sig());
            debug_assert!(!input.has_witness());

            let script_sig_size_hint = arg.script_sig_size_hint;
            let witness_size_hint = arg.witness_size_hint;

            total_in += arg.amount;

            selected_utxo.push(input);
            selected_args.push(arg);

            tx.replace_inputs(selected_utxo.clone());

            // Since we don't have the scriptSig or the witness, we need to
            // calculate the fee based on the provided size hints.
            let script_sig_fee = Self::fee_for_script_sig(script_sig_size_hint, fee_rate);
            script_fee += script_sig_fee;
            dbg!(script_sig_fee);

            let witness_fee = Self::fee_for_witness(witness_size_hint, fee_rate);
            script_fee += witness_fee;
            dbg!(witness_fee);

            let total_fee = tx.fee(fee_rate) + script_fee;
            if total_in >= total_out + total_fee {
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
        let total_fee = self.fee(fee_rate) + script_fee;
        let change = total_in - total_out - total_fee;

        Ok((args, total_fee, change))
    }
}
