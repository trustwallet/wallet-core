// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

pub mod padding;

use crate::{KEY_SIZE_AES_128, KEY_SIZE_AES_192, KEY_SIZE_AES_256};
use aes::cipher::{
    Block, BlockDecryptMut, BlockEncryptMut, BlockSizeUser, KeyIvInit, StreamCipherError,
};
use padding::{PaddingMode, BLOCK_SIZE_AES};

type Aes128CbcEnc = cbc::Encryptor<aes::Aes128>;
type Aes128CbcDec = cbc::Decryptor<aes::Aes128>;
type Aes192CbcEnc = cbc::Encryptor<aes::Aes192>;
type Aes192CbcDec = cbc::Decryptor<aes::Aes192>;
type Aes256CbcEnc = cbc::Encryptor<aes::Aes256>;
type Aes256CbcDec = cbc::Decryptor<aes::Aes256>;

fn blocks<N: BlockSizeUser>(data: &[u8]) -> Vec<Block<N>> {
    data.chunks(BLOCK_SIZE_AES)
        .map(|chunk| Block::<N>::clone_from_slice(chunk))
        .collect()
}

fn aes_cbc_encrypt_impl<E: KeyIvInit + BlockEncryptMut>(
    data: &[u8],
    iv: &[u8],
    key: &[u8],
    key_size: usize,
    padding_mode: PaddingMode,
) -> Result<Vec<u8>, StreamCipherError> {
    let key = if key.len() > key_size {
        &key[0..key_size]
    } else {
        key
    };
    let data = padding_mode.pad(data);
    let mut blocks = blocks::<E>(&data);
    let mut cipher = E::new(key.into(), iv.into());
    cipher.encrypt_blocks_mut(&mut blocks[..]);
    let buffer = blocks.concat();
    Ok(buffer)
}

fn aes_cbc_decrypt_impl<D: KeyIvInit + BlockDecryptMut>(
    data: &[u8],
    iv: &[u8],
    key: &[u8],
    key_size: usize,
    padding_mode: PaddingMode,
) -> Result<Vec<u8>, StreamCipherError> {
    let key = if key.len() > key_size {
        &key[0..key_size]
    } else {
        key
    };
    if data.len() % BLOCK_SIZE_AES != 0 {
        return Err(StreamCipherError);
    }
    let mut blocks = blocks::<D>(data);
    let mut cipher = D::new(key.into(), iv.into());
    cipher.decrypt_blocks_mut(&mut blocks[..]);
    let buffer = blocks.concat();
    Ok(padding_mode.unpad(&buffer))
}

pub fn aes_cbc_encrypt_128(
    data: &[u8],
    iv: &[u8],
    key: &[u8],
    padding_mode: PaddingMode,
) -> Result<Vec<u8>, StreamCipherError> {
    aes_cbc_encrypt_impl::<Aes128CbcEnc>(data, iv, key, KEY_SIZE_AES_128, padding_mode)
}

pub fn aes_cbc_decrypt_128(
    data: &[u8],
    iv: &[u8],
    key: &[u8],
    padding_mode: PaddingMode,
) -> Result<Vec<u8>, StreamCipherError> {
    aes_cbc_decrypt_impl::<Aes128CbcDec>(data, iv, key, KEY_SIZE_AES_128, padding_mode)
}

pub fn aes_cbc_encrypt_192(
    data: &[u8],
    iv: &[u8],
    key: &[u8],
    padding_mode: PaddingMode,
) -> Result<Vec<u8>, StreamCipherError> {
    aes_cbc_encrypt_impl::<Aes192CbcEnc>(data, iv, key, KEY_SIZE_AES_192, padding_mode)
}

pub fn aes_cbc_decrypt_192(
    data: &[u8],
    iv: &[u8],
    key: &[u8],
    padding_mode: PaddingMode,
) -> Result<Vec<u8>, StreamCipherError> {
    aes_cbc_decrypt_impl::<Aes192CbcDec>(data, iv, key, KEY_SIZE_AES_192, padding_mode)
}

pub fn aes_cbc_encrypt_256(
    data: &[u8],
    iv: &[u8],
    key: &[u8],
    padding_mode: PaddingMode,
) -> Result<Vec<u8>, StreamCipherError> {
    aes_cbc_encrypt_impl::<Aes256CbcEnc>(data, iv, key, KEY_SIZE_AES_256, padding_mode)
}

pub fn aes_cbc_decrypt_256(
    data: &[u8],
    iv: &[u8],
    key: &[u8],
    padding_mode: PaddingMode,
) -> Result<Vec<u8>, StreamCipherError> {
    aes_cbc_decrypt_impl::<Aes256CbcDec>(data, iv, key, KEY_SIZE_AES_256, padding_mode)
}

pub fn aes_cbc_encrypt(
    data: &[u8],
    iv: &[u8],
    key: &[u8],
    padding_mode: PaddingMode,
) -> Result<Vec<u8>, StreamCipherError> {
    match key.len() {
        KEY_SIZE_AES_128 => aes_cbc_encrypt_128(data, iv, key, padding_mode),
        KEY_SIZE_AES_192 => aes_cbc_encrypt_192(data, iv, key, padding_mode),
        KEY_SIZE_AES_256 => aes_cbc_encrypt_256(data, iv, key, padding_mode),
        _ => Err(StreamCipherError),
    }
}

pub fn aes_cbc_decrypt(
    data: &[u8],
    iv: &[u8],
    key: &[u8],
    padding_mode: PaddingMode,
) -> Result<Vec<u8>, StreamCipherError> {
    match key.len() {
        KEY_SIZE_AES_128 => aes_cbc_decrypt_128(data, iv, key, padding_mode),
        KEY_SIZE_AES_192 => aes_cbc_decrypt_192(data, iv, key, padding_mode),
        KEY_SIZE_AES_256 => aes_cbc_decrypt_256(data, iv, key, padding_mode),
        _ => Err(StreamCipherError),
    }
}
