// This seems to be required, even if the tests in `tests/` actually use
// functions/constants.
#![allow(dead_code)]

pub mod data;

pub const ONE_BTC: u64 = 100_000_000;
pub const MINER_FEE: u64 = 1_000_000;

pub fn hex(string: &str) -> Vec<u8> {
    tw_encoding::hex::decode(string).unwrap()
}
