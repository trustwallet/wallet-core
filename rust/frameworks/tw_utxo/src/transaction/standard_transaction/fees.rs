use super::{Transaction, TransactionInput, TransactionOutput};
use crate::{
    encode::compact_integer::CompactInteger,
    transaction::transaction_fee::{TransactionFee, TxIndividualFee},
};

const VERSION_SIZE: usize = 4;
const LOCKTIME_SIZE: usize = 4;
const OUT_POINT_SIZE: usize = 36;
const SEQUENCE_SIZE: usize = 4;
const VALUE_SIZE: usize = 8;

const SEGWIT_SCALE_FACTOR: usize = 4;

impl TransactionFee for Transaction {
    type Input = TransactionInput;
    type Output = TransactionOutput;

    fn inputs(&self) -> &[Self::Input] {
        &self.inputs
    }

    fn outputs(&self) -> &[Self::Output] {
        &self.outputs
    }

    fn size(&self) -> usize {
        let mut s = 0;
        // Base transaction weight.
        s += VERSION_SIZE;
        s += LOCKTIME_SIZE;
        s += CompactInteger::from(self.inputs().len()).serialized_len();
        s += CompactInteger::from(self.outputs().len()).serialized_len();
        self.inputs().iter().for_each(|input| s += input.size());
        self.outputs().iter().for_each(|output| s += output.size());
        s
    }

    fn vsize(&self) -> usize {
        (self.weight() + 3) / SEGWIT_SCALE_FACTOR // ceil(weight / 4)
    }

    fn weight(&self) -> usize {
        let mut w = 0;
        // Base transaction weight (non-segwit)
        w += VERSION_SIZE;
        w += LOCKTIME_SIZE;
        w += CompactInteger::from(self.inputs().len()).serialized_len();
        w += CompactInteger::from(self.outputs().len()).serialized_len();

        // Apply scale factor.
        w *= SEGWIT_SCALE_FACTOR;

        // Calculate the weight of each input and output. The Segwit scale
        // factor is already considered by the weight methods.
        self.inputs().iter().for_each(|input| w += input.weight());
        self.outputs()
            .iter()
            .for_each(|output| w += output.weight());

        w
    }

    fn fee(&self, fee_rate: u64) -> u64 {
        self.vsize() as u64 * fee_rate
    }
}

impl TxIndividualFee for TransactionInput {
    fn size(&self) -> usize {
        OUT_POINT_SIZE
            + self.script_sig.serialized_len()
            + SEQUENCE_SIZE
            + self.witness.serialized_len()
    }

    fn vsize(&self) -> usize {
        (self.weight() + 3) / SEGWIT_SCALE_FACTOR // ceil(weight / 4)
    }

    fn weight(&self) -> usize {
        let non_witness = OUT_POINT_SIZE
            + self.script_sig.serialized_len()
            + SEQUENCE_SIZE
            + self.witness.serialized_len();

        // Witness data has no scale factor applied, ie. it's discounted.
        non_witness * SEGWIT_SCALE_FACTOR + self.witness.serialized_len()
    }
}

impl TxIndividualFee for TransactionOutput {
    fn size(&self) -> usize {
        VALUE_SIZE + self.script_pubkey.serialized_len()
    }

    fn vsize(&self) -> usize {
        (self.weight() + 3) / SEGWIT_SCALE_FACTOR // ceil(weight / 4)
    }

    fn weight(&self) -> usize {
        self.size() * SEGWIT_SCALE_FACTOR
    }
}
