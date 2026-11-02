// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_memory::ffi::tw_data::{tw_data_create_with_bytes, tw_data_delete, TWData};

#[test]
fn test_tw_data_delete_via_ffi() {
    let bytes = b"TW_DATA_SENSITIVE_PLAINTEXT";
    let ptr = unsafe { tw_data_create_with_bytes(bytes.as_ptr(), bytes.len()) };
    unsafe { tw_data_delete(ptr) };
}

#[test]
fn test_tw_data_rust_drop_does_not_panic() {
    drop(TWData::from(b"TW_DATA_SENSITIVE_PLAINTEXT".to_vec()));
}
