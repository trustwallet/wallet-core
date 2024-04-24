// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

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
    enum NumOrStr<Num> {
        Num(Num),
        Str(String),
    }

    pub(crate) fn from_num_or_decimal_str<'de, T, Num, D>(deserializer: D) -> Result<T, D::Error>
    where
        D: Deserializer<'de>,
        T: From<Num> + FromStr<Err = NumberError>,
        Num: Deserialize<'de>,
    {
        let num_or_str: NumOrStr<Num> = NumOrStr::deserialize(deserializer)?;
        match num_or_str {
            NumOrStr::Num(num) => Ok(T::from(num)),
            NumOrStr::Str(s) => T::from_str(&s).map_err(|e| DeError::custom(format!("{e:?}"))),
        }
    }
}
