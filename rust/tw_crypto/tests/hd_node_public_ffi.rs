// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_crypto::ffi::crypto_hd_node_public::{
    tw_hd_node_public_create_with_extended_public_key, tw_hd_node_public_delete,
    tw_hd_node_public_derive_from_path, tw_hd_node_public_public_key_data,
};
use tw_encoding::hex;
use tw_hash::hasher::Hasher;
use tw_keypair::tw::Curve;
use tw_memory::test_utils::tw_data_helper::TWDataHelper;
use tw_memory::test_utils::tw_string_helper::TWStringHelper;
use tw_misc::traits::ToBytesVec;

#[test]
fn test_public_key_from_zpub() {
    use tw_keypair::ecdsa::secp256k1::PublicKey;

    let zpub = "zpub6rNUNtxSa9Gxvm4Bdxf1MPMwrvkzwDx6vP96Hkzw3jiQKdg3fhXBStxjn12YixQB8h88B3RMSRscRstf9AEVaYr3MAqVBEWBDuEJU4PGaT9";
    let zpub_string = TWStringHelper::create(zpub);

    // Create HDNode from extended private key
    let hd_node_ptr = unsafe {
        tw_hd_node_public_create_with_extended_public_key(
            zpub_string.ptr(),
            Curve::Secp256k1.to_raw() as i32,
            Hasher::Sha256d as i32,
        )
    };

    let path = "m/0/0";
    let path_string = TWStringHelper::create(path);

    let hasher = Hasher::Sha256ripemd;

    let derived_node_ptr = unsafe {
        tw_hd_node_public_derive_from_path(hd_node_ptr, path_string.ptr(), hasher as i32)
    };

    let public_key_data =
        TWDataHelper::wrap(unsafe { tw_hd_node_public_public_key_data(derived_node_ptr) });
    assert!(!public_key_data.is_null());

    let public_key_bytes = public_key_data.to_vec().unwrap();
    let public_key = PublicKey::try_from(public_key_bytes.as_slice()).unwrap();

    let public_key_hex = hex::encode(public_key.to_vec(), false);
    assert_eq!(
        public_key_hex,
        "02df9ef2a7a5552765178b181e1e1afdefc7849985c7dfe9647706dd4fa40df6ac"
    );

    unsafe { tw_hd_node_public_delete(derived_node_ptr) };
    unsafe { tw_hd_node_public_delete(hd_node_ptr) };
}

#[test]
fn test_public_key_from_dpub() {
    use tw_keypair::ecdsa::secp256k1::PublicKey;

    let zpub = "dpubZFUmm9oh5zmQkR2Tr2AXS4tCkTWg4B27SpCPFkapZrrAqgU1EwgEFgrmi6EnLGXhak86yDHhXPxFAnGU58W5S4e8NCKG1ASUVaxwRqqNdfP";
    let zpub_string = TWStringHelper::create(zpub);

    // Create HDNode from extended private key
    let hd_node_ptr = unsafe {
        tw_hd_node_public_create_with_extended_public_key(
            zpub_string.ptr(),
            Curve::Secp256k1.to_raw() as i32,
            Hasher::Blake256d as i32,
        )
    };

    let path = "m/0/0";
    let path_string = TWStringHelper::create(path);

    let hasher = Hasher::Blake256ripemd;

    let derived_node_ptr = unsafe {
        tw_hd_node_public_derive_from_path(hd_node_ptr, path_string.ptr(), hasher as i32)
    };

    let public_key_data =
        TWDataHelper::wrap(unsafe { tw_hd_node_public_public_key_data(derived_node_ptr) });
    assert!(!public_key_data.is_null());

    let public_key_bytes = public_key_data.to_vec().unwrap();
    let public_key = PublicKey::try_from(public_key_bytes.as_slice()).unwrap();

    let public_key_hex = hex::encode(public_key.to_vec(), false);
    assert_eq!(
        public_key_hex,
        "035328794a9392d6618c0f8071d2ba25fecef85230d916c2ef17f578af70483103"
    );

    unsafe { tw_hd_node_public_delete(derived_node_ptr) };
    unsafe { tw_hd_node_public_delete(hd_node_ptr) };
}

#[test]
fn test_public_key_from_extended_ethereum() {
    let xpub = "xpub6C7LtZJgtz1BKXG9mExKUxYvX7HSF38UMMmGbpqNQw3DfYwAw8E6sH7VSVxFipvEEm2afSqTjoRgcLmycXX4zfxCWJ4HY73a9KdgvfHEQGB";
    let xpub_string = TWStringHelper::create(xpub);

    let hd_node_ptr = unsafe {
        tw_hd_node_public_create_with_extended_public_key(
            xpub_string.ptr(),
            Curve::Secp256k1.to_raw() as i32,
            Hasher::Sha256d as i32,
        )
    };
    assert!(!hd_node_ptr.is_null());

    let hasher = Hasher::Sha256ripemd;
    let path = "m/0/1";
    let path_string = TWStringHelper::create(path);

    let derived_node_ptr = unsafe {
        tw_hd_node_public_derive_from_path(hd_node_ptr, path_string.ptr(), hasher as i32)
    };

    let public_key_data =
        TWDataHelper::wrap(unsafe { tw_hd_node_public_public_key_data(derived_node_ptr) });
    assert!(!public_key_data.is_null());

    let public_key_bytes = public_key_data.to_vec().unwrap();

    let public_key_hex = hex::encode(public_key_bytes, false);
    assert_eq!(
        public_key_hex,
        "024516c4aa5352035e1bb5be132694e1389a4ac37d32e5e717d35ee4c4dfab5132"
    );

    unsafe { tw_hd_node_public_delete(derived_node_ptr) };
    unsafe { tw_hd_node_public_delete(hd_node_ptr) };
}

#[test]
fn test_public_key_from_zpub_nist() {
    let zpub = "zpub6vBy1RjDYQ9EQZJYiWM1qvSdxKyjmEETobMBD9T9bwwmSGX1y4XrSDAKBZzHCjckGoh8gMEFmx3qyUEU2kcg3wMtUG5CpTtpqdzf8YShMLg";
    let zpub_string = TWStringHelper::create(zpub);

    // Create HDNode from extended private key
    let hd_node_ptr = unsafe {
        tw_hd_node_public_create_with_extended_public_key(
            zpub_string.ptr(),
            Curve::Nist256p1.to_raw() as i32,
            Hasher::Sha256d as i32,
        )
    };

    let path = "m/0/0";
    let path_string = TWStringHelper::create(path);

    let hasher = Hasher::Sha256ripemd;

    let derived_node_ptr = unsafe {
        tw_hd_node_public_derive_from_path(hd_node_ptr, path_string.ptr(), hasher as i32)
    };

    let public_key_data =
        TWDataHelper::wrap(unsafe { tw_hd_node_public_public_key_data(derived_node_ptr) });
    assert!(!public_key_data.is_null());

    unsafe { tw_hd_node_public_delete(derived_node_ptr) };
    unsafe { tw_hd_node_public_delete(hd_node_ptr) };
}
