use crate::{output::TxOutputP2TRKeyPath, Recipient};
use bitcoin::ScriptBuf;
use bitcoin::PublicKey;
use tw_memory::ffi::c_byte_array_ref::CByteArrayRef;
use tw_memory::ffi::c_byte_array::CByteArray;
use tw_memory::ffi::RawPtrTrait;
use tw_misc::{try_or_else, try_or_false};

pub struct Test(pub(crate) TxOutputP2TRKeyPath);

#[no_mangle]
pub unsafe extern "C" fn tw_build_pay_to_taproot_key_spend_script(
	input: *const u8,
	input_len: usize,
) -> CByteArray {
	let input_ref = CByteArrayRef::new(input, input_len);

	// Convert bytes to public key and tweak with zeroed merkle root
	// (the "key-spend" way).
	let input = try_or_else!(input_ref.as_slice(), std::ptr::null_mut);
	let pubkey = try_or_else!(Recipient::<PublicKey>::from_slice(input), std::ptr::null_mut);

	// We create the `scriptPubKey` directly an avoid the Rust builder.
	let script = ScriptBuf::new_v1_p2tr_tweaked(pubkey.tweaked_pubkey());

	CByteArray::from(script.to_bytes())
}
