extern crate serde;

pub mod brc20;
pub mod entry;
pub mod ffi;
pub mod modules;
pub mod nft;
pub mod ordinals;
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
