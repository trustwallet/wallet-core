use super::Transaction;
use crate::transaction::{transaction_fee::TransactionFee, transaction_parts::Amount};

impl TransactionFee for Transaction {
    fn fee(&self, fee_rate: Amount) -> Amount {
        // TODO: Check casting. And why is Amount = i64?
        Amount::from(self.vsize() as u32) * fee_rate
    }
}
