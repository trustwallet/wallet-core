#![allow(clippy::missing_safety_doc)]

use tw_memory::ffi::c_byte_array::{CByteArray, CByteArrayResult};
use tw_memory::ffi::c_byte_array_ref::CByteArrayRef;
use tw_memory::ffi::c_result::ErrorCode;

#[repr(C)]
pub enum CProtoError {
    Ok = 0,
    ErrorDeserializingMsg = 1,
    ErrorSerializingMsg = 2,
}

impl From<CProtoError> for ErrorCode {
    fn from(e: CProtoError) -> Self {
        e as ErrorCode
    }
}

/// Takes a serialized `Ethereum::Proto::SigningInput` message, deserializes it and returns a serialized message back.
/// This FFI is used within integration tests.
/// \param input *non-null* byte array.
/// \param input_len length of the input byte array.
/// \return C-compatible result with a C-compatible byte array.
#[no_mangle]
pub unsafe extern "C" fn pass_eth_signing_msg_through(
    input: *const u8,
    input_len: usize,
) -> CByteArrayResult {
    let data = CByteArrayRef::new(input, input_len)
        .to_vec()
        .unwrap_or_default();

    let msg: crate::Ethereum::Proto::SigningInput = match crate::deserialize(&data) {
        Ok(msg) => msg,
        Err(_) => return CByteArrayResult::error(CProtoError::ErrorDeserializingMsg),
    };
    crate::serialize(&msg)
        .map(CByteArray::from)
        .map_err(|_| CProtoError::ErrorSerializingMsg)
        .into()
}
