extern crate serde;

pub mod brc20;
pub mod claim;
pub mod ffi;
pub mod input;
pub mod nft;
pub mod ordinals;
pub mod output;
pub mod recipient;
#[cfg(test)]
mod tests;
pub mod transaction;
pub mod utils;

// Reexports
pub use input::*;
pub use output::*;
pub use recipient::Recipient;
pub use transaction::*;
pub use utils::*;

pub type Result<T> = std::result::Result<T, Error>;

#[derive(Debug, Clone)]
pub enum Error {
    Todo,
}
