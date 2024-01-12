// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::error::{SigningError, SigningErrorType};

pub mod contract;
pub mod decode;
pub mod encode;
pub mod function;
pub mod non_empty_array;
pub mod param;
pub mod param_token;
pub mod param_type;
pub mod prebuild;
pub mod signature;
pub mod token;
pub mod uint;

#[macro_export]
macro_rules! abi_output_error {
    ($output:ty, $error:expr) => {{
        let err = $error;

        let mut output = <$output>::default();
        output.error = err.0;
        output.error_message = std::borrow::Cow::from(format!("{err:?}"));

        output
    }};
}

pub type AbiResult<T> = Result<T, AbiError>;
pub type AbiErrorKind = tw_proto::EthereumAbi::Proto::AbiError;

#[derive(Debug)]
pub struct AbiError(pub AbiErrorKind);

impl From<AbiError> for SigningError {
    fn from(err: AbiError) -> Self {
        match err.0 {
            AbiErrorKind::OK => SigningError(SigningErrorType::OK),
            AbiErrorKind::Error_internal => SigningError(SigningErrorType::Error_internal),
            _ => SigningError(SigningErrorType::Error_invalid_params),
        }
    }
}
