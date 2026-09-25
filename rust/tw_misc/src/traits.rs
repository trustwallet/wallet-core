// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use zeroize::Zeroizing;

pub trait ToBytesVec {
    fn to_vec(&self) -> Vec<u8>;
}

impl<T> ToBytesVec for T
where
    T: AsRef<[u8]>,
{
    fn to_vec(&self) -> Vec<u8> {
        self.as_ref().to_vec()
    }
}

pub trait ToBytesZeroizing {
    fn to_zeroizing_vec(&self) -> Zeroizing<Vec<u8>>;
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

pub trait FromSlice: for<'a> TryFrom<&'a [u8]> {}

impl<T> FromSlice for T where for<'a> T: TryFrom<&'a [u8]> {}

pub trait OptionalEmpty: Sized {
    fn empty_or_some(self) -> Option<Self>;
}

impl<T> OptionalEmpty for Vec<T> {
    fn empty_or_some(self) -> Option<Self> {
        if self.is_empty() {
            None
        } else {
            Some(self)
        }
    }
}

impl OptionalEmpty for String {
    fn empty_or_some(self) -> Option<Self> {
        if self.is_empty() {
            None
        } else {
            Some(self)
        }
    }
}

pub trait OptionalInt: Sized {
    fn zero_or_some(self) -> Option<Self>;
}

macro_rules! impl_optional_int {
    ($($int:ty,)+) => {
        $(
            impl OptionalInt for $int {
                fn zero_or_some(self) -> Option<Self> {
                    if self == 0 {
                        None
                    } else {
                        Some(self)
                    }
                }
            }
        )+
    };
}

impl_optional_int!(i8, u8, i16, u16, i32, u32, i64, u64, i128, u128,);
