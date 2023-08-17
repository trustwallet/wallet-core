extern crate serde;

pub mod entry;
pub mod modules;
pub mod recipient;
#[cfg(test)]
mod tests;
pub mod utils;

// Reexports
pub use recipient::Recipient;
pub use utils::*;

use tw_proto::BitcoinV2::Proto;

pub type Result<T> = std::result::Result<T, Error>;

#[derive(Debug)]
pub struct Error(Proto::Error);

impl From<Proto::Error> for Error {
    fn from(value: Proto::Error) -> Self {
        Error(value)
    }
}

impl From<Error> for Proto::Error {
    fn from(value: Error) -> Self {
        value.0
    }
}

impl From<bitcoin::key::Error> for Error {
    fn from(_: bitcoin::key::Error) -> Self {
        Error::from(Proto::Error::Error_invalid_public_key)
    }
}

impl From<bitcoin::ecdsa::Error> for Error {
    fn from(_: bitcoin::ecdsa::Error) -> Self {
        Error::from(Proto::Error::Error_invalid_ecdsa_signature)
    }
}

impl From<bitcoin::taproot::Error> for Error {
    fn from(_: bitcoin::taproot::Error) -> Self {
        Error::from(Proto::Error::Error_invalid_schnorr_signature)
    }
}

use bitcoin::key::PublicKey;
use bitcoin::script::ScriptBuf;
use bitcoin::taproot::{TapNodeHash, TaprootSpendInfo};

#[derive(Debug, Clone, Eq, PartialEq)]
pub struct TaprootScript {
    pub pubkey: PublicKey,
    pub merkle_root: TapNodeHash,
}

#[derive(Debug, Clone)]
pub struct TaprootProgram {
    pub script: ScriptBuf,
    pub spend_info: TaprootSpendInfo,
}
