// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

/// A C-compatible wrapper over a byte array given as the FFI argument.
#[repr(C)]
#[derive(Debug)]
pub struct CByteArrayRef {
    data: *const u8,
    size: usize,
}

impl CByteArrayRef {
    /// Creates a new `CByteArrayRef` from the allocated byte array.
    pub fn new(data: *const u8, size: usize) -> CByteArrayRef {
        CByteArrayRef { data, size }
    }

    /// Clones the byte array as a `Vec<u8>`.
    /// Returns `None` if `CByteArrayRef::data` is null.
    ///
    /// # Safety
    ///
    /// The inner data must be valid.
    pub unsafe fn to_vec(&self) -> Option<Vec<u8>> {
        self.as_slice().map(|data| data.to_vec())
    }

    /// Returns a slice.
    /// Returns `None` if `CByteArrayRef::data` is null.
    ///
    /// # Safety
    ///
    /// The inner data must be valid.
    pub unsafe fn as_slice(&self) -> Option<&'static [u8]> {
        if self.data.is_null() {
            return None;
        }
        Some(std::slice::from_raw_parts(self.data, self.size))
    }
}
