use super::CTaprootError;
use crate::Recipient;
use bitcoin::PublicKey;
use std::ffi::CString;
use tw_memory::ffi::c_byte_array_ref::CByteArrayRef;
use tw_memory::ffi::c_result::CStrMutResult;

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

    CStrMutResult::ok(c_string)
}

#[no_mangle]
pub unsafe extern "C" fn tw_segwit_address_string(
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

    let Ok(address) = recipient.segwit_address_string(network.into()) else {
        return CStrMutResult::error(CTaprootError::InvalidSegwitPukey);
    };

    let c_string = CString::new(address)
        .expect("legacy address contains an internal 0 byte")
        .into_raw();

    CStrMutResult::ok(c_string)
}

#[no_mangle]
pub unsafe extern "C" fn tw_taproot_address_string(
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

    let address = recipient.taproot_address_string(network.into());
    let c_string = CString::new(address)
        .expect("legacy address contains an internal 0 byte")
        .into_raw();

    CStrMutResult::ok(c_string)
}

// A custom reimplementation of of `bitcoin::Network`.
#[repr(C)]
pub enum Network {
    Bitcoin = 0,
    Testnet = 1,
    Signet = 2,
    Regtest = 3,
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
