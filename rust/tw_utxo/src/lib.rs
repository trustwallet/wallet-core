use tw_proto::Utxo::Proto;

pub mod compiler;

pub type Result<T> = std::result::Result<T, Error>;

#[derive(Debug)]
pub struct Error(Proto::Error);

impl From<Proto::Error> for Error {
    fn from(value: Proto::Error) -> Self {
        Error(value)
    }
}

impl From<bitcoin::sighash::Error> for Error {
    fn from(_value: bitcoin::sighash::Error) -> Self {
        Error(Proto::Error::Error_sighash_failed)
    }
}

impl From<Error> for Proto::Error {
    fn from(value: Error) -> Self {
        value.0
    }
}
