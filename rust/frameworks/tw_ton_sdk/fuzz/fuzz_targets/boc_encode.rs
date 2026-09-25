#![no_main]

use libfuzzer_sys::fuzz_target;
use tw_ton_sdk::boc::BagOfCells;

fuzz_target!(|data: &[u8]| {
    let _ = BagOfCells::parse(data);
});
