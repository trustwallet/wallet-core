// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::{IV_SIZE, KEY_SIZE_AES_128, KEY_SIZE_AES_192, KEY_SIZE_AES_256};
use aes::cipher::{KeyIvInit, StreamCipher, StreamCipherError};
use aes::{Aes128, Aes192, Aes256};
use ctr::Ctr64BE;

type Aes128Ctr64BE = Ctr64BE<Aes128>;
type Aes192Ctr64BE = Ctr64BE<Aes192>;
type Aes256Ctr64BE = Ctr64BE<Aes256>;

const MAX_DATA_SIZE: usize = 10 * 1024 * 1024; // 10 MB

fn aes_ctr_process<C: KeyIvInit + StreamCipher>(
    data: &[u8],
    iv: &[u8],
    key: &[u8],
    key_size: usize,
) -> Result<Vec<u8>, StreamCipherError> {
    if data.len() > MAX_DATA_SIZE {
        return Err(StreamCipherError);
    }
    if iv.len() != IV_SIZE {
        return Err(StreamCipherError);
    }
    if key.len() < key_size {
        return Err(StreamCipherError);
    }
    let key = &key[0..key_size];
    let mut cipher = C::new(key.into(), iv.into());
    let mut data_vec = data.to_vec();
    cipher.try_apply_keystream(&mut data_vec)?;
    Ok(data_vec)
}

pub fn aes_ctr_encrypt_128(
    data: &[u8],
    iv: &[u8],
    key: &[u8],
) -> Result<Vec<u8>, StreamCipherError> {
    aes_ctr_process::<Aes128Ctr64BE>(data, iv, key, KEY_SIZE_AES_128)
}

pub fn aes_ctr_decrypt_128(
    data: &[u8],
    iv: &[u8],
    key: &[u8],
) -> Result<Vec<u8>, StreamCipherError> {
    aes_ctr_process::<Aes128Ctr64BE>(data, iv, key, KEY_SIZE_AES_128)
}

pub fn aes_ctr_encrypt_192(
    data: &[u8],
    iv: &[u8],
    key: &[u8],
) -> Result<Vec<u8>, StreamCipherError> {
    aes_ctr_process::<Aes192Ctr64BE>(data, iv, key, KEY_SIZE_AES_192)
}

pub fn aes_ctr_decrypt_192(
    data: &[u8],
    iv: &[u8],
    key: &[u8],
) -> Result<Vec<u8>, StreamCipherError> {
    aes_ctr_process::<Aes192Ctr64BE>(data, iv, key, KEY_SIZE_AES_192)
}

pub fn aes_ctr_encrypt_256(
    data: &[u8],
    iv: &[u8],
    key: &[u8],
) -> Result<Vec<u8>, StreamCipherError> {
    aes_ctr_process::<Aes256Ctr64BE>(data, iv, key, KEY_SIZE_AES_256)
}

pub fn aes_ctr_decrypt_256(
    data: &[u8],
    iv: &[u8],
    key: &[u8],
) -> Result<Vec<u8>, StreamCipherError> {
    aes_ctr_process::<Aes256Ctr64BE>(data, iv, key, KEY_SIZE_AES_256)
}

pub fn aes_ctr_encrypt(data: &[u8], iv: &[u8], key: &[u8]) -> Result<Vec<u8>, StreamCipherError> {
    match key.len() {
        KEY_SIZE_AES_128 => aes_ctr_encrypt_128(data, iv, key),
        KEY_SIZE_AES_192 => aes_ctr_encrypt_192(data, iv, key),
        KEY_SIZE_AES_256 => aes_ctr_encrypt_256(data, iv, key),
        _ => Err(StreamCipherError),
    }
}

pub fn aes_ctr_decrypt(data: &[u8], iv: &[u8], key: &[u8]) -> Result<Vec<u8>, StreamCipherError> {
    match key.len() {
        KEY_SIZE_AES_128 => aes_ctr_decrypt_128(data, iv, key),
        KEY_SIZE_AES_192 => aes_ctr_decrypt_192(data, iv, key),
        KEY_SIZE_AES_256 => aes_ctr_decrypt_256(data, iv, key),
        _ => Err(StreamCipherError),
    }
}
