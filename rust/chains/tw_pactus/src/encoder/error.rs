use tw_coin_entry::error::prelude::{SigningError, SigningErrorType};


/// Errors encountered when encoding or decoding data.
#[derive(Debug)]
pub enum Error {
    IoError(std::io::Error),
    ParseFailed(&'static str),
}

impl From<std::io::Error> for Error {
    fn from(err: std::io::Error) -> Self {
        Error::IoError(err)
    }
}


impl From<Error> for SigningError {
    fn from(_: Error) -> Self {
        SigningError::new(SigningErrorType::Error_input_parse)
    }
}
