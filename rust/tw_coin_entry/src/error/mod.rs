// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

mod address_error;
mod impl_from;
mod tw_error;
mod tw_result;

pub mod prelude {
    pub use super::address_error::*;
    pub use super::tw_error::*;
    pub use super::tw_result::*;

    pub type SigningErrorType = tw_proto::Common::Proto::SigningError;
    pub type SigningError = TWError<SigningErrorType>;
    pub type SigningResult<T> = Result<T, SigningError>;
}

#[macro_export]
macro_rules! signing_output_error {
    ($output:ty, $error:expr) => {{
        let err = $error;

        let mut output = <$output>::default();
        output.error = *TWError::error_type(&err);
        output.error_message = std::borrow::Cow::from(err.to_string());

        output
    }};
}

#[cfg(test)]
mod tests {
    use super::prelude::*;

    fn function_signing_error_type() -> Result<(), SigningErrorType> {
        Err(SigningErrorType::Error_internal)
    }

    fn function_address_error() -> AddressResult<()> {
        Err(AddressError::Internal)
    }

    /// Test `AddressError` -> `TWError<SigningErrorType>` conversion via [`Result::context`].
    #[test]
    fn test_error_convert_via_context() {
        let err: SigningError = function_address_error()
            .into_tw()
            .context("!test_error_convert_via_context")
            .unwrap_err();

        let expected = r#"A provided address (e.g. destination address) is invalid
Context:
0. !test_error_convert_via_context"#;
        assert_eq!(err.to_string(), expected);
    }

    /// Test `AddressError` -> `TWError<SigningErrorType>` conversion via [`Result::into_tw`].
    #[test]
    fn test_error_convert_via_into_tw() {
        let err: SigningError = function_signing_error_type().into_tw().unwrap_err();

        let expected = r#"Internal error"#;
        assert_eq!(err.to_string(), expected);
    }

    /// Test error chaining.
    #[test]
    fn test_error_chaining() {
        let res: SigningResult<()> = SigningError::err(SigningErrorType::Error_internal)
            .context("First context")
            .context("Second context");

        let expected = r#"Internal error
Context:
0. First context
1. Second context"#;
        assert_eq!(res.unwrap_err().to_string(), expected);
    }
}
