use crate::ffi::tw_bitcoin_calculate_transaction_fee;
use tw_memory::ffi::c_result::CUInt64Result;

/// Convenience wrapper.
fn call_ffi_calculate_fee(hex: &str, sat_vb: u64) -> u64 {
    let hex = tw_encoding::hex::decode(hex).unwrap();

    let res: CUInt64Result =
        unsafe { tw_bitcoin_calculate_transaction_fee(hex.as_ptr(), hex.len(), sat_vb) };

    res.unwrap()
}

#[test]
fn ffi_calculate_p2pkh_fee() {
    let fee = call_ffi_calculate_fee(crate::tests::p2pkh::TX_RAW, 10);
    assert_eq!(fee, 191 * 10);
}

#[test]
fn ffi_calculate_p2wpkh_fee() {
    let fee = call_ffi_calculate_fee(crate::tests::p2wpkh::TX_RAW, 10);
    assert_eq!(fee, 189 * 10);
}

#[test]
fn ffi_calculate_brc20_commit_fee() {
    let fee = call_ffi_calculate_fee(crate::tests::brc20_transfer::COMMIT_TX_RAW, 19);
    assert_eq!(fee, 153 * 19);
}

#[test]
fn ffi_calculate_brc20_reveal_fee() {
    let fee = call_ffi_calculate_fee(crate::tests::brc20_transfer::REVEAL_TX_RAW, 49);
    assert_eq!(fee, 131 * 49);
}

#[test]
fn ffi_calculate_ordinal_nft_commit_fee() {
    let fee = call_ffi_calculate_fee(crate::tests::nft::COMMIT_RAW_TX, 10);
    assert_eq!(fee, 122 * 10);
}

#[test]
fn ffi_calculate_ordinal_nft_reveal_fee() {
    let fee = call_ffi_calculate_fee(crate::tests::nft::REVEAL_RAW_TX, 15);
    assert_eq!(fee, 2019 * 15);
}
