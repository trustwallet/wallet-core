mod u256;

pub use u256::U256;

pub type NumberResult<T> = Result<T, NumberError>;

#[derive(Debug)]
pub enum NumberError {
    InvalidBinaryRepresentation,
    InvalidStringRepresentation,
}
