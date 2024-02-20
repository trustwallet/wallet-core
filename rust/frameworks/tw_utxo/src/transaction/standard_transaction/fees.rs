use super::{Transaction, TransactionInput, TransactionOutput};
use crate::{
    encode::compact_integer::CompactInteger,
    transaction::{
        transaction_fee::TransactionFee, transaction_interface::TransactionInterface,
        transaction_parts::Amount,
    },
};

impl TransactionFee for Transaction {
    fn fee(&self, fee_rate: Amount) -> Amount {
        // TODO: Check casting. And why is Amount = i64?
        Amount::from(self.vsize() as u32) * fee_rate
    }
}
