mod u256;

pub use u256::U256;

pub type NumberResult<T> = Result<T, NumberError>;

pub enum NumberError {
    InvalidBinaryRepresentation,
}
