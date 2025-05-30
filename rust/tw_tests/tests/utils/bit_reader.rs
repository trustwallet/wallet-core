// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_encoding::hex::{DecodeHex, ToHex};
use tw_memory::test_utils::tw_data_helper::TWDataHelper;
use wallet_core_rs::ffi::utils::bit_reader_ffi::{
    tw_bit_reader_create, tw_bit_reader_delete, tw_bit_reader_finished, tw_bit_reader_read_u8,
    tw_bit_reader_read_u8_slice, CBitReaderCode,
};

#[test]
fn test_tw_bit_reader_success() {
    let ton_address_cell = "8005bd3e6bab0c5c6ca7c84ea9e4c0bfa0a38662f5bba544702d2ede1c8e315d2ba0"
        .decode_hex()
        .unwrap();
    let ton_address_cell = TWDataHelper::create(ton_address_cell);

    let reader = unsafe { tw_bit_reader_create(ton_address_cell.ptr(), 267) };
    assert!(!reader.is_null());

    let tp = unsafe { tw_bit_reader_read_u8(reader, 2) };
    assert_eq!(tp.into_result(), Ok(2));

    let res1 = unsafe { tw_bit_reader_read_u8(reader, 1) };
    assert_eq!(res1.into_result(), Ok(0));

    let wc = unsafe { tw_bit_reader_read_u8(reader, 8) };
    assert_eq!(wc.into_result(), Ok(0));

    assert!(!unsafe { tw_bit_reader_finished(reader) });

    let hash_part = unsafe { tw_bit_reader_read_u8_slice(reader, 32).unwrap().into_vec() };
    assert_eq!(
        hash_part.to_hex(),
        "2de9f35d5862e3653e42754f2605fd051c3317addd2a23816976f0e4718ae95d"
    );

    assert!(unsafe { tw_bit_reader_finished(reader) });

    unsafe { tw_bit_reader_delete(reader) };
}

#[test]
fn test_tw_bit_reader_error() {
    let bytes_len = 2;
    // Less than two bytes.
    let bits_len = 15;

    let data = TWDataHelper::create(vec![1; bytes_len]);

    let reader = unsafe { tw_bit_reader_create(data.ptr(), bits_len as u64) };
    assert!(!reader.is_null());

    // Cannot read u8 from 9 bits.
    let res = unsafe { tw_bit_reader_read_u8(reader, 9) };
    assert_eq!(
        res.into_result().unwrap_err(),
        CBitReaderCode::TooManyBitsForType as i32
    );

    // Read a dummy u8.
    let _ = unsafe { tw_bit_reader_read_u8_slice(reader, 8) };

    // Cannot read 8 bits as there are 7 bits left only.
    let res = unsafe { tw_bit_reader_read_u8_slice(reader, 8) };
    assert_eq!(
        res.into_result().unwrap_err(),
        CBitReaderCode::NotEnoughData as i32
    );

    unsafe { tw_bit_reader_delete(reader) };
}
