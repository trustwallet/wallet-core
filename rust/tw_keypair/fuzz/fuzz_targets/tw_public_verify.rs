// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#![no_main]

use libfuzzer_sys::{arbitrary, fuzz_target};
use tw_keypair::tw::{PublicKey, PublicKeyType};

#[derive(arbitrary::Arbitrary, Debug)]
struct TWPublicVerifyInput {
    public: Vec<u8>,
    hash: Vec<u8>,
    signature: Vec<u8>,
    pubkey_type: PublicKeyType,
}

fuzz_target!(|input: TWPublicVerifyInput| {
    if let Ok(public) = PublicKey::new(input.public, input.pubkey_type) {
        public.verify(&input.signature, &input.hash);
    }
});
