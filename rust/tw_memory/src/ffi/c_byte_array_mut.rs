// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

/// A C-compatible wrapper over a mutable byte array pointer given as an FFI argument.
#[repr(C)]
pub struct CByteArrayMut {
    data: *mut u8,
    size: usize,
}

impl CByteArrayMut {
    /// Creates a new `CByteArrayMut` from the allocated byte array.
    pub fn new(data: *mut u8, size: usize) -> CByteArrayMut {
        CByteArrayMut { data, size }
    }

    /// Returns a mutable slice if `data` is non-null and `size` is non-zero.
    ///
    /// # Safety
    ///
    /// The inner data must be valid.
    pub unsafe fn as_mut(&mut self) -> Option<&mut [u8]> {
        if self.data.is_null() || self.size == 0 {
            return None;
        }
        Some(std::slice::from_raw_parts_mut(self.data, self.size))
    }
}
