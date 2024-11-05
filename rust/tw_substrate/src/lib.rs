use tw_coin_entry::error::prelude::*;

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

impl From<EncodeError> for SigningError {
    #[inline]
    fn from(_err: EncodeError) -> Self {
        TWError::new(SigningErrorType::Error_invalid_params)
    }
}

pub type EncodeResult<T> = Result<T, EncodeError>;
pub type WithCallIndexResult<T> = Result<WithCallIndex<T>, EncodeError>;
