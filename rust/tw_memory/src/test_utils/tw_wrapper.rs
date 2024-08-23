// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

pub type Destructor<T> = unsafe extern "C" fn(*mut T);

pub trait WithDestructor: Sized {
    fn destructor() -> unsafe extern "C" fn(*mut Self);
}

/// Structure pointer wrapper.
/// Use this wrapper when `T` can implement the `WithDestructor` trait.
/// Otherwise, consider using `TWWrapper`.
pub struct TWAutoWrapper<T: WithDestructor> {
    ptr: *mut T,
}

impl<T: WithDestructor> TWAutoWrapper<T> {
    pub fn wrap(ptr: *mut T) -> Self {
        TWAutoWrapper { ptr }
    }

    pub fn ptr(&self) -> *mut T {
        self.ptr
    }
}

impl<T: WithDestructor> Drop for TWAutoWrapper<T> {
    fn drop(&mut self) {
        if self.ptr.is_null() {
            return;
        }
        unsafe { (T::destructor())(self.ptr) }
    }
}

/// Structure pointer wrapper with a manual destructor.
pub struct TWWrapper<T> {
    ptr: *mut T,
    destructor: Destructor<T>,
}

impl<T> TWWrapper<T> {
    pub fn wrap(ptr: *mut T, destructor: Destructor<T>) -> Self {
        TWWrapper { ptr, destructor }
    }

    pub fn ptr(&self) -> *mut T {
        self.ptr
    }
}

impl<T> Drop for TWWrapper<T> {
    fn drop(&mut self) {
        if self.ptr.is_null() {
            return;
        }
        unsafe { (self.destructor)(self.ptr) }
    }
}
