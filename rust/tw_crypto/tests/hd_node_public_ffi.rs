// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_crypto::crypto_hd_node::extended_key::bip32_public_key::BIP32PublicKey;
use tw_crypto::ffi::crypto_hd_node_public::{
    tw_hd_node_public_create_with_extended_public_key, tw_hd_node_public_derive_from_path,
    tw_hd_node_public_public_key_data,
};
use tw_encoding::hex;
use tw_keypair::tw::Curve;
use tw_memory::ffi::{tw_data::TWData, RawPtrTrait};
use tw_memory::test_utils::tw_string_helper::TWStringHelper;

#[test]
fn test_public_key_from_xprv() {
    use tw_keypair::ecdsa::secp256k1::PublicKey;

    let zpub = "zpub6rNUNtxSa9Gxvm4Bdxf1MPMwrvkzwDx6vP96Hkzw3jiQKdg3fhXBStxjn12YixQB8h88B3RMSRscRstf9AEVaYr3MAqVBEWBDuEJU4PGaT9";
    let zpub_string = TWStringHelper::create(zpub);

    // Create HDNode from extended private key
    let hd_node_ptr = unsafe {
        tw_hd_node_public_create_with_extended_public_key(
            zpub_string.ptr(),
            Curve::Secp256k1.to_raw(),
        )
    };

    let path = "m/0/0";
    let path_string = TWStringHelper::create(path);
    let derived_node_ptr =
        unsafe { tw_hd_node_public_derive_from_path(hd_node_ptr, path_string.ptr()) };

    let public_key_data = unsafe { tw_hd_node_public_public_key_data(derived_node_ptr) };
    assert!(!public_key_data.is_null());

    let public_key_bytes = unsafe { TWData::from_ptr_as_ref(public_key_data).unwrap().to_vec() };
    let public_key = PublicKey::try_from(public_key_bytes.as_slice()).unwrap();

    let public_key_hex = hex::encode(public_key.to_bytes(), false);
    assert_eq!(
        public_key_hex,
        "02df9ef2a7a5552765178b181e1e1afdefc7849985c7dfe9647706dd4fa40df6ac"
    );
}
