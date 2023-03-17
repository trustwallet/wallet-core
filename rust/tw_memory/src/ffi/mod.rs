use std::ffi::{c_char, CString};

pub mod c_byte_array;

pub use c_byte_array::CByteArray;

/// Releases the memory previously allocated for the `ptr` string.
/// \param ptr *non-null* C-compatible, nul-terminated string.
#[no_mangle]
pub unsafe extern "C" fn free_string(ptr: *const c_char) {
    // Take the ownership back to rust and drop the owner
    let _ = CString::from_raw(ptr as *mut _);
}
