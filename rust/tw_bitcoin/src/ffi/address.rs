use super::CTaprootError;
use crate::{try_or_else, Recipient};
use bitcoin::PublicKey;
use std::ffi::CString;
use tw_memory::ffi::c_byte_array_ref::CByteArrayRef;
use tw_memory::ffi::c_result::{CStrMutResult, ErrorCode};

// A custom reimplementation of of `bitcoin::Network`.
#[repr(C)]
pub enum Network {
    Bitcoin = 0,
    Testnet = 1,
    Signet = 2,
    Regtest = 3,
}

#[no_mangle]
pub unsafe extern "C" fn tw_legacy_address_string(
    pubkey: *const u8,
    pubkey_len: usize,
    network: Network,
) -> CStrMutResult {
    // Convert Recipient.
    let Some(slice) = CByteArrayRef::new(pubkey, pubkey_len).as_slice() else {
		return CStrMutResult::error(CTaprootError::InvalidSlice);
	};

    let Ok(recipient) = Recipient::<PublicKey>::from_slice(slice) else {
		return CStrMutResult::error(CTaprootError::InvalidPubkey);
	};

    let address = recipient.legacy_address_string(network.into());
    let c_string = CString::new(address)
        .expect("legacy address contains an internal 0 byte")
        .into_raw();

    CStrMutResult::from(Result::<_, CTaprootError>::Ok(c_string))
}

impl From<Network> for bitcoin::Network {
    fn from(n: Network) -> Self {
        match n {
            Network::Bitcoin => bitcoin::Network::Bitcoin,
            Network::Testnet => bitcoin::Network::Testnet,
            Network::Signet => bitcoin::Network::Signet,
            Network::Regtest => bitcoin::Network::Regtest,
        }
    }
}
