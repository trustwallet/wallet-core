use super::Transaction;
use crate::transaction::{transaction_fee::TransactionFee, transaction_parts::Amount};
use tw_coin_entry::error::prelude::*;

impl TransactionFee for Transaction {
    fn vsize(&self) -> usize {
        self.vsize()
    }

    fn fee(&self, fee_rate: Amount) -> SigningResult<Amount> {
        let vsize = self.vsize();
        Amount::try_from(vsize)
            .ok()
            .and_then(|vsize| vsize.checked_mul(fee_rate))
            .or_tw_err(SigningErrorType::Error_wrong_fee)
            .with_context(|| format!("feePerVByte is too large: '{vsize} * {fee_rate}' overflow"))
    }
}
