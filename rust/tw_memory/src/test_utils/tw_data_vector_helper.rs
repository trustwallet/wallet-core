// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::ffi::tw_data_vector::{
    tw_data_vector_add, tw_data_vector_create, tw_data_vector_delete, TWDataVector,
};
use crate::test_utils::tw_data_helper::TWDataHelper;

type Data = Vec<u8>;

pub struct TWDataVectorHelper {
    ptr: *mut TWDataVector,
}

impl TWDataVectorHelper {
    pub fn create<I>(vec: I) -> TWDataVectorHelper
    where
        I: IntoIterator<Item = Data>,
    {
        let ptr = unsafe { tw_data_vector_create() };
        for data in vec {
            let data = TWDataHelper::create(data);
            unsafe { tw_data_vector_add(ptr, data.ptr()) };
        }

        TWDataVectorHelper { ptr }
    }

    pub fn ptr(&self) -> *mut TWDataVector {
        self.ptr
    }
}

impl Default for TWDataVectorHelper {
    fn default() -> Self {
        TWDataVectorHelper::create([])
    }
}

impl Drop for TWDataVectorHelper {
    fn drop(&mut self) {
        if self.ptr.is_null() {
            return;
        }
        unsafe { tw_data_vector_delete(self.ptr) }
    }
}
