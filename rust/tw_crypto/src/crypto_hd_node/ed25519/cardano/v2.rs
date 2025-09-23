// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

// Taken from https://github.com/typed-io/rust-ed25519-bip32/blob/master/src/derivation/v2.rs

pub fn add_28_mul8_v2(x: &[u8; 32], y: &[u8; 32]) -> [u8; 32] {
    let mut carry: u16 = 0;
    let mut out = [0u8; 32];

    for i in 0..28 {
        let r = x[i] as u16 + ((y[i] as u16) << 3) + carry;
        out[i] = (r & 0xff) as u8;
        carry = r >> 8;
    }
    for i in 28..32 {
        let r = x[i] as u16 + carry;
        out[i] = (r & 0xff) as u8;
        carry = r >> 8;
    }
    out
}

pub fn add_256bits_v2(x: &[u8; 32], y: &[u8; 32]) -> [u8; 32] {
    let mut carry: u16 = 0;
    let mut out = [0u8; 32];
    for i in 0..32 {
        let r = (x[i] as u16) + (y[i] as u16) + carry;
        out[i] = r as u8;
        carry = r >> 8;
    }
    out
}

pub fn le32(i: u32) -> [u8; 4] {
    [i as u8, (i >> 8) as u8, (i >> 16) as u8, (i >> 24) as u8]
}
