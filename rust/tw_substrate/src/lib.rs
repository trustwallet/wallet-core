pub mod address;
pub use address::*;

pub mod extrinsic;
pub use extrinsic::*;

#[derive(Debug, Clone, Eq, PartialEq)]
pub enum EncodeError {
    InvalidNetworkId,
    MissingCallIndicesTable,
    InvalidCallIndex,
    InvalidAddress,
    InvalidValue,
}

pub type EncodeResult<T> = Result<T, EncodeError>;
pub type WithCallIndexResult<T> = Result<WithCallIndex<T>, EncodeError>;
