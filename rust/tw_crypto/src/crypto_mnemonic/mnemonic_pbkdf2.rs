// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

// Taken from https://github.com/rust-bitcoin/rust-bip39/blob/master/src/pbkdf2.rs
// The only reason we had to copy this is because we want to obtain the seed
// for an invalid mnemonic if the validity check is disabled.

use bitcoin_hashes::{hmac, sha512, Hash, HashEngine};

const SALT_PREFIX: &'static str = "mnemonic";

/// Calculate the binary size of the mnemonic.
fn mnemonic_byte_len<M>(mnemonic: M) -> usize
where
    M: Iterator<Item = &'static str> + Clone,
{
    let mut len = 0;
    for (i, word) in mnemonic.enumerate() {
        if i > 0 {
            len += 1;
        }
        len += word.len();
    }
    len
}

/// Wrote the mnemonic in binary form into the hash engine.
fn mnemonic_write_into<M>(mnemonic: M, engine: &mut sha512::HashEngine)
where
    M: Iterator<Item = &'static str> + Clone,
{
    for (i, word) in mnemonic.enumerate() {
        if i > 0 {
            engine.input(" ".as_bytes());
        }
        engine.input(word.as_bytes());
    }
}

/// Create an HMAC engine from the passphrase.
/// We need a special method because we can't allocate a new byte
/// vector for the entire serialized mnemonic.
fn create_hmac_engine<M>(mnemonic: M) -> hmac::HmacEngine<sha512::Hash>
where
    M: Iterator<Item = &'static str> + Clone,
{
    // Inner code is borrowed from the bitcoin_hashes::hmac::HmacEngine::new method.
    let mut ipad = [0x36u8; 128];
    let mut opad = [0x5cu8; 128];
    let mut iengine = sha512::Hash::engine();
    let mut oengine = sha512::Hash::engine();

    if mnemonic_byte_len(mnemonic.clone()) > sha512::HashEngine::BLOCK_SIZE {
        let hash = {
            let mut engine = sha512::Hash::engine();
            mnemonic_write_into(mnemonic, &mut engine);
            sha512::Hash::from_engine(engine)
        };

        for (b_i, b_h) in ipad.iter_mut().zip(&hash[..]) {
            *b_i ^= *b_h;
        }
        for (b_o, b_h) in opad.iter_mut().zip(&hash[..]) {
            *b_o ^= *b_h;
        }
    } else {
        // First modify the first elements from the prefix.
        let mut cursor = 0;
        for (i, word) in mnemonic.enumerate() {
            if i > 0 {
                ipad[cursor] ^= ' ' as u8;
                opad[cursor] ^= ' ' as u8;
                cursor += 1;
            }
            for (b_i, b_h) in ipad.iter_mut().skip(cursor).zip(word.as_bytes()) {
                *b_i ^= *b_h;
            }
            for (b_o, b_h) in opad.iter_mut().skip(cursor).zip(word.as_bytes()) {
                *b_o ^= *b_h;
            }
            cursor += word.len();
            assert!(
                cursor <= sha512::HashEngine::BLOCK_SIZE,
                "mnemonic_byte_len is broken"
            );
        }
    };

    iengine.input(&ipad[..sha512::HashEngine::BLOCK_SIZE]);
    oengine.input(&opad[..sha512::HashEngine::BLOCK_SIZE]);
    hmac::HmacEngine::from_inner_engines(iengine, oengine)
}

// Method borrowed from rust-bitcoin's endian module.
#[inline]
fn u32_to_array_be(val: u32) -> [u8; 4] {
    let mut res = [0; 4];
    for i in 0..4 {
        res[i] = ((val >> (4 - i - 1) * 8) & 0xff) as u8;
    }
    res
}

#[inline]
fn xor(res: &mut [u8], salt: &[u8]) {
    debug_assert!(salt.len() >= res.len(), "length mismatch in xor");

    res.iter_mut().zip(salt.iter()).for_each(|(a, b)| *a ^= b);
}

/// PBKDF2-HMAC-SHA512 implementation using bitcoin_hashes.
pub(crate) fn pbkdf2<M>(mnemonic: M, unprefixed_salt: &[u8], c: usize, res: &mut [u8])
where
    M: Iterator<Item = &'static str> + Clone,
{
    let prf = create_hmac_engine(mnemonic);

    for (i, chunk) in res.chunks_mut(sha512::Hash::LEN).enumerate() {
        for v in chunk.iter_mut() {
            *v = 0;
        }

        let mut salt = {
            let mut prfc = prf.clone();
            prfc.input(SALT_PREFIX.as_bytes());
            prfc.input(unprefixed_salt);
            prfc.input(&u32_to_array_be((i + 1) as u32));

            let salt = hmac::Hmac::from_engine(prfc).to_byte_array();
            xor(chunk, &salt);
            salt
        };

        for _ in 1..c {
            let mut prfc = prf.clone();
            prfc.input(&salt);
            salt = hmac::Hmac::from_engine(prfc).to_byte_array();

            xor(chunk, &salt);
        }
    }
}
