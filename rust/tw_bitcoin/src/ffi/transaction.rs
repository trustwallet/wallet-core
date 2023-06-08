use crate::{output::TxOutputP2TRKeyPath, Recipient, TransactionBuilder};
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
