// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::error::prelude::*;

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
        output.error = *TWError::error_type(&err);
        output.error_message = std::borrow::Cow::from(err.to_string());

        output
    }};
}

pub type AbiErrorKind = tw_proto::EthereumAbi::Proto::AbiError;
pub type AbiError = TWError<AbiErrorKind>;
pub type AbiResult<T> = Result<T, AbiError>;

pub fn abi_to_signing_error(abi: AbiError) -> SigningError {
    abi.map_err(|abi_kind| match abi_kind {
        AbiErrorKind::OK => SigningErrorType::OK,
        AbiErrorKind::Error_internal => SigningErrorType::Error_internal,
        _ => SigningErrorType::Error_invalid_params,
    })
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_abi_to_signing_error() {
        // Test OK case
        let abi_ok = AbiError::new(AbiErrorKind::OK);
        let signing_ok = abi_to_signing_error(abi_ok);
        assert_eq!(*signing_ok.error_type(), SigningErrorType::OK);

        // Test internal error case
        let abi_internal = AbiError::new(AbiErrorKind::Error_internal);
        let signing_internal = abi_to_signing_error(abi_internal);
        assert_eq!(
            *signing_internal.error_type(),
            SigningErrorType::Error_internal
        );

        // Test other error cases map to invalid params
        let abi_other = AbiError::new(AbiErrorKind::Error_invalid_abi);
        let signing_other = abi_to_signing_error(abi_other);
        assert_eq!(
            *signing_other.error_type(),
            SigningErrorType::Error_invalid_params
        );
    }
}
