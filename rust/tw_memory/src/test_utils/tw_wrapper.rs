// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
