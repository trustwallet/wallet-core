use tw_coin_entry::error::prelude::*;
use tw_ss58_address::NetworkId;

pub mod entry;
pub use entry::*;

pub mod substrate_coin_entry;
pub use substrate_coin_entry::*;

pub mod address;
pub use address::*;

pub mod extrinsic;
pub use extrinsic::*;

pub mod extensions;
pub use extensions::*;

#[derive(Debug, Clone, Eq, PartialEq)]
pub enum EncodeError {
    InvalidNetworkId,
    MissingCallIndices,
    InvalidCallIndex,
    InvalidAddress,
    InvalidValue,
    NotSupported,
}

impl EncodeError {
    /// Returns a TWError with the given error and context.
    pub fn with_context(self, context: String) -> TWError<Self> {
        TWError::new(self).context(context)
    }

    /// Returns a TWResult with the given error and context.
    pub fn tw_result<T>(self, context: String) -> TWResult<T, Self> {
        Err(self.with_context(context))
    }

    /// Convert another error type to this error type and return a TWError.
    pub fn with_error<T: std::fmt::Debug>(self, err: T) -> TWError<Self> {
        TWError::new(self).context(format!("{err:?}"))
    }
}

impl From<EncodeError> for SigningErrorType {
    #[inline]
    fn from(err: EncodeError) -> Self {
        match err {
            EncodeError::InvalidAddress => SigningErrorType::Error_invalid_address,
            EncodeError::InvalidValue => SigningErrorType::Error_input_parse,
            EncodeError::MissingCallIndices => SigningErrorType::Error_not_supported,
            EncodeError::NotSupported => SigningErrorType::Error_not_supported,
            _ => SigningErrorType::Error_invalid_params,
        }
    }
}

impl From<EncodeError> for SigningError {
    #[inline]
    fn from(err: EncodeError) -> Self {
        TWError::new::<SigningErrorType>(err.clone().into()).context(format!("{err:?}"))
    }
}

pub type EncodeResult<T> = Result<T, TWError<EncodeError>>;
pub type WithCallIndexResult<T> = Result<WithCallIndex<T>, TWError<EncodeError>>;

#[derive(Debug, Clone)]
pub struct SubstrateContext {
    pub multi_address: bool,
    pub network: NetworkId,
    pub spec_version: u32,
    pub transaction_version: u32,
    pub fee_asset_id: Option<u32>,
}

impl SubstrateContext {
    pub fn multi_address(&self, account: AccountId) -> MultiAddress {
        MultiAddress::new(account, self.multi_address)
    }

    pub fn multi_addresses(&self, accounts: Vec<AccountId>) -> Vec<MultiAddress> {
        accounts
            .into_iter()
            .map(|account| MultiAddress::new(account, self.multi_address))
            .collect()
    }
}
