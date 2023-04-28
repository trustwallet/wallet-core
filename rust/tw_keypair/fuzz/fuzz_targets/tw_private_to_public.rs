// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#![no_main]

use libfuzzer_sys::{arbitrary, fuzz_target};
use tw_keypair::tw::{PrivateKey, PublicKeyType};

#[derive(arbitrary::Arbitrary, Debug)]
struct TWPrivateToPublicInput {
    private: Vec<u8>,
    public_key_type: PublicKeyType,
}

fuzz_target!(|input: TWPrivateToPublicInput| {
    if let Ok(private) = PrivateKey::new(input.private) {
        private.get_public_key_by_type(input.public_key_type).ok();
    }
});
