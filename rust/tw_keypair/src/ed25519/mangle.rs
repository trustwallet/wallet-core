// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

/// Clamps the scalar by:
/// 1. clearing the 3 lower bits,
/// 2. clearing the highest bit,
/// 3. setting the second highest bit.
///
/// Source: https://github.com/dalek-cryptography/ed25519-dalek/blob/1.0.1/src/secret.rs#L277-L279C26
pub fn mangle_scalar(scalar: &mut [u8; 32]) {
    scalar[0] &= 0b1111_1000;
    scalar[31] &= 0b0011_1111;
    scalar[31] |= 0b0100_0000;
}
