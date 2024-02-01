// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::ffi::tw_data::TWData;
use crate::ffi::RawPtrTrait;

type Data = Vec<u8>;

/// A vector of `TWData` byte arrays.
#[derive(Default)]
pub struct TWDataVector(Vec<TWData>);

impl TWDataVector {
    /// Returns an empty `TWDataVector` instance.
    pub fn new() -> TWDataVector {
        TWDataVector(Vec::new())
    }

    /// Adds an element to a vector of `TWData`.
    pub fn push(&mut self, data: TWData) {
        self.0.push(data);
    }

    /// Retrieves the number of elements.
    pub fn size(&self) -> usize {
        self.0.len()
    }

    /// Converts `Vec<TWData>` to `Vec<Data>` by cloning each element.
    pub fn to_data_vec(&self) -> Vec<Data> {
        self.0.iter().map(TWData::to_vec).collect()
    }
}

impl RawPtrTrait for TWDataVector {}

/// Creates a Vector of Data.
///
/// \note Must be deleted with \TWDataVectorDelete
/// \return a non-null Vector of Data.
#[no_mangle]
pub unsafe extern "C" fn tw_data_vector_create() -> *mut TWDataVector {
    TWDataVector::new().into_ptr()
}

/// Delete/Deallocate a Vector of Data
///
/// \param data_vector A non-null Vector of data
#[no_mangle]
pub unsafe extern "C" fn tw_data_vector_delete(data_vector: *mut TWDataVector) {
    // Take the ownership back to rust and drop the owner.
    let _ = TWDataVector::from_ptr(data_vector);
}

/// Add an element to a Vector of Data. Element is cloned
///
/// \param data_vector A non-null Vector of data
/// \param data A non-null valid block of data
/// \note data input parameter must be deleted on its own
#[no_mangle]
pub unsafe extern "C" fn tw_data_vector_add(data_vector: *mut TWDataVector, data: *const TWData) {
    let Some(data_vector) = TWDataVector::from_ptr_as_mut(data_vector) else {
        return;
    };

    let Some(data_ref) = TWData::from_ptr_as_ref(data) else {
        return;
    };

    data_vector.push(data_ref.clone());
}
