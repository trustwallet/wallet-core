// TODO: Deprecate this module, move this somewhere else.

use crate::{
    sighash_computer::{TxSigningArgs, UtxoToSign},
    transaction::{transaction_interface::TxInputInterface, transaction_parts::Amount},
};

use super::transaction_interface::{TransactionInterface, TxOutputInterface};

pub trait TransactionFee: TransactionInterface
where
    Self::Output: TxOutputInterface,
{
    fn fee(&self, fee_rate: Amount) -> Amount;
}
