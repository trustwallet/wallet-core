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
        output.error = err.0;
        output.error_message = std::borrow::Cow::from(err.to_string());

        output
    }};
}
