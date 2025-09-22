// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::collections::HashSet;
use std::ffi::CString;
use wallet_core_rs::ffi::utils::uuid_ffi::tw_uuid_random;

/// Example of the valid UUID: 3cbbcce1-db89-4ea2-be24-88a686be461c
#[test]
fn test_tw_uuid_random_is_valid() {
    let uuid_ptr = unsafe { tw_uuid_random() };
    let uuid = unsafe { CString::from_raw(uuid_ptr) };

    let tokens: Vec<_> = uuid.to_str().unwrap().split("-").collect();
    assert_eq!(tokens.len(), 5);
    assert_eq!(tokens[0].len(), 8);
    assert_eq!(tokens[1].len(), 4);
    assert_eq!(tokens[2].len(), 4);
    assert_eq!(tokens[3].len(), 4);
    assert_eq!(tokens[4].len(), 12);
}

#[test]
fn test_tw_uuid_random_do_not_repeat() {
    const ITERATIONS: usize = 10000;

    // Use `Vec` instead of `HashSet` here to make each iteration as fast as possible.
    let mut uuids = Vec::with_capacity(ITERATIONS);
    for _ in 0..ITERATIONS {
        let uuid_ptr = unsafe { tw_uuid_random() };
        let uuid = unsafe { CString::from_raw(uuid_ptr) };
        uuids.push(uuid.to_str().unwrap().to_string());
    }

    let unique_uuids: HashSet<String> = uuids.into_iter().collect();
    assert_eq!(unique_uuids.len(), ITERATIONS);
}
