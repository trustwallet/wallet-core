// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#![no_main]

use libfuzzer_sys::{arbitrary, fuzz_target};
use tw_keypair::tw::{Curve, PrivateKey};

#[derive(arbitrary::Arbitrary, Debug)]
struct TWPrivateSignInput {
    private: Vec<u8>,
    hash: Vec<u8>,
    curve: Curve,
}

fuzz_target!(|input: TWPrivateSignInput| {
    if let Ok(private) = PrivateKey::new(input.private) {
        private.sign(&input.hash, input.curve).ok();
    }
});
