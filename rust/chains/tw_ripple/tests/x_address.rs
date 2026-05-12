// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::str::FromStr;
use tw_hash::H160;
use tw_ripple::address::x_address::{TagFlag, XAddress};

#[test]
fn test_x_address_from_str() {
    let expected_key_hash = H160::from("2decab42ca805119a9ba2ff305c9afa12f0b86a1");

    let addr = XAddress::from_str("X76UnYEMbQfEs3mUqgtjp4zFy9exgThRj7XVZ6UxsdrBptF").unwrap();
    assert_eq!(addr.destination_tag(), 12_345);
    assert_eq!(addr.tag_flag(), TagFlag::Classic);
    assert_eq!(addr.public_key_hash(), expected_key_hash);

    let addr = XAddress::from_str("X76UnYEMbQfEs3mUqgtjp4zFy9exgTsM93nriVZAPufrpE3").unwrap();
    assert_eq!(addr.destination_tag(), 0);
    assert_eq!(addr.tag_flag(), TagFlag::Classic);
    assert_eq!(addr.public_key_hash(), expected_key_hash);

    // Valid no-tag (tag_flag=None, tag bytes are zero).
    let addr = XAddress::from_str("X76UnYEMbQfEs3mUqgtjp4zFy9exgSxWAqcQwu9z2r5d7Tm").unwrap();
    assert_eq!(addr.destination_tag(), 0);
    assert_eq!(addr.tag_flag(), TagFlag::None);
    assert_eq!(addr.public_key_hash(), expected_key_hash);
}

/// An X-address with tag_flag=None but non-zero tag bytes must be rejected.
/// Before the fix, from_str() accepted such addresses, allowing destination_tag()
/// to return a non-zero value that pre_process_x_addr() would then inject into
/// signed transactions despite the address declaring itself tagless.
#[test]
fn test_x_address_rejects_malformed_no_tag() {
    // tag_flag=None (0x00), tag bytes = 12345 LE — structurally invalid.
    assert!(
        XAddress::from_str("X76UnYEMbQfEs3mUqgtjp4zFy9exgSybYUh1taorDmyP7YL").is_err(),
        "Malformed no-tag X-address (flag=None, tag=12345) must be rejected"
    );
}
