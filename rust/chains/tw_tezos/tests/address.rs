// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::str::FromStr;
use tw_encoding::hex;
use tw_keypair::ed25519;
use tw_tezos::address::TezosAddress;

#[test]
fn test_account_zero() {
    let addr = TezosAddress::from_str("tz1eZwq8b5cvE2bPKokatLkVMzkxz24z3Don").unwrap();
    let forged = addr.forge().unwrap();
    assert_eq!(
        hex::encode(forged, false),
        "0000cfa4aae60f5d9389752d41e320da224d43287fe2"
    );
}

#[test]
fn test_forge_tz2() {
    let addr = TezosAddress::from_str("tz2Rh3NYeLxrqTuvaZJmaMiVMqCajeXMWtYo").unwrap();
    let forged = addr.forge().unwrap();
    assert_eq!(
        hex::encode(forged, false),
        "0001be99dd914e38388ec80432818b517759e3524f16"
    );
}

#[test]
fn test_forge_tz3() {
    let addr = TezosAddress::from_str("tz3RDC3Jdn4j15J7bBHZd29EUee9gVB1CxD9").unwrap();
    let forged = addr.forge().unwrap();
    assert_eq!(
        hex::encode(forged, false),
        "0002358cbffa97149631cfb999fa47f0035fb1ea8636"
    );
}

#[test]
fn test_forge_kt1() {
    let addr = TezosAddress::from_str("KT1XnTn74bUtxHfDtBmm2bGZAQfhPbvKWR8o").unwrap();
    let forged = addr.forge().unwrap();
    assert_eq!(
        hex::encode(forged, false),
        "01fe810959c3d6127a41cbd471e7cb4e91a61b780b00"
    );
}

#[test]
fn test_invalid_addresses() {
    let invalid_addresses = [
        "NmH7tmeJUmHcncBDvpr7aJNEBk7rp5zYsB1qt", // Invalid prefix, valid checksum
        "tz1eZwq8b5cvE2bPKokatLkVMzkxz24z3AAAA", // Valid prefix, invalid checksum
        "1tzeZwq8b5cvE2bPKokatLkVMzkxz24zAAAAA", // Invalid prefix, invalid checksum
    ];

    for address in invalid_addresses {
        assert!(!TezosAddress::from_str(address).is_ok());
    }
}

#[test]
fn test_valid_addresses() {
    let valid_addresses = [
        "tz1Yju7jmmsaUiG9qQLoYv35v5pHgnWoLWbt",
        "tz2PdGc7U5tiyqPgTSgqCDct94qd6ovQwP6u",
        "tz3VEZ4k6a4Wx42iyev6i2aVAptTRLEAivNN",
        "KT1XnTn74bUtxHfDtBmm2bGZAQfhPbvKWR8o",
    ];

    for address in valid_addresses {
        assert!(TezosAddress::from_str(address).is_ok());
    }
}

#[test]
fn test_string() {
    let address_string = "tz1d1qQL3mYVuiH4JPFvuikEpFwaDm85oabM";
    let address = TezosAddress::from_str(address_string).unwrap();
    assert_eq!(address.to_string(), address_string);
}

#[test]
fn test_public_key_init() {
    let bytes =
        hex::decode("01fe157cc8011727936c592f856c9071d39cf4acdadfa6d76435e4619c9dc56f63").unwrap();
    let public_key = ed25519::sha512::PublicKey::try_from(&bytes[1..]).unwrap();
    let address = TezosAddress::with_ed25519_public_key(&public_key).unwrap();
    let expected = "tz1cG2jx3W4bZFeVGBjsTxUAG8tdpTXtE8PT";
    assert_eq!(address.to_string(), expected);
}
