// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

//! Original: https://github.com/RustCrypto/password-hashes/blob/master/scrypt/src/romix.rs
/// Execute the ROMix operation in-place.
/// b - the data to operate on
/// v - a temporary variable to store the vector V (will be zeroized after use)
/// t - a temporary variable to store the result of the xor
/// n - the scrypt parameter N
#[allow(clippy::many_single_char_names)]
pub(crate) fn scrypt_ro_mix(b: &mut [u8], v: &mut [u8], t: &mut [u8], n: usize) {
    fn integerify(x: &[u8], n: usize) -> usize {
        // n is a power of 2, so n - 1 gives us a bitmask that we can use to perform a calculation
        // mod n using a simple bitwise and.
        let mask = n - 1;
        // This cast is safe since we're going to get the value mod n (which is a power of 2), so we
        // don't have to care about truncating any of the high bits off
        //let result = (LittleEndian::read_u32(&x[x.len() - 64..x.len() - 60]) as usize) & mask;
        let t = u32::from_le_bytes(x[x.len() - 64..x.len() - 60].try_into().unwrap());
        (t as usize) & mask
    }

    let len = b.len();

    for chunk in v.chunks_mut(len) {
        chunk.copy_from_slice(b);
        scrypt_block_mix(chunk, b);
    }

    for _ in 0..n {
        let j = integerify(b, n);
        xor(b, &v[j * len..(j + 1) * len], t);
        scrypt_block_mix(t, b);
    }

    // Zeroize the vector v which contains sensitive PBKDF2 output data
    v.iter_mut().for_each(|byte| *byte = 0);
}

/// Execute the BlockMix operation
/// input - the input vector. The length must be a multiple of 128.
/// output - the output vector. Must be the same length as input.
fn scrypt_block_mix(input: &[u8], output: &mut [u8]) {
    use salsa20::{
        cipher::{typenum::U4, StreamCipherCore},
        SalsaCore,
    };

    type Salsa20_8 = SalsaCore<U4>;

    let mut x = [0u8; 64];
    x.copy_from_slice(&input[input.len() - 64..]);

    let mut t = [0u8; 64];

    for (i, chunk) in input.chunks(64).enumerate() {
        xor(&x, chunk, &mut t);

        let mut t2 = [0u32; 16];

        for (c, b) in t.chunks_exact(4).zip(t2.iter_mut()) {
            *b = u32::from_le_bytes(c.try_into().unwrap());
        }

        Salsa20_8::from_raw_state(t2).write_keystream_block((&mut x).into());

        let pos = if i % 2 == 0 {
            (i / 2) * 64
        } else {
            (i / 2) * 64 + input.len() / 2
        };

        output[pos..pos + 64].copy_from_slice(&x);
    }
}

fn xor(x: &[u8], y: &[u8], output: &mut [u8]) {
    for ((out, &x_i), &y_i) in output.iter_mut().zip(x.iter()).zip(y.iter()) {
        *out = x_i ^ y_i;
    }
}
