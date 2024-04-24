// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

pub mod address;
pub mod context;
pub mod modules;
pub mod private_key;
pub mod public_key;
pub mod signature;
pub mod transaction;

#[cfg(feature = "test-utils")]
pub mod test_utils;

#[allow(non_snake_case)]
#[rustfmt::skip]
pub mod proto {
    use tw_proto::google;

    include!(concat!(env!("OUT_DIR"), "/proto/mod.rs"));
}
