use std::io;

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
