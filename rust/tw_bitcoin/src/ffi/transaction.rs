use crate::{output::TxOutputP2TRKeyPath, Recipient, TransactionBuilder};
use super::p2tr_key_path::{TWTxInputP2TRKeyPath, TWTxOutputP2TRKeyPath};
use super::p2wpkh::{TWTxInputP2WPKH, TWTxOutputP2WPKH};
use bitcoin::PublicKey;
use bitcoin::ScriptBuf;
use tw_memory::ffi::c_byte_array::CByteArray;
use tw_memory::ffi::c_byte_array_ref::CByteArrayRef;
use tw_memory::ffi::RawPtrTrait;
use tw_misc::{try_or_else, try_or_false};

pub struct TWTransactionBuilder(TransactionBuilder);

impl RawPtrTrait for TWTransactionBuilder {}

#[no_mangle]
//pub unsafe extern "C" fn tw_build_pay_to_taproot_key_spend_script(
pub unsafe extern "C" fn tw_transaction_builder_create() -> *mut TWTransactionBuilder {
    let builder = TransactionBuilder::new();

    TWTransactionBuilder(builder).into_ptr()
}

pub unsafe extern "C" fn tw_transaction_add_p2wpkh_input(
	builder: *mut TWTransactionBuilder,
	input: *mut TWTxInputP2WPKH,
) -> *mut TWTransactionBuilder {
	let builder = try_or_else!(TWTransactionBuilder::from_ptr(builder), std::ptr::null_mut);
	let input = try_or_else!(TWTxInputP2WPKH::from_ptr(input), std::ptr::null_mut);

	let builder = builder.0.add_input(input.0.into());

    TWTransactionBuilder(builder).into_ptr()
}

pub unsafe extern "C" fn tw_transaction_add_p2tr_key_path_input(
	builder: *mut TWTransactionBuilder,
	input: *mut TWTxInputP2TRKeyPath,
) -> *mut TWTransactionBuilder {
	let builder = try_or_else!(TWTransactionBuilder::from_ptr(builder), std::ptr::null_mut);
	let input = try_or_else!(TWTxInputP2TRKeyPath::from_ptr(input), std::ptr::null_mut);

	let builder = builder.0.add_input(input.0.into());

    TWTransactionBuilder(builder).into_ptr()
}

pub unsafe extern "C" fn tw_transaction_add_p2wpkh_output(
	builder: *mut TWTransactionBuilder,
	output: *mut TWTxOutputP2WPKH,
) -> *mut TWTransactionBuilder {
	let builder = try_or_else!(TWTransactionBuilder::from_ptr(builder), std::ptr::null_mut);
	let output = try_or_else!(TWTxOutputP2WPKH::from_ptr(output), std::ptr::null_mut);

	let builder = builder.0.add_output(output.0.into());

    TWTransactionBuilder(builder).into_ptr()
}

pub unsafe extern "C" fn tw_transaction_add_p2tr_key_path_output(
	builder: *mut TWTransactionBuilder,
	output: *mut TWTxOutputP2TRKeyPath,
) -> *mut TWTransactionBuilder {
	let builder = try_or_else!(TWTransactionBuilder::from_ptr(builder), std::ptr::null_mut);
	let output = try_or_else!(TWTxOutputP2TRKeyPath::from_ptr(output), std::ptr::null_mut);

	let builder = builder.0.add_output(output.0.into());

    TWTransactionBuilder(builder).into_ptr()
}
