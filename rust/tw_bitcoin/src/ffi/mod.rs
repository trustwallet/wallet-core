#![allow(clippy::missing_safety_doc)]
use tw_memory::ffi::c_result::ErrorCode;

pub mod address;
pub mod p2pkh;
pub mod p2tr_key_path;
pub mod p2wpkh;
pub mod transaction;

#[repr(C)]
pub enum CTaprootError {
    Ok = 0,
    InvalidSlice = 1,
    InvalidPubkey = 2,
}

impl From<CTaprootError> for ErrorCode {
    fn from(error: CTaprootError) -> Self {
        error as ErrorCode
    }
}

// The code below is reimplemented from the `dev` branch.

#[macro_export]
macro_rules! try_or_else {
    ($exp:expr, $or_else:expr) => {{
        match $crate::ffi::IntoOption::into_option($exp) {
            Some(res) => res,
            None => return $or_else(),
        }
    }};
}

pub trait IntoOption<T> {
    fn into_option(self) -> Option<T>;
}

impl<T, E> IntoOption<T> for Result<T, E> {
    fn into_option(self) -> Option<T> {
        self.ok()
    }
}

impl<T> IntoOption<T> for Option<T> {
    fn into_option(self) -> Option<T> {
        self
    }
}

pub trait RawPtrTrait: Sized {
    fn into_ptr(self) -> *mut Self {
        Box::into_raw(Box::new(self))
    }

    unsafe fn from_ptr(raw: *mut Self) -> Option<Self> {
        if raw.is_null() {
            return None;
        }
        Some(*Box::from_raw(raw))
    }

    unsafe fn from_ptr_as_ref(raw: *mut Self) -> Option<&'static Self> {
        if raw.is_null() {
            return None;
        }
        Some(&*raw)
    }
}
