// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::ffi::privkey::{tw_private_key_create_with_data, tw_private_key_delete, TWPrivateKey};
use tw_encoding::hex;
use tw_memory::ffi::c_byte_array::CByteArray;

pub struct TWPrivateKeyHelper {
    ptr: *mut TWPrivateKey,
}

impl TWPrivateKeyHelper {
    pub fn with_bytes<T: Into<Vec<u8>>>(bytes: T) -> TWPrivateKeyHelper {
        let priv_key_raw = CByteArray::from(bytes.into());
        let ptr =
            unsafe { tw_private_key_create_with_data(priv_key_raw.data(), priv_key_raw.size()) };
        TWPrivateKeyHelper { ptr }
    }

    pub fn with_hex(hex: &str) -> TWPrivateKeyHelper {
        let priv_key_data = hex::decode(hex).unwrap();
        TWPrivateKeyHelper::with_bytes(priv_key_data)
    }

    pub fn ptr(&self) -> *mut TWPrivateKey {
        self.ptr
    }

    pub fn is_null(&self) -> bool {
        self.ptr.is_null()
    }
}

impl Drop for TWPrivateKeyHelper {
    fn drop(&mut self) {
        if self.ptr.is_null() {
            return;
        }
        unsafe { tw_private_key_delete(self.ptr) };
    }
}
