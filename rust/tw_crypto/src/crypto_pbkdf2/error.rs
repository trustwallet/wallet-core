// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

pub type Result<T> = core::result::Result<T, Error>;

#[derive(Copy, Clone, Debug, Eq, PartialEq)]
pub enum Error {
    /// Invalid output size
    InvalidOutputSize,
    /// Invalid salt size
    InvalidSaltSize,
}
