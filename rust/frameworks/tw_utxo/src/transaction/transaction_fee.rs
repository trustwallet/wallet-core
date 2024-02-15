pub trait TransactionFee {
    type Input: TxIndividualFee;
    type Output: TxIndividualFee;

    fn inputs(&self) -> &[Self::Input];
    fn outputs(&self) -> &[Self::Output];

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
