use crate::ffi::tw_build_p2pkh_script;
use bitcoin::{ScriptBuf, PublicKey};
use tw_encoding::hex;
use tw_proto::Bitcoin::Proto as LegacyProto;

const SATOSHIS: i64 = 0;
const PUBKEY: &str = "028d7dce6d72fb8f7af9566616c6436349c67ad379f2404dd66fe7085fe0fba28f";

#[test]
fn ffi_tw_build_p2pkh_script() {
    let pubkey_slice = hex::decode(PUBKEY).unwrap();
	let pubkey = PublicKey::from_slice(&pubkey_slice).unwrap();

	let raw = unsafe {
		tw_build_p2pkh_script(SATOSHIS, pubkey_slice.as_ptr(), pubkey_slice.len()).into_vec()
	};

	// The expected script.
	let expected = ScriptBuf::new_p2pkh(&pubkey.pubkey_hash());

	let proto: LegacyProto::TransactionOutput = tw_proto::deserialize(&raw).unwrap();
	assert_eq!(proto.value, SATOSHIS);
	assert_eq!(proto.script, expected.as_bytes());
	assert!(proto.spendingScript.is_empty());
}