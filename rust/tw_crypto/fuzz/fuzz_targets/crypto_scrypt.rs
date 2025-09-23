// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#![no_main]

use libfuzzer_sys::{arbitrary, fuzz_target};
use tw_crypto::crypto_scrypt::{params::Params, scrypt};

#[derive(arbitrary::Arbitrary, Debug)]
struct ScryptInput<'a> {
    password: &'a [u8],
    salt: &'a [u8],
    log_n: u8,
    r: u32,
    p: u32,
    desired_len: usize,
}

fuzz_target!(|input: ScryptInput<'_>| {
    // Greater r, p parameters make `scrypt` incredibly slow.
    if input.r > 16 || input.p > 16 {
        return;
    }

    let params = Params {
        log_n: input.log_n,
        r: input.r,
        p: input.p,
        desired_len: input.desired_len,
    };

    let _ = scrypt(input.password, input.salt, &params);
});
