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
///
/// # Freeing memory
///
/// Since the structure fields are public, it's highly unsafe to free the memory on [`Drop::drop`].
/// In order to free the memory, consider using [`CByteArray::into_vec`] or [`CByteArray::free`]
/// unsafe methods.
#[repr(C)]
pub struct CByteArray {
    pub data: *mut u8,
    pub size: usize,
    pub capacity: usize,
}

impl From<Vec<u8>> for CByteArray {
    fn from(data: Vec<u8>) -> Self {
        CByteArray::new(data)
    }
}

impl CByteArray {
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

    pub fn new_ptr(data: Vec<u8>) -> CByteArrayPtr {
        Box::into_raw(Box::new(CByteArray::from(data)))
    }

    pub unsafe fn into_vec(self) -> Vec<u8> {
        Vec::from_raw_parts(self.data, self.size, self.capacity)
    }

    pub unsafe fn from_ptr(ptr: CByteArrayPtr) -> CByteArray {
        *Box::from_raw(ptr)
    }

    pub unsafe fn free(self) {
        let _ = Vec::from_raw_parts(self.data, self.size, self.capacity);
    }
}

/// Releases the memory previously allocated for the pointer to `CByteArray`.
/// \param ptr *non-null* C-compatible byte array.
#[no_mangle]
pub unsafe extern "C" fn free_c_byte_array(ptr: CByteArrayPtr) {
    // Take the ownership back to rust and drop the owner.
    if ptr.is_null() {
        return;
    }
    Box::from_raw(ptr).free();
}
