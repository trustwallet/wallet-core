extern crate serde;

pub mod brc20;
pub mod entry;
pub mod ffi;
pub mod nft;
pub mod ordinals;
pub mod recipient;
#[cfg(test)]
mod tests;
pub mod utils;

// Reexports
pub use recipient::Recipient;
pub use utils::*;

pub type Result<T> = std::result::Result<T, Error>;

#[derive(Debug, Clone)]
pub enum Error {
    Todo,
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