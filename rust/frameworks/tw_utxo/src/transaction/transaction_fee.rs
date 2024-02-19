use crate::{
    sighash_computer::{TxSigningArgs, UtxoToSign},
    transaction::{transaction_interface::TxInputInterface, transaction_parts::Amount},
};

use super::transaction_interface::{TransactionInterface, TxOutputInterface};

pub trait TransactionFee: TransactionInterface
where
    Self::Output: TxOutputInterface,
{
    fn base_size(&self) -> usize;
    fn size(&self) -> usize;
    fn weight(&self) -> usize;
    fn vsize(&self) -> usize;
    fn fee(&self, fee_rate: Amount) -> Amount;
}
