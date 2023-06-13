#![allow(clippy::missing_safety_doc)]

use super::p2pkh::{TWTxInputP2PKH, TWTxOutputP2PKH};
use super::p2tr_key_path::{TWTxInputP2TRKeyPath, TWTxOutputP2TRKeyPath};
use super::p2wpkh::{TWTxInputP2WPKH, TWTxOutputP2WPKH};
use super::RawPtrTrait;
use crate::{try_or_else, TransactionBuilder};
use secp256k1::KeyPair;
use tw_memory::ffi::c_byte_array::{CByteArray, CByteArrayResult};
use tw_memory::ffi::c_byte_array_ref::CByteArrayRef;
use tw_memory::ffi::c_result::ErrorCode;

pub struct TWTransactionBuilder(TransactionBuilder);

impl RawPtrTrait for TWTransactionBuilder {}

#[no_mangle]
//pub unsafe extern "C" fn tw_build_pay_to_taproot_key_spend_script(
pub unsafe extern "C" fn tw_transaction_builder_create() -> *mut TWTransactionBuilder {
    let builder = TransactionBuilder::new();

    TWTransactionBuilder(builder).into_ptr()
}

#[no_mangle]
pub unsafe extern "C" fn tw_transaction_add_p2pkh_input(
    builder: *mut TWTransactionBuilder,
    input: *mut TWTxInputP2PKH,
) -> *mut TWTransactionBuilder {
    let mut builder = try_or_else!(TWTransactionBuilder::from_ptr_as_box(builder), std::ptr::null_mut);
    let input = try_or_else!(TWTxInputP2PKH::from_ptr(input), std::ptr::null_mut);

    let updated = builder.0.add_input(input.0.into());
    *builder = TWTransactionBuilder(updated);

    Box::into_raw(builder)
}

#[no_mangle]
pub unsafe extern "C" fn tw_transaction_add_p2wpkh_input(
    builder: *mut TWTransactionBuilder,
    input: *mut TWTxInputP2WPKH,
) -> *mut TWTransactionBuilder {
    let mut builder = try_or_else!(TWTransactionBuilder::from_ptr_as_box(builder), std::ptr::null_mut);
    let input = try_or_else!(TWTxInputP2WPKH::from_ptr(input), std::ptr::null_mut);

    let updated = builder.0.add_input(input.0.into());
    *builder = TWTransactionBuilder(updated);

    Box::into_raw(builder)
}

#[no_mangle]
pub unsafe extern "C" fn tw_transaction_add_p2tr_key_path_input(
    builder: *mut TWTransactionBuilder,
    input: *mut TWTxInputP2TRKeyPath,
) -> *mut TWTransactionBuilder {
    let mut builder = try_or_else!(TWTransactionBuilder::from_ptr_as_box(builder), std::ptr::null_mut);
    let input = try_or_else!(TWTxInputP2TRKeyPath::from_ptr(input), std::ptr::null_mut);

    let updated = builder.0.add_input(input.0.into());
    *builder = TWTransactionBuilder(updated);

    Box::into_raw(builder)
}

#[no_mangle]
pub unsafe extern "C" fn tw_transaction_add_p2pkh_output(
    builder: *mut TWTransactionBuilder,
    output: *mut TWTxOutputP2PKH,
) -> *mut TWTransactionBuilder {
    let mut builder = try_or_else!(TWTransactionBuilder::from_ptr_as_box(builder), std::ptr::null_mut);
    let output = try_or_else!(TWTxOutputP2PKH::from_ptr(output), std::ptr::null_mut);

    let updated = builder.0.add_output(output.0.into());
    *builder = TWTransactionBuilder(updated);

    Box::into_raw(builder)
}

#[no_mangle]
pub unsafe extern "C" fn tw_transaction_add_p2wpkh_output(
    builder: *mut TWTransactionBuilder,
    output: *mut TWTxOutputP2WPKH,
) -> *mut TWTransactionBuilder {
    let mut builder = try_or_else!(TWTransactionBuilder::from_ptr_as_box(builder), std::ptr::null_mut);
    let output = try_or_else!(TWTxOutputP2WPKH::from_ptr(output), std::ptr::null_mut);

    let updated = builder.0.add_output(output.0.into());
    *builder = TWTransactionBuilder(updated);

    Box::into_raw(builder)
}

#[no_mangle]
pub unsafe extern "C" fn tw_transaction_add_p2tr_key_path_output(
    builder: *mut TWTransactionBuilder,
    output: *mut TWTxOutputP2TRKeyPath,
) -> *mut TWTransactionBuilder {
    let mut builder = try_or_else!(TWTransactionBuilder::from_ptr_as_box(builder), std::ptr::null_mut);
    let output = try_or_else!(TWTxOutputP2TRKeyPath::from_ptr(output), std::ptr::null_mut);

    let updated = builder.0.add_output(output.0.into());
    *builder = TWTransactionBuilder(updated);

    Box::into_raw(builder)
}

#[repr(C)]
pub enum CTransactionBuilderError {
    Ok = 0,
    InvalidBuilder = 1,
    InvalidSecretKey = 2,
    FailedSigning = 3,
    FailedSerialization = 4,
}

impl From<CTransactionBuilderError> for ErrorCode {
    fn from(e: CTransactionBuilderError) -> Self {
        e as ErrorCode
    }
}

#[no_mangle]
pub unsafe extern "C" fn tw_transaction_sign(
    builder: *mut TWTransactionBuilder,
    secret_key: *const u8,
    secret_key_len: usize,
) -> CByteArrayResult {
    let Some(builder) = TWTransactionBuilder::from_ptr(builder) else {
		return CByteArrayResult::error(CTransactionBuilderError::InvalidBuilder);
	};

    // Convert secret key to keypair.
    let Some(slice) = CByteArrayRef::new(secret_key, secret_key_len).as_slice() else {
		return CByteArrayResult::error(CTransactionBuilderError::InvalidSecretKey);
	};

    let Ok(keypair) = KeyPair::from_seckey_slice(&secp256k1::Secp256k1::new(), slice) else {
		return CByteArrayResult::error(CTransactionBuilderError::InvalidSecretKey);
	};

    // Sign transaction.
    let Ok(builder) = builder.0.sign_inputs(keypair) else {
		return CByteArrayResult::error(CTransactionBuilderError::FailedSigning);
	};

    builder
        .serialize()
        .map(CByteArray::from)
        .map_err(|_| CTransactionBuilderError::FailedSerialization)
        .into()
}
