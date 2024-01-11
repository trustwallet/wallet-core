// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_encoding::hex;
use tw_keypair::ffi::asn::ecdsa_signature_from_asn_der;
use tw_memory::ffi::c_byte_array::CByteArray;

#[test]
fn test_ecdsa_signature_from_asn_der() {
    let encoded = "3046022100db421231f23d0320dbb8f1284b600cd34b8e9218628139539ff4f1f6c05495da022100ff715aab70d5317dbf8ee224eb18bec3120cfb9db1000dbb31eadaf96c71c1b1";
    let expected = "db421231f23d0320dbb8f1284b600cd34b8e9218628139539ff4f1f6c05495daff715aab70d5317dbf8ee224eb18bec3120cfb9db1000dbb31eadaf96c71c1b1";

    let encoded = CByteArray::from(hex::decode(encoded).unwrap());
    let actual = unsafe {
        ecdsa_signature_from_asn_der(encoded.data(), encoded.size())
            .unwrap()
            .into_vec()
    };

    assert_eq!(actual, hex::decode(expected).unwrap());
}

#[test]
fn test_ecdsa_signature_from_asn_der_invalid() {
    let encoded = "3000022100db421231f23d0320dbb8f1284b600cd34b8e9218628139539ff4f1f6c05495da022100ff715aab70d5317dbf8ee224eb18bec3120cfb9db1000dbb31eadaf96c71c1b1";
    let encoded = CByteArray::from(hex::decode(encoded).unwrap());
    let res = unsafe { ecdsa_signature_from_asn_der(encoded.data(), encoded.size()) };
    assert!(res.is_err());
}
