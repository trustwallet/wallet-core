// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

pub mod padding;

use aes::cipher::{BlockDecryptMut, BlockEncryptMut, KeyIvInit, StreamCipherError};
use aes::Block;
use padding::{PaddingMode, BLOCK_SIZE_AES};

type Aes128CbcEnc = cbc::Encryptor<aes::Aes128>;
type Aes128CbcDec = cbc::Decryptor<aes::Aes128>;
type Aes192CbcEnc = cbc::Encryptor<aes::Aes192>;
type Aes192CbcDec = cbc::Decryptor<aes::Aes192>;
type Aes256CbcEnc = cbc::Encryptor<aes::Aes256>;
type Aes256CbcDec = cbc::Decryptor<aes::Aes256>;

fn blocks(data: &[u8]) -> Vec<Block> {
    let mut blocks = Vec::new();
    for i in 0..data.len() / BLOCK_SIZE_AES {
        let start = i * BLOCK_SIZE_AES;
        let end = start + BLOCK_SIZE_AES;
        blocks.push(Block::clone_from_slice(&data[start..end]));
    }
    blocks
}

pub fn aes_cbc_encrypt_128(
    data: &[u8],
    iv: &[u8],
    key: &[u8],
    padding_mode: PaddingMode,
) -> Result<Vec<u8>, StreamCipherError> {
    let key = if key.len() > 16 { &key[0..16] } else { key };
    let data = padding_mode.pad(data);
    let mut blocks = blocks(&data);
    let mut cipher = Aes128CbcEnc::new(key.into(), iv.into());
    cipher.encrypt_blocks_mut(&mut blocks[..]);
    let buffer = blocks.concat();
    Ok(buffer)
}

pub fn aes_cbc_decrypt_128(
    data: &[u8],
    iv: &[u8],
    key: &[u8],
    padding_mode: PaddingMode,
) -> Result<Vec<u8>, StreamCipherError> {
    let key = if key.len() > 16 { &key[0..16] } else { key };
    let mut blocks = blocks(data);
    let mut cipher = Aes128CbcDec::new(key.into(), iv.into());
    cipher.decrypt_blocks_mut(&mut blocks[..]);
    let buffer = blocks.concat();
    Ok(padding_mode.unpad(&buffer))
}

pub fn aes_cbc_encrypt_192(
    data: &[u8],
    iv: &[u8],
    key: &[u8],
    padding_mode: PaddingMode,
) -> Result<Vec<u8>, StreamCipherError> {
    let key = if key.len() > 24 { &key[0..24] } else { key };
    let data = padding_mode.pad(data);
    let mut blocks = blocks(&data);
    let mut cipher = Aes192CbcEnc::new(key.into(), iv.into());
    cipher.encrypt_blocks_mut(&mut blocks[..]);
    let buffer = blocks.concat();
    Ok(buffer)
}

pub fn aes_cbc_decrypt_192(
    data: &[u8],
    iv: &[u8],
    key: &[u8],
    padding_mode: PaddingMode,
) -> Result<Vec<u8>, StreamCipherError> {
    let key = if key.len() > 24 { &key[0..24] } else { key };
    let mut blocks = blocks(data);
    let mut cipher = Aes192CbcDec::new(key.into(), iv.into());
    cipher.decrypt_blocks_mut(&mut blocks[..]);
    let buffer = blocks.concat();
    Ok(padding_mode.unpad(&buffer))
}

pub fn aes_cbc_encrypt_256(
    data: &[u8],
    iv: &[u8],
    key: &[u8],
    padding_mode: PaddingMode,
) -> Result<Vec<u8>, StreamCipherError> {
    let key = if key.len() > 32 { &key[0..32] } else { key };
    let data = padding_mode.pad(data);
    let mut blocks = blocks(&data);
    let mut cipher = Aes256CbcEnc::new(key.into(), iv.into());
    cipher.encrypt_blocks_mut(&mut blocks[..]);
    let buffer = blocks.concat();
    Ok(buffer)
}

pub fn aes_cbc_decrypt_256(
    data: &[u8],
    iv: &[u8],
    key: &[u8],
    padding_mode: PaddingMode,
) -> Result<Vec<u8>, StreamCipherError> {
    let key = if key.len() > 32 { &key[0..32] } else { key };
    let mut blocks = blocks(data);
    let mut cipher = Aes256CbcDec::new(key.into(), iv.into());
    cipher.decrypt_blocks_mut(&mut blocks[..]);
    let buffer = blocks.concat();
    Ok(padding_mode.unpad(&buffer))
}

pub fn aes_cbc_encrypt(
    data: &[u8],
    iv: &[u8],
    key: &[u8],
    padding_mode: PaddingMode,
) -> Result<Vec<u8>, StreamCipherError> {
    match key.len() {
        16 => aes_cbc_encrypt_128(data, iv, key, padding_mode),
        24 => aes_cbc_encrypt_192(data, iv, key, padding_mode),
        32 => aes_cbc_encrypt_256(data, iv, key, padding_mode),
        _ => return Err(StreamCipherError),
    }
}

pub fn aes_cbc_decrypt(
    data: &[u8],
    iv: &[u8],
    key: &[u8],
    padding_mode: PaddingMode,
) -> Result<Vec<u8>, StreamCipherError> {
    match key.len() {
        16 => aes_cbc_decrypt_128(data, iv, key, padding_mode),
        24 => aes_cbc_decrypt_192(data, iv, key, padding_mode),
        32 => aes_cbc_decrypt_256(data, iv, key, padding_mode),
        _ => return Err(StreamCipherError),
    }
}
