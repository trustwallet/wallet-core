// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use aes::cipher::{KeyIvInit, StreamCipher, StreamCipherError};
use aes::{Aes128, Aes192, Aes256};
use ctr::Ctr64BE;

type Aes128Ctr64BE = Ctr64BE<Aes128>;
type Aes192Ctr64BE = Ctr64BE<Aes192>;
type Aes256Ctr64BE = Ctr64BE<Aes256>;

pub fn aes_ctr_encrypt_128(
    data: &[u8],
    iv: &[u8],
    key: &[u8],
) -> Result<Vec<u8>, StreamCipherError> {
    // Truncate key to 16 bytes (128 bits) for AES-128
    let key = if key.len() > 16 { &key[0..16] } else { key };
    let mut cipher = Aes128Ctr64BE::new(key.into(), iv.into());
    let mut data_vec = data.to_vec();
    cipher.try_apply_keystream(&mut data_vec)?;
    Ok(data_vec)
}

pub fn aes_ctr_decrypt_128(
    data: &[u8],
    iv: &[u8],
    key: &[u8],
) -> Result<Vec<u8>, StreamCipherError> {
    let key = if key.len() > 16 { &key[0..16] } else { key };
    let mut cipher = Aes128Ctr64BE::new(key.into(), iv.into());
    let mut data_vec = data.to_vec();
    cipher.try_apply_keystream(&mut data_vec)?;
    Ok(data_vec)
}

pub fn aes_ctr_encrypt_192(
    data: &[u8],
    iv: &[u8],
    key: &[u8],
) -> Result<Vec<u8>, StreamCipherError> {
    // Truncate key to 24 bytes (192 bits) for AES-192
    let key = if key.len() > 24 { &key[0..24] } else { key };
    let mut cipher = Aes192Ctr64BE::new(key.into(), iv.into());
    let mut data_vec = data.to_vec();
    cipher.try_apply_keystream(&mut data_vec)?;
    Ok(data_vec)
}

pub fn aes_ctr_decrypt_192(
    data: &[u8],
    iv: &[u8],
    key: &[u8],
) -> Result<Vec<u8>, StreamCipherError> {
    let key = if key.len() > 24 { &key[0..24] } else { key };
    let mut cipher = Aes192Ctr64BE::new(key.into(), iv.into());
    let mut data_vec = data.to_vec();
    cipher.try_apply_keystream(&mut data_vec)?;
    Ok(data_vec)
}

pub fn aes_ctr_encrypt_256(
    data: &[u8],
    iv: &[u8],
    key: &[u8],
) -> Result<Vec<u8>, StreamCipherError> {
    // Truncate key to 32 bytes (256 bits) for AES-256
    let key = if key.len() > 32 { &key[0..32] } else { key };
    let mut cipher = Aes256Ctr64BE::new(key.into(), iv.into());
    let mut data_vec = data.to_vec();
    cipher.try_apply_keystream(&mut data_vec)?;
    Ok(data_vec)
}

pub fn aes_ctr_decrypt_256(
    data: &[u8],
    iv: &[u8],
    key: &[u8],
) -> Result<Vec<u8>, StreamCipherError> {
    let key = if key.len() > 32 { &key[0..32] } else { key };
    let mut cipher = Aes256Ctr64BE::new(key.into(), iv.into());
    let mut data_vec = data.to_vec();
    cipher.try_apply_keystream(&mut data_vec)?;
    Ok(data_vec)
}
