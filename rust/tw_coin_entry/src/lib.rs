mod coin_context;
mod coin_entry;
mod derivation;
mod error;
mod prefix;

pub use coin_context::CoinContext;
pub use coin_entry::{CoinEntry, ProtoError, ProtoResult};
pub use derivation::Derivation;
pub use error::AddressError;
pub use prefix::{NoPrefix, Prefix};
