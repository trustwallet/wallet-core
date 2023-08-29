extern crate serde;

pub mod entry;
mod modules;
// TODO: `recipient` and `utils` should be deprecated entirely.
mod recipient;
#[cfg(test)]
mod tests;

// Reexports
pub use recipient::Recipient;

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
