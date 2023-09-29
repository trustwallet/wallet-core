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

#[derive(Debug, PartialEq)]
pub enum NumberError {
    IntegerOverflow,
    InvalidBinaryRepresentation,
    InvalidStringRepresentation,
    Overflow,
}

#[cfg(feature = "serde")]
pub(crate) mod serde_common {
    use crate::NumberError;
    use serde::de::Error as DeError;
    use serde::{Deserialize, Deserializer};
    use std::str::FromStr;

    #[derive(Deserialize)]
    #[serde(untagged)]
    enum NumOrStr<'a, Num> {
        Num(Num),
        Str(&'a str),
    }

    pub(crate) fn from_i64_or_decimal_str<'de, T, Num, D>(deserializer: D) -> Result<T, D::Error>
    where
        D: Deserializer<'de>,
        T: From<Num> + FromStr<Err = NumberError>,
        Num: Deserialize<'de>,
    {
        let num_or_str: NumOrStr<'de, Num> = NumOrStr::deserialize(deserializer)?;
        match num_or_str {
            NumOrStr::Num(num) => Ok(T::from(num)),
            NumOrStr::Str(s) => T::from_str(s).map_err(|e| DeError::custom(format!("{e:?}"))),
        }
    }
}
