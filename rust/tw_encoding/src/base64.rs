// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::{EncodingError, EncodingResult};
use serde::de::Error as DeError;
use serde::{Deserialize, Deserializer, Serialize, Serializer};
use tw_memory::Data;

pub const STANDARD: Config = Config {
    url: false,
    pad: true,
};

pub const NO_PAD: Config = Config {
    url: false,
    pad: true,
};

pub const URL_SAFE: Config = Config {
    url: true,
    pad: true,
};

pub const URL_NO_PAD: Config = Config {
    url: true,
    pad: false,
};

#[derive(Clone, Copy)]
pub struct Config {
    /// Whether URL safe.
    pub url: bool,
    /// Whether '=' padded.
    pub pad: bool,
}

impl Config {
    fn as_encoding(&self) -> data_encoding::Encoding {
        match (self.url, self.pad) {
            (false, false) => data_encoding::BASE64_NOPAD,
            (false, true) => data_encoding::BASE64,
            (true, false) => data_encoding::BASE64URL_NOPAD,
            (true, true) => data_encoding::BASE64URL,
        }
    }
}

pub fn encode(data: &[u8], config: Config) -> String {
    config.as_encoding().encode(data)
}

pub fn decode(data: &str, config: Config) -> EncodingResult<Vec<u8>> {
    config
        .as_encoding()
        .decode(data.as_bytes())
        .map_err(|_| EncodingError::InvalidInput)
}

#[derive(Clone, Debug)]
pub struct Base64Encoded(pub Data);

impl Serialize for Base64Encoded {
    fn serialize<S>(&self, serializer: S) -> Result<S::Ok, S::Error>
    where
        S: Serializer,
    {
        serializer.serialize_str(&encode(&self.0, STANDARD))
    }
}

impl<'de> Deserialize<'de> for Base64Encoded {
    fn deserialize<D>(deserializer: D) -> Result<Self, D::Error>
    where
        D: Deserializer<'de>,
    {
        let s = String::deserialize(deserializer)?;
        decode(&s, STANDARD)
            .map(Base64Encoded)
            .map_err(|e| DeError::custom(format!("{e:?}")))
    }
}
