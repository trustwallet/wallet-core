use super::transaction_interface::TransactionInterface;

pub trait TransactionFee: TransactionInterface {
    fn size(&self) -> usize;
    fn vsize(&self) -> usize;
    fn weight(&self) -> usize;
    fn fee(&self, fee_rate: u64) -> u64;
}

pub trait TxIndividualFee {
    fn size(&self) -> usize;
    fn vsize(&self) -> usize;
    fn weight(&self) -> usize;
}
