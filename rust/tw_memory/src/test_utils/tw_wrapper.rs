// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

pub trait WithDestructor: Sized {
    fn destructor() -> unsafe extern "C" fn(*mut Self);
}

pub struct TWWrapper<T: WithDestructor> {
    ptr: *mut T,
}

impl<T: WithDestructor> TWWrapper<T> {
    pub fn wrap(ptr: *mut T) -> Self {
        TWWrapper { ptr }
    }

    pub fn ptr(&self) -> *mut T {
        self.ptr
    }
}

impl<T: WithDestructor> Drop for TWWrapper<T> {
    fn drop(&mut self) {
        if self.ptr.is_null() {
            return;
        }
        unsafe { (T::destructor())(self.ptr) }
    }
}
