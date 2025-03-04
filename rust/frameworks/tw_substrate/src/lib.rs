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

pub mod modules;

#[derive(Debug, Clone, Eq, PartialEq)]
pub enum EncodeError {
    InvalidNetworkId,
    MissingCallIndices,
    InvalidCallIndex,
    InvalidAddress,
    InvalidValue,
    NotSupported,
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

pub type EncodeResult<T> = Result<T, TWError<EncodeError>>;
pub type WithCallIndexResult<T> = Result<WithCallIndex<T>, TWError<EncodeError>>;

#[derive(Debug, Clone)]
pub struct SubstrateContext {
    pub multi_address: bool,
    pub check_metadata: bool,
    pub network: NetworkId,
    pub spec_version: u32,
    pub transaction_version: u32,
    pub fee_asset_id: Option<u32>,
}

impl SubstrateContext {
    pub fn multi_address(&self, account: AccountId) -> MultiAddress {
        MultiAddress::new(account, self.multi_address)
    }
}
