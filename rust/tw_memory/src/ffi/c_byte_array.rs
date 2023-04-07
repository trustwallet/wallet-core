// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

pub type CByteArrayPtr = *mut CByteArray;

#[repr(C)]
pub struct CByteArrayResult {
    pub code: i32,
    pub result: CByteArrayPtr,
}

crate::impl_c_result!(CByteArrayResult, CByteArrayPtr, std::ptr::null_mut());

/// C-compatible byte array.
#[repr(C)]
pub struct CByteArray {
    data: *mut u8,
    size: usize,
    capacity: usize,
}

impl Drop for CByteArray {
    fn drop(&mut self) {
        // Do nothing if the memory has been released already.
        // It could be released on `CByteArray::into_vec`.
        if self.data.is_null() {
            return;
        }

        let _ = unsafe { Vec::from_raw_parts(self.data, self.size, self.capacity) };
    }
}

impl From<Vec<u8>> for CByteArray {
    fn from(data: Vec<u8>) -> Self {
        CByteArray::new(data)
    }
}

impl CByteArray {
    /// Returns a `CByteArray` instance from the given `mut_vec` bytes.
    pub fn new(mut mut_vec: Vec<u8>) -> CByteArray {
        let data = mut_vec.as_mut_ptr();
        let size = mut_vec.len();
        let capacity = mut_vec.capacity();
        std::mem::forget(mut_vec);
        CByteArray {
            data,
            size,
            capacity,
        }
    }

    /// Returns the pointer to a `CByteArray` instance allocated on heap.
    pub fn new_ptr(data: Vec<u8>) -> CByteArrayPtr {
        Box::into_raw(Box::new(CByteArray::from(data)))
    }

    /// Converts `CByteArray` into `Vec<u8>` without additional allocation.
    ///
    /// # Safe
    ///
    /// `data`, `size` and `capacity` must be valid and exactly the same as after [`CByteArray::new`].
    pub unsafe fn into_vec(mut self) -> Vec<u8> {
        let data = Vec::from_raw_parts(self.data, self.size, self.capacity);
        // Set the self to null to avoid realising the memory.
        self.set_null();
        data
    }

    /// Creates a `CByteArray` from the `ptr` pointer.
    ///
    /// # Safe
    ///
    /// The `ptr` pointer must point to a memory previously allocated on heap in Rust *only*.
    pub unsafe fn from_ptr(ptr: CByteArrayPtr) -> CByteArray {
        *Box::from_raw(ptr)
    }

    /// Set the pointer, size and capacity to the default values.
    fn set_null(&mut self) {
        self.data = std::ptr::null_mut();
        self.size = 0;
        self.capacity = 0;
    }
}

/// Releases the memory previously allocated for the pointer to `CByteArray`.
/// \param ptr *non-null* C-compatible byte array.
#[no_mangle]
pub unsafe extern "C" fn free_c_byte_array(ptr: CByteArrayPtr) {
    if ptr.is_null() {
        return;
    }
    // Take the ownership back to rust and drop the owner.
    let _ = CByteArray::from_ptr(ptr);
}
