// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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

impl Drop for TWDataVectorHelper {
    fn drop(&mut self) {
        if self.ptr.is_null() {
            return;
        }
        unsafe { tw_data_vector_delete(self.ptr) }
    }
}
