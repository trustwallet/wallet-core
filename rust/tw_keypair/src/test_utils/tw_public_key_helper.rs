// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::ffi::pubkey::{tw_public_key_create_with_data, tw_public_key_delete, TWPublicKey};
use crate::tw::PublicKeyType;
use tw_encoding::hex;
use tw_memory::ffi::c_byte_array::CByteArray;
use tw_memory::Data;

pub struct TWPublicKeyHelper {
    ptr: *mut TWPublicKey,
}

impl TWPublicKeyHelper {
    pub fn with_bytes<T: Into<Data>>(bytes: T, ty: PublicKeyType) -> TWPublicKeyHelper {
        let public_key_raw = CByteArray::from(bytes.into());
        let ptr = unsafe {
            tw_public_key_create_with_data(public_key_raw.data(), public_key_raw.size(), ty as u32)
        };
        TWPublicKeyHelper { ptr }
    }

    pub fn with_hex(hex: &str, ty: PublicKeyType) -> TWPublicKeyHelper {
        let priv_key_data = hex::decode(hex).unwrap();
        TWPublicKeyHelper::with_bytes(priv_key_data, ty)
    }

    pub fn wrap(ptr: *mut TWPublicKey) -> TWPublicKeyHelper {
        TWPublicKeyHelper { ptr }
    }

    pub fn ptr(&self) -> *mut TWPublicKey {
        self.ptr
    }

    pub fn is_null(&self) -> bool {
        self.ptr.is_null()
    }
}

impl Drop for TWPublicKeyHelper {
    fn drop(&mut self) {
        if self.ptr.is_null() {
            return;
        }
        unsafe { tw_public_key_delete(self.ptr) };
    }
}
