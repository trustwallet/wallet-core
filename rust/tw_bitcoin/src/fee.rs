use bitcoin::{Transaction, Weight};

pub fn calculate_fee(tx: &Transaction, sat_vb: u64) -> (Weight, u64) {
    let weight = tx.weight();
    (weight, weight.to_vbytes_ceil() * sat_vb)
}
