// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::abi::{AbiError, AbiErrorKind, AbiResult};
use core::fmt;
use std::num::NonZeroUsize;
use std::ops::Deref;
use tw_coin_entry::error::prelude::*;

pub type NonEmptyBytes = NonEmptyArray<u8>;

/// A convenient wrapper over `NonZeroUsize`.
#[derive(Copy, Clone, PartialEq)]
pub struct NonZeroLen(NonZeroUsize);

impl NonZeroLen {
    pub fn new(len: usize) -> AbiResult<NonZeroLen> {
        NonZeroUsize::new(len)
            .or_tw_err(AbiErrorKind::Error_invalid_param_type)
            .map(NonZeroLen)
    }

    pub fn get(&self) -> usize {
        self.0.get()
    }
}

impl fmt::Debug for NonZeroLen {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{self}")
    }
}

impl fmt::Display for NonZeroLen {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.0.get())
    }
}

#[derive(Clone, Debug, PartialEq)]
pub struct NonEmptyArray<T>(Vec<T>);

impl<T> NonEmptyArray<T> {
    pub fn new(elements: Vec<T>) -> AbiResult<NonEmptyArray<T>> {
        if elements.is_empty() {
            return AbiError::err(AbiErrorKind::Error_empty_type);
        }
        Ok(NonEmptyArray(elements))
    }

    pub fn len(&self) -> NonZeroLen {
        NonZeroLen::new(self.0.len()).expect("`NonEmptyArray` must have at least one element")
    }

    pub fn into_vec(self) -> Vec<T> {
        self.0
    }
}

impl<T> IntoIterator for NonEmptyArray<T> {
    type Item = T;
    type IntoIter = std::vec::IntoIter<T>;

    fn into_iter(self) -> Self::IntoIter {
        self.0.into_iter()
    }
}

impl<T> Deref for NonEmptyArray<T> {
    type Target = [T];

    fn deref(&self) -> &Self::Target {
        &self.0
    }
}

impl AsRef<[u8]> for NonEmptyArray<u8> {
    fn as_ref(&self) -> &[u8] {
        &self.0
    }
}
