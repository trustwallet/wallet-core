// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

mod i256;
mod sign;
mod u256;

pub use i256::I256;
pub use sign::Sign;
pub use u256::U256;

pub type NumberResult<T> = Result<T, NumberError>;

#[derive(Debug)]
pub enum NumberError {
    InvalidBinaryRepresentation,
    InvalidStringRepresentation,
    Overflow,
}
