// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::ptr;
use tw_crypto::ffi::crypto_random::{crypto_random_buffer, CRandomCode};

#[test]
fn test_crypto_random_buffer() {
    let sizes = &[1_usize, 2, 16, 32, 64, 128, 200, 1000, 10_000];
    for size in sizes.iter().copied() {
        let mut buffer_0 = vec![0u8; size];
        let result = unsafe { crypto_random_buffer(buffer_0.as_mut_ptr(), size) };
        assert_eq!(result, CRandomCode::Ok as i32);

        if size < 16 {
            continue;
        }

        // Check that not all values are the same
        let first_value = buffer_0[0];
        assert!(
            buffer_0.iter().any(|&b| b != first_value),
            "Buffer contains all the same values"
        );

        // Check that two consecutive random buffers are not equal.
        let mut buffer_1 = vec![0u8; size];
        let result = unsafe { crypto_random_buffer(buffer_1.as_mut_ptr(), size) };
        assert_eq!(result, CRandomCode::Ok as i32);
        assert_ne!(
            buffer_0, buffer_1,
            "Two consecutive random buffers are equal"
        );
    }
}

#[test]
fn test_crypto_random_buffer_null_buffer() {
    // Pass data=null & size=1 intentionally with null pointer to test null buffer case.
    let result = unsafe { crypto_random_buffer(ptr::null_mut(), 1) };
    assert_eq!(result, CRandomCode::NullBuffer as i32);

    // Pass data=!null & size=0 intentionally to test null buffer case.
    let non_empty_buffer = &mut [0_u8];
    let result = unsafe { crypto_random_buffer(non_empty_buffer.as_mut_ptr(), 0) };
    assert_eq!(result, CRandomCode::NullBuffer as i32);
    assert_eq!(non_empty_buffer, &[0_u8]);
}
