use ciborium::{de, ser};
use serde::{de::DeserializeOwned, Serialize};

type CborResult<T> = Result<T, String>;

pub fn serialize<S: Serialize>(message: &S) -> CborResult<Vec<u8>> {
    let mut bytes = vec![];
    ser::into_writer(message, &mut bytes).map_err(|e| e.to_string())?;
    Ok(bytes)
}

pub fn deserialize<D: DeserializeOwned>(data: &[u8]) -> CborResult<D> {
    de::from_reader(data).map_err(|e| e.to_string())
}
